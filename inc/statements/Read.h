#pragma once

#include <memory>

#include "location.hh"

#include "symbol.h"

#include "instructions/Instruction.h"
#include "statements/Statement.h"

namespace cpq { namespace statements {
	
/**
 * @brief Represents a CPL read input statement.
 */
class Read : public Statement
{
public:

	/**
	 * @brief Create a read statement.
	 *
	 * @param loc - The location of this statement in source code.
	 * @param symbol - The symbol to receive the inputted value.
	 */
	Read(const cpq::location& loc, const std::shared_ptr<Symbol> symbol);

	virtual void generate(instructions::InstructionList& insts) const override;

private:
	const std::shared_ptr<Symbol> m_symbol;
};
	
}}
