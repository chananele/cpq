#pragma once

#include <memory>

#include "location.hh"

#include "symbol.h"

#include "instructions/Instruction.h"
#include "expressions/Expression.h"

namespace cpq { namespace booleans {
	
/**
 * @brief Abstract class from which all CPL boolean expressions must inherit.
 */
class Boolean : public expressions::Expression {
public:
	using expressions::Expression::Expression;

	virtual symbol_type_e type() const override;

	virtual void generate(
		instructions::InstructionList& insts,
		const std::shared_ptr<Symbol> symbol
	) const = 0;
};
	
}}
