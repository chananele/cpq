#pragma once

#include <memory>

#include "location.hh"

#include "symbol.h"

#include "instructions/Instruction.h"
#include "instructions/Cast.h"
#include "instructions/Math.h"

#include "expressions/Expression.h"

namespace cpq { namespace expressions {

/**
 * Represents a CPL mathematical binary expression.
 *
 * The implicit contract on type parameter T is:
 *
 *     T::operation is a binary instruction type. (see instructions/Math.h)
 */
template <class T>
class BinaryExpression : public Expression
{
public:

	/**
	 * @brief Create a mathematical binary expression of type T.
	 *
	 * @param loc - The location of this expression in source code.
	 * @param left - The left operand of this expression.
	 * @param right - The right operand of this expression.
	 */
	BinaryExpression(
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
		instructions::InstructionList& insts,
		const std::shared_ptr<Symbol> symbol
		) const override
	{
		auto first 	= m_left;
		auto second = m_right;

		if (first->type() != type()) { auto temp = second; second = first; first = temp; }

		auto temp(std::make_shared<Symbol>(second->type()));
		first->generate(insts, symbol);
		second->generate(insts, temp);

		if (second->type() != type()) { // implicit cast
			auto converted(std::make_shared<Symbol>(type()));
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

/**
 * @brief CPL expression multiplication.
 */
class Multiply : public BinaryExpression<Multiply>
{
public:
	using BinaryExpression<Multiply>::BinaryExpression;
	using operation = instructions::Multiply;
};

/**
 * @brief CPL expression division.
 */
class Divide : public BinaryExpression<Divide>
{
public:
	using BinaryExpression<Divide>::BinaryExpression;
	using operation = instructions::Divide;
};


/**
 * @brief CPL expression addition.
 */
class Add : public BinaryExpression<Add>
{
public:
	using BinaryExpression<Add>::BinaryExpression;
	using operation = instructions::Add;
};

/**
 * @brief CPL expression subtraction.
 */
class Subtract : public BinaryExpression<Subtract>
{
public:
	using BinaryExpression<Subtract>::BinaryExpression;
	using operation = instructions::Subtract;
};

/**
 * @brief Returns the expression of the type represented by the given character.
 *
 * @param loc - The location of the expression in source code.
 * @param operation - The char representation of the operation type: '+', '-', '*' or '/'.
 * @param left - The left operand of the expression.
 * @param right - The right operand of the expression.
 *
 * @return The expression resulting from applying the operation to the given operands.
 */
std::shared_ptr<Expression> binary(
	const cpq::location& loc,
	char operation,
	const std::shared_ptr<Expression> left,
	const std::shared_ptr<Expression> right
	);
	
}}
