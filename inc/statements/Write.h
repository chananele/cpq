#pragma once

#include <memory>

#include "location.hh"

#include "instructions/Instruction.h"
#include "expressions/Expression.h"
#include "statements/Statement.h"

namespace cpq { namespace statements {
	
/**
 * @brief Represents a CPL print output statement.
 */
class Write : public Statement
{
public:

	/**
	 * @brief Create a write statement.
	 *
	 * @param loc - The location of this statement in source code.
	 * @param expression - The expression who's value should be written to output.
	 */
	Write(const cpq::location& loc, const std::shared_ptr<expressions::Expression> expression);

	virtual void generate(instructions::InstructionList& insts) const override;

private:
	const std::shared_ptr<expressions::Expression> m_expression;
};
	
}}
