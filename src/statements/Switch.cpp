#include <memory>
#include <string>
#include <set>

#include "location.hh"
#include "parser.hh"

#include "symbol.h"

#include "instructions/Instruction.h"
#include "instructions/Inequality.h"
#include "instructions/Branch.h"
#include "instructions/Label.h"
#include "instructions/Jump.h"

#include "expressions/Expression.h"
#include "statements/Statement.h"
#include "statements/Switch.h"

namespace cpq { namespace statements {

Case::Case(const cpq::location& loc,
	const std::shared_ptr<expressions::Constant> constant,
	const std::shared_ptr<Statement> statement
)
	: m_loc(loc)
	, m_constant(constant)
	, m_statement(statement)
{}

void Switch::generate(instructions::InstructionList& insts) const
{
	auto expr(std::make_shared<Symbol>(m_expression->type()));
	auto comp(std::make_shared<Symbol>(symbol_type_e::INT));

	auto end(std::make_shared<instructions::Label>());

	m_expression->generate(insts, expr);

	for (const auto& c : m_cases) {
		auto next(std::make_shared<instructions::Label>());

		insts.push_back(std::make_unique<instructions::Inequality>(
				comparison_e::EQ,
				expr->type(),
				comp->var(),
				expr->var(),
				c->m_constant->str()
			));
		insts.push_back(std::make_unique<instructions::Branch>(
				comp->var(),
				next
			));
		c->m_statement->generate(insts);
		insts.push_back(std::make_unique<instructions::Jump>(
				end
			));
		next->set(insts.size());
	}

	m_default->generate(insts);
	end->set(insts.size());
}

void Switch::set_expression(const std::shared_ptr<expressions::Expression> expr) {
	m_expression = expr;
	
	if (expr->type() == symbol_type_e::INT) {
		for (const auto& c : m_cases) {
			if (c->m_constant->type() == symbol_type_e::REAL) {
				throw cpq::parser::syntax_error(c->m_loc, "case has invalid type");
			}
		}
	}
}

void Switch::add_case(std::unique_ptr<Case> c) {
	auto result = m_options.insert(std::stof(c->m_constant->str()));
	if (!result.second) {
		throw cpq::parser::syntax_error(c->m_loc, "case already defined");
	}
	
	m_cases.push_back(std::move(c));	
}

void Switch::set_default_case(std::shared_ptr<Statement> stmt) {
	m_default = stmt;
}

}}
