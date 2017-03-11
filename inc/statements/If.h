#pragma once

#include <memory>

#include "location.hh"

#include "instructions/Instruction.h"
#include "booleans/Boolean.h"

#include "statements/Statement.h"

namespace cpq { namespace statements {

/**
 * @brief Represents a CPL if statement.
 */
class If : public Statement {
public:

	/**
	 * @brief Create an if statement.
	 *
	 * @param loc - The location of this statement in source code.
	 * @param condition - The condition by which to decide which statement to execute.
	 * @param positive - The statement to execute if the condition was true.
	 * @param negative - The statement to execute if the condition was false.
	 */
	If(
		const cpq::location& loc,
		const std::shared_ptr<booleans::Boolean> condition,
		const std::shared_ptr<Statement> positive,
		const std::shared_ptr<Statement> negative);

	virtual void generate(instructions::InstructionList& insts) const override;
	
private:
	const std::shared_ptr<booleans::Boolean> 	m_condition;
	const std::shared_ptr<Statement>		 	m_positive;
	const std::shared_ptr<Statement>		 	m_negative;
};


}}
