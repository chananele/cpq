#include <initializer_list>
#include <memory>

#include "location.hh"

#include "instructions/Instruction.h"
#include "booleans/Boolean.h"

#include "statements/Statement.h"
#include "statements/StatementBlock.h"
#include "statements/While.h"
#include "statements/For.h"

namespace cpq { namespace statements {

For::For(
	const cpq::location& loc,
	const std::shared_ptr<Statement> assignment,
	const std::shared_ptr<booleans::Boolean> condition,
	const std::shared_ptr<Statement> step,
	const std::shared_ptr<Statement> statement
	)
: Statement(loc)
, m_assignment(assignment)
, m_loop(std::make_shared<While>(
	loc,
	condition,
	std::make_shared<StatementBlock>(
		loc,
		std::make_shared<StatementList>(
			std::initializer_list<std::shared_ptr<Statement>>({statement, step}))
		)
	))
{}

void For::generate(std::vector<std::unique_ptr<instructions::Instruction>>& insts) const
{
	m_assignment->generate(insts);
	m_loop->generate(insts);
}

}}
