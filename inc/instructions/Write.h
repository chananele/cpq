#pragma once

#include <string>

#include "symbol.h"
#include "instructions/Instruction.h"

namespace cpq { namespace instructions {

/**
 * @brief Represents a QUAD instruction to print output.
 */
class Write : public Instruction {
public:

	/**
	 * @brief Creates a WRITE instruction.
	 *
	 * @param type - The type of the variable who's contents should be printed.
	 * @param var  - The name of the QUAD variable to print.
	 */
	Write(symbol_type_e type, const std::string& var);
	
	virtual std::string generate() const override;

private:
	const symbol_type_e m_type;
	const std::string 	m_var;
};

}}
