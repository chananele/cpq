#pragma once

#include <initializer_list>

#include <memory>
#include <vector>
#include <string>
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

std::shared_ptr<Expression> binary(
	const cpq::location& loc,
	char operation,
	const std::shared_ptr<Expression> left,
	const std::shared_ptr<Expression> right
	);

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

class Boolean {
public:
	virtual void generate(
		std::vector<std::unique_ptr<instructions::Instruction>>& insts,
		const std::shared_ptr<Symbol> symbol
		) const = 0;
};

template <bool res>
class Connective : public Boolean {
public:
	Connective(const std::shared_ptr<Boolean> left, const std::shared_ptr<Boolean> right)
		: m_left(left)
		, m_right(right)
	{}

	virtual void generate(
		std::vector<std::unique_ptr<instructions::Instruction>>& insts,
		const std::shared_ptr<Symbol> symbol
	) const override
	{
		auto label(std::make_shared<instructions::Label>());
		m_left->generate(insts, symbol);
		auto comp(std::make_shared<Symbol>("", symbol_type_e::INT));
		insts.push_back(std::make_unique<instructions::Inequality>(
			comparison_e::NEQ,
			symbol_type_e::INT,
			comp->var(),
			symbol->var(),
			std::to_string(int(res))
		));
		insts.push_back(std::make_unique<instructions::Branch>(comp->var(), label));
		m_right->generate(insts, symbol);
		label->set(insts.size());
	}

private:
	const std::shared_ptr<Boolean> m_left;
	const std::shared_ptr<Boolean> m_right;
};

using Or =  Connective<true>;
using And = Connective<false>;

class Negate : public Boolean {
public:
	Negate(const std::shared_ptr<Boolean> boolean)
		: m_boolean(boolean)
	{}

	virtual void generate(
		std::vector<std::unique_ptr<instructions::Instruction>>& insts,
		const std::shared_ptr<Symbol> symbol
	) const override
	{
		m_boolean->generate(insts, symbol);
		insts.push_back(std::make_unique<instructions::Inequality>(
			comparison_e::NEQ,
			symbol_type_e::INT,
			symbol->var(),
			symbol->var(),
			"1"
			));
	}

private:
	const std::shared_ptr<Boolean> m_boolean;
};

class Comparison : public Boolean {
public:
	Comparison(
		const comparison_e comp,
		const std::shared_ptr<Expression> left,
		const std::shared_ptr<Expression> right
	)
		: m_comp(comp)
		, m_left(left)
		, m_right(right)
	{}

	virtual void generate(
		std::vector<std::unique_ptr<instructions::Instruction>>& insts,
		const std::shared_ptr<Symbol> symbol
	) const override
	{
		auto first	(std::make_shared<Symbol>("", m_left->type()));
		auto second	(std::make_shared<Symbol>("", m_right->type()));

		m_left	->generate(insts, first);
		m_right	->generate(insts, second);

		if (first->type() == type()) { auto temp = first; first = second; second = temp; }

		auto converted(first);
		if (first->type() != type()) {
			converted = std::make_shared<Symbol>("", type());
			insts.push_back(std::make_unique<instructions::Cast>(
				converted->type(),
				converted->var(),
				first->var()
				));
		}
		insts.push_back(std::make_unique<instructions::Inequality>(
			m_comp,
			type(),
			symbol->var(),
			converted->var(),
			second->var()
			));
	}

private:
	symbol_type_e type() const {
		if (m_left->type() == symbol_type_e::REAL) {
			return symbol_type_e::REAL;
		}
		if (m_right->type() == symbol_type_e::REAL) {
			return symbol_type_e::REAL;
		}
		return symbol_type_e::INT;
	}

	const comparison_e					m_comp;
	const std::shared_ptr<Expression> 	m_left;
	const std::shared_ptr<Expression> 	m_right;
};

class WhileStatement : public Statement {
public:
	WhileStatement(
		const cpq::location& loc,
		const std::shared_ptr<Boolean> boolean,
		const std::shared_ptr<Statement> statement
		)
		: Statement(loc)
		, m_boolean(std::make_shared<Negate>(boolean))
		, m_statement(statement)
	{}

