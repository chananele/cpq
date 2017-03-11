#pragma once

#include <memory>
#include <list>

#include "location.hh"
#include "instructions/Instruction.h"

namespace cpq { namespace statements {
	
/**
 * @brief The abstract class all CPL statements must inherit from.
 */
class Statement {
public:

	/**
	 * @brief Create a statement.
	 *
	 * @param loc - The location of this statement in the source code.
	 */
	Statement(const cpq::location& loc);

	/**
	 * @brief Generate the QUAD instructions needed to evaluate this statement.
	 *
	 * @param insts - The stream to which the instructions should be appended.
	 */
	virtual void generate(instructions::InstructionList& insts) const = 0;

protected:
	const cpq::location m_loc;
};

/**
 * @brief Complete the program.
 * @summary - This inserts the halt instruction, after which no more instructions
 *            may be executed.
 */
void finish(instructions::InstructionList& insts);

using StatementList = std::list<std::shared_ptr<Statement>>;
	
}}
