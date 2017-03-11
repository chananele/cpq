#pragma once

#include <memory>

#include "location.hh"

#include "symbol.h"

#include "instructions/Instruction.h"

namespace cpq { namespace expressions {
	
/**
 * @brief The abstract class All CPL expressions inherit from.
 */
class Expression {
public:

	/**
	 * @brief Create an expression.
	 *
	 * @param loc - The location of this expression in the source code.
	 */
	Expression(const cpq::location& loc);

	/**
	 * @brief Generate the QUAD instructions needed to evaluate this expression.
	 *
	 * @param insts - The stream to which the instructions should be appended.
	 * @param symbol - The variable to which the value of the expression should be assigned.
	 */
	virtual void generate(
		instructions::InstructionList& insts,
		const std::shared_ptr<Symbol> symbol
		) const = 0;

	/**
	 * @brief Returns the type of the value of this expression.
	 */
	virtual symbol_type_e type() const = 0;

protected:
	const cpq::location m_loc;
};
	
}}
