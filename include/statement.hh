#pragma once

#include <memory>
#include <vector>
#include <list>

#include "location.hh"
#include "instruction.hh"
#include "symbol.hh"

namespace cpq {

class Statement {
public:
	Statement(const cpq::location& loc)
		: m_loc(loc)
	{}

	virtual void generate(std::vector<std::unique_ptr<Instruction>>& instructions) const = 0;

protected:
	const cpq::location m_loc;
};

class Expression {
public:

	Expression(const cpq::location& loc)
		: m_loc(loc)
	{}

	virtual void generate(
		std::vector<std::unique_ptr<Instruction>>& instructions,
		const std::shared_ptr<Symbol> symbol
		) const = 0;

	virtual symbol_type_e type() const = 0;

protected:
	const cpq::location m_loc;
};

class Load : public Expression
{
public:
	Load(const cpq::location& loc, const std::shared_ptr<Symbol> symbol)
		: Expression(loc)
		, m_symbol(symbol)
	{}

	virtual symbol_type_e type() const override
	{
		return m_symbol->type();
	}

	virtual void generate(
			std::vector<std::unique_ptr<Instruction>>& instructions,
			const std::shared_ptr<Symbol> symbol
			) const override
	{
		instructions.push_back(std::make_unique<Assign>(
			symbol->type(),
			symbol->var(),
			m_symbol->var()
			));
	}

private:
	const std::shared_ptr<Symbol> m_symbol;
};

template <class T>
class BinaryExpression : public Expression
{
public:
	explicit BinaryExpression(
		const cpq::location& loc,
		const std::shared_ptr<Expression> left,
		const std::shared_ptr<Expression> right
	)
	: Expression(loc)
	, m_left(left)
	, m_right(right)
	{}

	virtual symbol_type_e type() const override
	{
		if (m_left->type() == symbol_type_e::REAL) {
			return symbol_type_e::REAL;
		}
		if (m_right->type() == symbol_type_e::REAL) {
			return symbol_type_e::REAL;
		}
		return symbol_type_e::INT;
	}

	virtual void generate(
		std::vector<std::unique_ptr<Instruction>>& instructions,
		const std::shared_ptr<Symbol> symbol
		) const override
	{
		auto first 	= m_left;
		auto second = m_right;

		if (first->type() != type()) { auto temp = second; second = first; first = temp; }

		auto temp(std::make_shared<Symbol>("", second->type()));
		first->generate(instructions, symbol);
		second->generate(instructions, temp);

		if (second->type() != type()) {
			auto converted(std::make_shared<Symbol>("", type()));
			instructions.push_back(std::make_unique<Cast>(
				type(),
				converted->var(),
				temp->var()));
			temp = converted;
		}

		instructions.push_back(std::make_unique<typename T::operation>(
			symbol->type(),
			symbol->var(),
			symbol->var(),
			temp->var()
			));
	}

private:
	const std::shared_ptr<Expression> m_left;
	const std::shared_ptr<Expression> m_right;
};

class Multiplication : public BinaryExpression<Multiplication>
{
public:
	using BinaryExpression<Multiplication>::BinaryExpression;
	using operation = Multiply;
};

class Division : public BinaryExpression<Division>
{
public:
	using BinaryExpression<Division>::BinaryExpression;
	using operation = Divide;
};

class Addition : public BinaryExpression<Addition>
{
public:
	using BinaryExpression<Addition>::BinaryExpression;
	using operation = Add;
};

class Subtraction : public BinaryExpression<Subtraction>
{
public:
	using BinaryExpression<Subtraction>::BinaryExpression;
	using operation = Subtract;
};

class Constant : public Expression {
public:
	using Expression::Expression;

	virtual std::string str() const = 0;

	virtual void generate(
			std::vector<std::unique_ptr<Instruction>>& instructions,
			const std::shared_ptr<Symbol> symbol
			) const override
	{
		instructions.push_back(std::make_unique<Assign>(
			symbol->type(),
			symbol->var(),
			str()
			));
	}
};

template <class T>
class Trait
{};

class Int;
class Real;

template <>
class Trait<Int> {
public:
	using type = int;
	static const symbol_type_e symbol_type = symbol_type_e::INT;
};

template <>
class Trait<Real> {
public:
	using type = float;
	static const symbol_type_e symbol_type = symbol_type_e::REAL;
};

template <class T>
class Trivial : public Constant
{
public:
	using value_type = typename Trait<T>::type;

	Trivial(const cpq::location& loc, value_type value)
		: Constant(loc)
		, m_value(value)
	{}

	virtual symbol_type_e type() const override {
		return Trait<T>::symbol_type;
	}

	virtual std::string str() const override {
		return std::to_string(m_value);
	}

private:
	value_type m_value;
};

class Int : public Trivial<Int> {
public:
	using Trivial<Int>::Trivial;
};

class Real : public Trivial<Real> {
public:
	 using Trivial<Real>::Trivial;
};

class Assignment : public Statement {
public:
	Assignment(
		const cpq::location& loc,
		const std::shared_ptr<Symbol> symbol,
		const std::shared_ptr<Expression> expression
		);

	virtual void generate(std::vector<std::unique_ptr<Instruction>>& instructions) const override
	{
		auto dest = m_symbol;
		if (m_expression->type() != m_symbol->type()) {
			auto temp = std::make_shared<Symbol>("", m_symbol->type());

			m_expression->generate(
				instructions,
				temp
				);
			instructions.push_back(std::make_unique<Cast>(
				m_symbol->type(),
				m_symbol->var(),
				temp->var()
				));
		}
		else {
			m_expression->generate(
				instructions,
				m_symbol
				);
		}
	}

private:
	const std::shared_ptr<Symbol>		m_symbol;
	const std::shared_ptr<Expression>	m_expression;
};

using StatementList = std::list<std::unique_ptr<Statement>>;

}
