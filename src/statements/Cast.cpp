#include <memory>

#include "location.hh"

#include "symbol.h"

#include "instructions/Instruction.h"
#include "instructions/Cast.h"

#include "expressions/Expression.h"
#include "statements/Cast.h"

namespace cpq { namespace statements {
	
Cast::Cast(
	const cpq::location& loc,
	const std::shared_ptr<Symbol> symbol,
	const std::shared_ptr<expressions::Expression> expression
)
	: Statement(loc)
	, m_symbol(symbol)
	, m_expression(expression)
{}

void Cast::generate(instructions::InstructionList& insts) const
{
	if (m_symbol->type() == symbol_type_e::REAL) {
		auto temp(std::make_shared<Symbol>(symbol_type_e::INT));
		m_expression->generate(insts, m_symbol);
		insts.push_back(std::make_unique<instructions::Cast>(
			symbol_type_e::INT,
			temp->var(),
			m_symbol->var()
			));
		insts.push_back(std::make_unique<instructions::Cast>(
			symbol_type_e::REAL,
			m_symbol->var(),
			temp->var()
			));
			
	} else {
		auto temp(std::make_shared<Symbol>(symbol_type_e::REAL));
		m_expression->generate(insts, temp);
		insts.push_back(std::make_unique<instructions::Cast>(
			symbol_type_e::INT,
			m_symbol->var(),
			temp->var()
			));
	}
}
	
}}
