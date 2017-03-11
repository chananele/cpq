#pragma once

#include <memory>

#include "location.hh"

#include "instructions/Instruction.h"
#include "statements/Statement.h"

namespace cpq { namespace statements {
	
/**
 * @brief Represents a block of CPL statements to be executed sequentially.
 */
class StatementBlock : public Statement {
public:

	/**
	 * @brief Create a statement block.
	 *
	 * @param loc - The location of this statement block in source code.
	 * @param stmts - The list of statements in this block.
	 */
	StatementBlock(const location& loc, const std::shared_ptr<StatementList> stmts);

	virtual void generate(instructions::InstructionList& insts) const override;

private:
	const std::shared_ptr<StatementList> m_stmts;
};
	
}}
