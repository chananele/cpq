#pragma once

#include <string>

#include "symbol.h"
#include "instructions/Instruction.h"

namespace cpq { namespace instructions {

/**
 * @brief Represent a QUAD instruction to read input.
 */
class Read : public Instruction {
public:

	/**
	 * @brief Creates a READ instruction.
	 *
	 * @param type - The type of the variable into which the read will be performed.
	 * @param var  - The name of the said QUAD variable.
	 */
	Read(symbol_type_e type, const std::string& var);
	
	virtual std::string generate() const override;

private:
	const symbol_type_e m_type;
	const std::string 	m_var;
};

}}
