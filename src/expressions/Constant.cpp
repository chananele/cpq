#include <memory>

#include "symbol.h"

#include "instructions/Instruction.h"
#include "instructions/Assign.h"

#include "expressions/Constant.h"

namespace cpq { namespace expressions {
	
void Constant::generate(
	instructions::InstructionList& insts,
	const std::shared_ptr<Symbol> symbol
	) const
{
	insts.push_back(std::make_unique<instructions::Assign>(
		symbol->type(),
		symbol->var(),
		str()
		));
}
	
}}
