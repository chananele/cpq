#include <memory>

#include "location.hh"

#include "instructions/Instruction.h"
#include "instructions/Write.h"

#include "expressions/Expression.h"
#include "statements/Write.h"

namespace cpq { namespace statements {

Write::Write(const cpq::location& loc, const std::shared_ptr<expressions::Expression> expression)
	: Statement(loc)
	, m_expression(expression)
{}

void Write::generate(instructions::InstructionList& insts) const
{
	auto temp(std::make_shared<Symbol>(m_expression->type()));
	m_expression->generate(insts, temp);

	insts.push_back(std::make_unique<instructions::Write>(
		temp->type(),
		temp->var()
		));
}

}}
