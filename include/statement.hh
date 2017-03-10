#pragma once

#include <memory>
#include <vector>
#include <list>

#include "location.hh"
#include "instruction.hh"
#include "symbol.hh"

namespace cpq {

namespace statements {

class Statement {
public:
	Statement(const cpq::location& loc)
		: m_loc(loc)
	{}

	virtual void generate(std::vector<std::unique_ptr<instructions::Instruction>>& insts) const = 0;

protected:
	const cpq::location m_loc;
};

class Expression {
public:

	Expression(const cpq::location& loc)
		: m_loc(loc)
	{}

	virtual void generate(
		std::vector<std::unique_ptr<instructions::Instruction>>& insts,
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
			std::vector<std::unique_ptr<instructions::Instruction>>& insts,
			const std::shared_ptr<Symbol> symbol
			) const override;

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
		std::vector<std::unique_ptr<instructions::Instruction>>& insts,
		const std::shared_ptr<Symbol> symbol
		) const override
	{
		auto first 	= m_left;
		auto second = m_right;

		if (first->type() != type()) { auto temp = second; second = first; first = temp; }

		auto temp(std::make_shared<Symbol>("", second->type()));
		first->generate(insts, symbol);
		second->generate(insts, temp);

		if (second->type() != type()) {
			auto converted(std::make_shared<Symbol>("", type()));
			insts.push_back(std::make_unique<instructions::Cast>(
				type(),
				converted->var(),
				temp->var()));
			temp = converted;
		}

		insts.push_back(std::make_unique<typename T::operation>(
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

class Multiply : public BinaryExpression<Multiply>
{
public:
	using BinaryExpression<Multiply>::BinaryExpression;
	using operation = instructions::Multiply;
};

class Divide : public BinaryExpression<Divide>
{
public:
	using BinaryExpression<Divide>::BinaryExpression;
	using operation = instructions::Divide;
};

class Add : public BinaryExpression<Add>
{
public:
	using BinaryExpression<Add>::BinaryExpression;
	using operation = instructions::Add;
};

class Subtract : public BinaryExpression<Subtract>
{
public:
	using BinaryExpression<Subtract>::BinaryExpression;
	using operation = instructions::Subtract;
};

class Constant : public Expression {
public:
	using Expression::Expression;

	virtual std::string str() const = 0;

	virtual void generate(
			std::vector<std::unique_ptr<instructions::Instruction>>& insts,
			const std::shared_ptr<Symbol> symbol
			) const override;
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

	virtual void generate(
			std::vector<std::unique_ptr<instructions::Instruction>>& insts
			) const override;

private:
	const std::shared_ptr<Symbol>		m_symbol;
	const std::shared_ptr<Expression>	m_expression;
};

class Read : public Statement
{
public:
	Read(const cpq::location& loc, const std::shared_ptr<Symbol> symbol)
		: Statement(loc)
		, m_symbol(symbol)
	{}

	virtual void generate(
			std::vector<std::unique_ptr<instructions::Instruction>>& insts
			) const override;

private:
	const std::shared_ptr<Symbol> m_symbol;
};

class Write : public Statement
{
public:
	Write(const cpq::location& loc, const std::shared_ptr<Expression> expression)
		: Statement(loc)
		, m_expression(expression)
	{}

	virtual void generate(
			std::vector<std::unique_ptr<instructions::Instruction>>& insts
			) const override;

private:
	const std::shared_ptr<Expression> m_expression;
};

class Cast : public Statement {
public:
	Cast(
		const cpq::location& loc,
		const std::shared_ptr<Symbol> symbol,
		const std::shared_ptr<Expression> expression
	)
		: Statement(loc)
		, m_symbol(symbol)
		, m_expression(expression)
	{}

	virtual void generate(
			std::vector<std::unique_ptr<instructions::Instruction>>& insts
			) const override;

private:
	const std::shared_ptr<Symbol> m_symbol;
	const std::shared_ptr<Expression> m_expression;
};

using StatementList = std::list<std::shared_ptr<Statement>>;

class StatementBlock : public Statement {
public:
	StatementBlock(const location& loc, const std::shared_ptr<StatementList> stmts)
		: Statement(loc)
		, m_stmts(stmts)
	{}

	virtual void generate(
			std::vector<std::unique_ptr<instructions::Instruction>>& insts
			) const override;

private:
	const std::shared_ptr<StatementList> m_stmts;
};

class NullStatement : public Statement {
public:
	NullStatement(const location& loc)
		: Statement(loc)
	{}

	virtual void generate(
		std::vector<std::unique_ptr<instructions::Instruction>>& insts
		) const override
	{}
};

void finish(std::vector<std::unique_ptr<instructions::Instruction>>& insts);

}

}
