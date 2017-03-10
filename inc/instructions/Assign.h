#pragma once

#include <string>

#include "symbol.h"
#include "instructions/Instruction.h"

namespace cpq { namespace instructions {

/**
 * @brief Represents a QUAD instruction to assign a value to a variable.
 */
class Assign : public Instruction {
public:

	/**
	 * @brief Creates an assignment instruction.
	 *
	 * @param type - The type of the target variable.
	 * @param var  - The variable to which to assign the given value.
	 * @param val  - The value to assign the said variable.
	 *				 (May be either a CONSTANT or VARIABLE.) 
	 */
	Assign(symbol_type_e type, const std::string& var, const std::string& val);

	virtual std::string generate() const override;

private:
	const symbol_type_e m_type;
	const std::string	m_var;
	const std::string	m_val;
};

}}
