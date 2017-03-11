#include <memory>

#include "location.hh"

#include "symbol.h"

#include "instructions/Instruction.h"
#include "instructions/Branch.h"
#include "instructions/Label.h"
#include "instructions/Jump.h"

#include "booleans/Boolean.h"

#include "statements/Statement.h"
#include "statements/If.h"

namespace cpq { namespace statements {

If::If(
	const cpq::location& loc,
	const std::shared_ptr<booleans::Boolean> condition,
	const std::shared_ptr<Statement> positive,
	const std::shared_ptr<Statement> negative
)
	: Statement(loc)
	, m_condition(condition)
	, m_positive(positive)
	, m_negative(negative)
{}

void If::generate(instructions::InstructionList& insts) const
{
	auto symbol(std::make_shared<Symbol>(symbol_type_e::INT));
	m_condition->generate(insts, symbol);

	auto alternative(std::make_shared<instructions::Label>());
	auto end(std::make_shared<instructions::Label>());

	insts.push_back(std::make_unique<instructions::Branch>(
		symbol->var(),
		alternative
		));

	m_positive->generate(insts);
	insts.push_back(std::make_unique<instructions::Jump>(end));
	alternative->set(insts.size());

	m_negative->generate(insts);
	end->set(insts.size());
}

}}
