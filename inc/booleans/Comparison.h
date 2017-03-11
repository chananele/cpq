#pragma once

#include <memory>

#include "location.hh"

#include "symbol.h"

#include "instructions/Instruction.h"
#include "expressions/Expression.h"
#include "booleans/Boolean.h"

namespace cpq { namespace booleans {

/**
 * @brief Represents a CPL expression comparison.
 */
class Comparison : public Boolean {
public:

	/**
	 * @brief Create a comparison.
	 *
	 * @param loc - The location of this comparison expression in source code.
	 * @param comp - The type of comparison to perform.
	 * @param left - The left operand of the comparison.
	 * @param right - The right operand of the comparison.
	 */
	Comparison(
		const cpq::location& loc,
		const comparison_e comp,
		const std::shared_ptr<expressions::Expression> left,
		const std::shared_ptr<expressions::Expression> right
	);

	virtual void generate(
		instructions::InstructionList& insts,
		const std::shared_ptr<Symbol> symbol
	) const override;

private:

	/**
	 * @brief Returns whether the comparison will be of integers or reals.
	 */
	symbol_type_e type() const;

	const comparison_e								m_comp;
	const std::shared_ptr<expressions::Expression> 	m_left;
	const std::shared_ptr<expressions::Expression> 	m_right;
};

}}
