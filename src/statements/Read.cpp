#include <memory>

#include "location.hh"

#include "instructions/Instruction.h"
#include "instructions/Read.h"

#include "statements/Read.h"

namespace cpq { namespace statements {
	
Read::Read(const cpq::location& loc, const std::shared_ptr<Symbol> symbol)
	: Statement(loc)
	, m_symbol(symbol)
{}
	
void Read::generate(instructions::InstructionList& insts) const
{
	insts.push_back(std::make_unique<instructions::Read>(
		m_symbol->type(),
		m_symbol->var()
		));
}
	
}}
