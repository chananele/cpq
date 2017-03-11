#pragma once

#include <memory>

#include "location.hh"

#include "instructions/Instruction.h"
#include "booleans/Boolean.h"

#include "statements/Statement.h"

namespace cpq { namespace statements {

/**
 * @brief Represents a CPL while statement.
 */
class While : public Statement {
public:

	/**
	 * @brief Create a while statement.
	 *
	 * @param loc - The location of this statement in source code.
	 * @param boolean - The loop condition.
	 * @param statement - The statement to execute in the loop.
	 */
	While(
		const cpq::location& loc,
		const std::shared_ptr<booleans::Boolean> boolean,
		const std::shared_ptr<Statement> statement
		);

	virtual void generate(instructions::InstructionList& insts) const override;

private:
	const std::shared_ptr<booleans::Boolean>	m_boolean;
	const std::shared_ptr<Statement> 			m_statement;
};

}}