	virtual void generate(std::vector<std::unique_ptr<instructions::Instruction>>& insts) const override {

		auto result(std::make_shared<Symbol>("", symbol_type_e::INT));

		auto condition(std::make_shared<instructions::Label>());
		auto code(std::make_shared<instructions::Label>());

		insts.push_back(std::make_unique<instructions::Jump>(condition));
		code->set(insts.size());

		m_statement->generate(insts);
		condition->set(insts.size());

		m_boolean->generate(insts, result);
		insts.push_back(std::make_unique<instructions::Branch>(
			result->var(),
			code
			));
	}

private:
	const std::shared_ptr<Boolean> m_boolean;
	const std::shared_ptr<Statement> m_statement;
};

class UntilStatement : public Statement {
public:
	UntilStatement(
		const cpq::location& loc,
		const std::shared_ptr<Boolean> boolean,
		const std::shared_ptr<Statement> statement
		)
		: Statement(loc)
		, m_boolean(boolean)
		, m_statement(statement)
	{}

	virtual void generate(std::vector<std::unique_ptr<instructions::Instruction>>& insts) const override {

		auto result(std::make_shared<Symbol>("", symbol_type_e::INT));

		auto condition(std::make_shared<instructions::Label>());

		auto code(std::make_shared<instructions::Label>());
		code->set(insts.size());

		m_statement->generate(insts);
		condition->set(insts.size());

		m_boolean->generate(insts, result);
		insts.push_back(std::make_unique<instructions::Branch>(
			result->var(),
			code
			));
	}

private:
	const std::shared_ptr<Boolean> m_boolean;
	const std::shared_ptr<Statement> m_statement;
};

class ForStatement : public Statement {
public:
	ForStatement(
		const cpq::location& loc,
		const std::shared_ptr<Statement> assignment,
		const std::shared_ptr<Boolean> condition,
		const std::shared_ptr<Statement> step,
		const std::shared_ptr<Statement> statement
		)
	: Statement(loc)
	, m_assignment(assignment)
	, m_loop(std::make_shared<WhileStatement>(
		loc,
		condition,
		std::make_shared<StatementBlock>(
			loc,
			std::make_shared<StatementList>(
				std::initializer_list<std::shared_ptr<Statement>>({statement, step}))
			)
		))
	{}

	virtual void generate(std::vector<std::unique_ptr<instructions::Instruction>>& insts) const override {
		m_assignment->generate(insts);
		m_loop->generate(insts);
	}

private:
	const std::shared_ptr<Statement> 	m_assignment;
	const std::shared_ptr<Statement>	m_loop;
};

class IfStatement : public Statement {
public:
	IfStatement(
		const cpq::location& loc,
		const std::shared_ptr<Boolean> condition,
		const std::shared_ptr<Statement> positive,
		const std::shared_ptr<Statement> negative)
	: Statement(loc)
	, m_condition(condition)
	, m_positive(positive)
	, m_negative(negative)
	{}

	virtual void generate(std::vector<std::unique_ptr<instructions::Instruction>>& insts) const override {
		auto symbol(std::make_shared<Symbol>("", symbol_type_e::INT));
		m_condition->generate(insts, symbol);

		auto alternative(std::make_shared<instructions::Label>());
		auto end(std::make_shared<instructions::Label>());

		insts.push_back(std::make_unique<instructions::Branch>(
			symbol->var(),
			alternative
			));

		m_positive->generate(insts);
		insts.push_back(std::make_unique<instructions::Jump>(end));
		alternative->set(insts.size());

		m_negative->generate(insts);
		end->set(insts.size());
	}

private:
	const std::shared_ptr<Boolean> 		m_condition;
	const std::shared_ptr<Statement> 	m_positive;
	const std::shared_ptr<Statement> 	m_negative;
};

class Case
{
public:
	Case(const cpq::location& loc,
		const std::shared_ptr<Constant> constant,
		const std::shared_ptr<Statement> statement
		)
		: m_loc(loc)
		, m_constant(constant)
		, m_statement(statement)
	{}

	const cpq::location& 				m_loc;
	const std::shared_ptr<Constant>		m_constant;
	const std::shared_ptr<Statement>	m_statement;
};

class Switch : public Statement {
public:
	Switch (const cpq::location& loc)
		: Statement(loc)
	{}

	void set_expression(const std::shared_ptr<Expression> expr);
	void add_case(std::unique_ptr<Case> c);
	void set_default_case(std::shared_ptr<Statement> stmt);

	virtual void generate(std::vector<std::unique_ptr<instructions::Instruction>>& insts) const override;

private:
	std::vector<std::unique_ptr<Case>>	m_cases;
	std::shared_ptr<Statement>			m_default;
	std::shared_ptr<Expression>			m_expression;
};

void finish(std::vector<std::unique_ptr<instructions::Instruction>>& insts);

}

}
