#pragma once

#include "instructions/Instruction.h"
#include "statements/Statement.h"

namespace cpq { namespace statements {
	
/**
 * @brief Empty statement used to continue error handling.
 * @note This will never actually be used to generate code.
 */
class NullStatement : public Statement {
public:
	using Statement::Statement;

	virtual void generate(instructions::InstructionList&) const override {}
};

	
}}
