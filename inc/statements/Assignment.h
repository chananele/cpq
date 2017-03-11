#pragma once

#include <memory>

#include "location.hh"

#include "symbol.h"

#include "instructions/Instruction.h"
#include "expressions/Expression.h"
#include "statements/Statement.h"

namespace cpq { namespace statements {
	
/**
 * @brief Represents a CPL assignment statement.
 */
class Assignment : public Statement {
public:

	/**
	 * @brief Create an assignment statement.
	 *
	 * @param loc - The location in source of this statement.
	 * @param symbol - Symbol representing the variable to be assigned.
	 * @param expresssion - The expression who's value should be assigned to the given variable.
	 */
	Assignment(
		const cpq::location& loc,
		const std::shared_ptr<Symbol> symbol,
		const std::shared_ptr<expressions::Expression> expression
		);

	virtual void generate(instructions::InstructionList& insts) const override;

private:
	const std::shared_ptr<Symbol>					m_symbol;
	const std::shared_ptr<expressions::Expression>	m_expression;
};

	
}}
