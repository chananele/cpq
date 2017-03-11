#pragma once

#include <memory>

#include "location.hh"

#include "symbol.h"

#include "instructions/Instruction.h"
#include "expressions/Expression.h"
#include "statements/Statement.h"

namespace cpq { namespace statements {
	
/**
 * @brief Represents an explicit CPL cast.
 */
class Cast : public Statement {
public:

	/**
	 * @brief Create a cast statement.
	 *
	 * @param loc - The location of this statement in source code.
	 * @param symbol - The symbol to receive the cast value of the given expression.
	 * @param expression - The expression who's value to cast.
	 */
	Cast(
		const cpq::location& loc,
		const std::shared_ptr<Symbol> symbol,
		const std::shared_ptr<expressions::Expression> expression
	);

	virtual void generate(instructions::InstructionList& insts) const override;

private:
	const std::shared_ptr<Symbol> m_symbol;
	const std::shared_ptr<expressions::Expression> m_expression;
};
	
}}
