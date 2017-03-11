#include <memory>

#include "location.hh"

#include "instructions/Instruction.h"

#include "statements/Statement.h"
#include "statements/StatementBlock.h"

namespace cpq { namespace statements {
	
StatementBlock::StatementBlock(const location& loc, const std::shared_ptr<StatementList> stmts)
	: Statement(loc)
	, m_stmts(stmts)
{}

void StatementBlock::generate(instructions::InstructionList& insts) const
{
	for (const auto& stmt : *m_stmts) {
		stmt->generate(insts);
	}
}

}}
