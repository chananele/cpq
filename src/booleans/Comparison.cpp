#include <memory>

#include "location.hh"

#include "symbol.h"

#include "instructions/Instruction.h"
#include "instructions/Inequality.h"
#include "instructions/Cast.h"

#include "expressions/Expression.h"
#include "booleans/Comparison.h"

namespace cpq { namespace booleans {

Comparison::Comparison(
	const cpq::location& loc,
	const comparison_e comp,
	const std::shared_ptr<expressions::Expression> left,
	const std::shared_ptr<expressions::Expression> right
)
	: Boolean(loc)
	, m_comp(comp)
	, m_left(left)
	, m_right(right)
{}

void Comparison::generate(
	instructions::InstructionList& insts,
	const std::shared_ptr<Symbol> symbol
) const
{
	auto first	(std::make_shared<Symbol>(m_left->type()));
	auto second	(std::make_shared<Symbol>(m_right->type()));

	m_left	->generate(insts, first);
	m_right	->generate(insts, second);

	if (first->type() == type()) { auto temp = first; first = second; second = temp; }

	auto converted(first);
	if (first->type() != type()) {
		converted = std::make_shared<Symbol>(type());
		insts.push_back(std::make_unique<instructions::Cast>(
			converted->type(),
			converted->var(),
			first->var()
			));
	}
	insts.push_back(std::make_unique<instructions::Inequality>(
		m_comp,
		type(),
		symbol->var(),
		converted->var(),
		second->var()
		));
}

symbol_type_e Comparison::type() const
{
	if (m_left->type() == symbol_type_e::REAL) {
		return symbol_type_e::REAL;
	}
	if (m_right->type() == symbol_type_e::REAL) {
		return symbol_type_e::REAL;
	}
	return symbol_type_e::INT;
}

}}
