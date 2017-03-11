#include <memory>

#include "location.hh"

#include "symbol.h"

#include "instructions/Instruction.h"
#include "instructions/Assign.h"

#include "expressions/Load.h"

namespace cpq { namespace expressions {
	
Load::Load(const cpq::location& loc, const std::shared_ptr<Symbol> symbol)
	: Expression(loc)
	, m_symbol(symbol)
{}

symbol_type_e Load::type() const
{
	return m_symbol->type();
}

void Load::generate(
	instructions::InstructionList& insts,
	const std::shared_ptr<Symbol> symbol
	) const
{
	insts.push_back(std::make_unique<instructions::Assign>(
		symbol->type(),
		symbol->var(),
		m_symbol->var()
		));
}
	
}}
