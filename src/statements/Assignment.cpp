#include <memory>

#include "location.hh"
#include "parser.hh"

#include "symbol.h"

#include "instructions/Instruction.h"
#include "instructions/Cast.h"

#include "expressions/Expression.h"
#include "statements/Assignment.h"

namespace cpq { namespace statements {
	
Assignment::Assignment(
	const cpq::location& loc,
	const std::shared_ptr<Symbol> symbol,
	const std::shared_ptr<expressions::Expression> expression)
	: Statement(loc)
	, m_symbol(symbol)
	, m_expression(expression)
{
	if (symbol_type_e::INT == symbol->type() && symbol_type_e::REAL == expression->type()) {
		std::string err = "type mismatch: ";
		err += "variable declared of type int but assigned value of type real";
		throw cpq::parser::syntax_error(m_loc, err);
	}
	
	if (symbol->constant()) {
		throw cpq::parser::syntax_error(m_loc, "attempting to modify constant variable");
	}
}

void Assignment::generate(instructions::InstructionList& insts) const
{
	auto dest = m_symbol;
	if (m_expression->type() != m_symbol->type()) // implicit cast
	{
		auto temp = std::make_shared<Symbol>(m_symbol->type());

		m_expression->generate(
			insts,
			temp
			);
		insts.push_back(std::make_unique<instructions::Cast>(
			m_symbol->type(),
			m_symbol->var(),
			temp->var()
			));
	}
	else 
	{
		m_expression->generate(
			insts,
			m_symbol
			);
	}
}
	
}}
