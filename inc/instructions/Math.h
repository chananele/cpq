#pragma once

#include <string>

#include "symbol.h"
#include "instructions/Instruction.h"

namespace cpq { namespace instructions {

/**
 * @brief Represent a QUAD mathematical instruction (i.e. ADD, SUBTRACT, MULTIPLY or DIVIDE).
 *
 * @param T - The specific mathematical instruction.
 */
template <class T>
class BinaryOperation : public Instruction {
public:

	/**
	 * @brief Creates a mathematical instruction of type T.
	 *
	 * @param type  - The type of the target variable.
	 * @param dest  - The variable to which the result of the instruction should be assigned.
	 * @param left  - The left operand of the instruction.
	 * @param right - The right operand of the instruction.
	 */
	BinaryOperation(
		symbol_type_e type,
		const std::string& dest,
		const std::string& left,
		const std::string& right)
	: m_type(type)
	, m_dest(dest)
	, m_left(left)
	, m_right(right)
	{}

	virtual std::string generate() const override 
	{
		return T::instruction[m_type] + " " + m_dest + " " + m_left + " " + m_right;
	}

private:
	const symbol_type_e m_type;
	const std::string m_dest;
	const std::string m_left;
	const std::string m_right;
};

/**
 * @brief The binary addition operation.
 */
class Add : public BinaryOperation<Add> {
public:
	using BinaryOperation<Add>::BinaryOperation;
	static const std::string instruction[];
};

/**
 * @brief The binary subtraction operation.
 */
class Subtract : public BinaryOperation<Subtract> {
public:
	using BinaryOperation<Subtract>::BinaryOperation;
	static const std::string instruction[];
};

/**
 * @brief The binary multiplication operation.
 */
class Multiply : public BinaryOperation<Multiply> {
public:
	using BinaryOperation<Multiply>::BinaryOperation;
	static const std::string instruction[];
};

/**
 * @brief The binary division operation.
 */
class Divide : public BinaryOperation<Divide> {
public:
	using BinaryOperation<Divide>::BinaryOperation;
	static const std::string instruction[];
};

}}
