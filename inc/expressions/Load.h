#pragma once

#include <memory>

#include "location.hh"

#include "symbol.h"

#include "expressions/Expression.h"

namespace cpq { namespace expressions {
	
/**
 * @brief Represents the CPL expression of evaluating a variable.
 */
class Load : public Expression
{
public:

	/**
	 * @brief Create a load expression.
	 *
	 * @param loc - The location of this expression in the source code.
	 * @param symbol - The variable who's value is being evaluated.
	 */
	Load(const cpq::location& loc, const std::shared_ptr<Symbol> symbol);

	virtual symbol_type_e type() const override;

	virtual void generate(
			instructions::InstructionList& insts,
			const std::shared_ptr<Symbol> symbol
			) const override;

private:
	const std::shared_ptr<Symbol> m_symbol;
};
	
}}
