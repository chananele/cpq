#pragma once

#include <memory>

#include "location.hh"

#include "instructions/Instruction.h"
#include "booleans/Boolean.h"

#include "statements/Statement.h"

namespace cpq { namespace statements {

/**
 * @brief Represents a CPL until statement.
 */
class Until : public Statement {
public:

	/**
	 * @brief Create an until statement.
	 *
	 * @param loc - The location of this statement in source code.
	 * @param boolean - The loop stop condition.
	 * @param statement - The statement to execute in the loop.
	 */
	Until(
		const cpq::location& loc,
		const std::shared_ptr<booleans::Boolean> boolean,
		const std::shared_ptr<Statement> statement
		);

	virtual void generate(instructions::InstructionList& insts) const override;
private:
	const std::shared_ptr<booleans::Boolean> m_boolean;
	const std::shared_ptr<Statement> m_statement;
};

}}
