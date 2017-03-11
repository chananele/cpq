#pragma once

#include <memory>

#include "location.hh"

#include "instructions/Instruction.h"
#include "booleans/Boolean.h"

#include "statements/Statement.h"

namespace cpq { namespace statements {

/**
 * @brief Represents a CPL for statement.
 */
class For : public Statement {
public:

	/**
	 * @brief Create a for statement.
	 *
	 * @param loc - The location of this statement in code.
	 * @param assignment - The assignment executed before the loop.
	 * @param condition - The loop condition.
	 * @param step - The statement executed after each loop.
	 * @param statement - The loop statement.
	 */
	For(
		const cpq::location& loc,
		const std::shared_ptr<Statement> assignment,
		const std::shared_ptr<booleans::Boolean> condition,
		const std::shared_ptr<Statement> step,
		const std::shared_ptr<Statement> statement
		);

	virtual void generate(std::vector<std::unique_ptr<instructions::Instruction>>& insts) const override;

private:
	const std::shared_ptr<Statement> 	m_assignment;
	const std::shared_ptr<Statement>	m_loop;
};

}}
