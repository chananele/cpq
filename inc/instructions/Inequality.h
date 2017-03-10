#pragma once

#include <string>

#include "symbol.h"
#include "instructions/Instruction.h"

namespace cpq { namespace instructions {

/**
 * @brief Represent a QUAD inequality instruction.
 */
class Inequality : public Instruction {
public:

	/**
	 * @brief Create an inequality instruction.
	 *
	 * @param comp  - The type of comparison to be performed.
	 * @param type  - The type of the operands of the comparison.
	 * @param dest  - The target variable to hold the result of the comparison.
	 * @param left  - The left operand of the inequality.
	 * @param right - The right operand of the inequality.
	 */
	Inequality(
		comparison_e	comp,
		symbol_type_e 	type,
		const std::string& dest,
		const std::string& left,
		const std::string& right
	);

	virtual std::string generate() const override;

private:
	const comparison_e	m_comp;
	const symbol_type_e m_type;
	const std::string	m_dest;
	const std::string	m_left;
	const std::string	m_right;
};

}}
