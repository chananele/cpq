#include <memory>

#include "location.hh"

#include "symbol.h"

#include "instructions/Instruction.h"
#include "instructions/Inequality.h"

#include "booleans/Basic.h"

namespace cpq { namespace booleans {
	
Negate::Negate(const cpq::location& loc, const std::shared_ptr<Boolean> boolean)
	: Boolean(loc)
	, m_boolean(boolean)
{}

void Negate::generate(
	std::vector<std::unique_ptr<instructions::Instruction>>& insts,
	const std::shared_ptr<Symbol> symbol
) const
{
	m_boolean->generate(insts, symbol);
	insts.push_back(std::make_unique<instructions::Inequality>(
		comparison_e::NEQ,
		symbol_type_e::INT,
		symbol->var(),
		symbol->var(),
		"1"
		));
}
	
}}
