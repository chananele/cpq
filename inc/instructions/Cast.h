#pragma once

#include <string>

#include "symbol.h"
#include "instructions/Instruction.h"

namespace cpq { namespace instructions {

/**
 * @brief Represents a QUAD instruction to cast a variable from INT to REAL and vice versa.
 */
class Cast : public Instruction {
public:

	/**
	 * @brief Creates a cast instruction.
	 *
	 * @param type - The type of the target variable.
	 * @param var  - The variable to which the result should be assigned.
	 * @param val  - The value to cast. (May be either a CONSTANT or VARIABLE.)
	 */
	Cast(symbol_type_e type, const std::string& var, const std::string val);

	virtual std::string generate() const override;

private:
	const symbol_type_e m_type;
	const std::string	m_var;
	const std::string	m_val;
};

}}
