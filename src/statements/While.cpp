#include <memory>

#include "location.hh"

#include "symbol.h"

#include "instructions/Instruction.h"
#include "instructions/Branch.h"
#include "instructions/Label.h"
#include "instructions/Jump.h"

#include "booleans/Basic.h"

#include "statements/Statement.h"
#include "statements/While.h"

namespace cpq { namespace statements {

While::While(
	const cpq::location& loc,
	const std::shared_ptr<booleans::Boolean> boolean,
	const std::shared_ptr<Statement> statement
	)
	: Statement(loc)
	, m_boolean(std::make_shared<booleans::Negate>(loc, boolean))
	, m_statement(statement)
{}

void While::generate(instructions::InstructionList& insts) const
{
	auto result(std::make_shared<Symbol>(symbol_type_e::BOOLEAN));

	auto condition(std::make_shared<instructions::Label>());
	auto code(std::make_shared<instructions::Label>());

	insts.push_back(std::make_unique<instructions::Jump>(condition));
	code->set(insts.size());

	m_statement->generate(insts);
	condition->set(insts.size());

	m_boolean->generate(insts, result);
	insts.push_back(std::make_unique<instructions::Branch>(
		result->var(),
		code
		));
}

}}
