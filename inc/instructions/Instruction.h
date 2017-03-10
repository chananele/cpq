#pragma once

#include <string>

namespace cpq { namespace instructions {

/**
 * @brief Interface which all instructions must implement.
 */
class Instruction {
public:
	/**
	 * @brief Generate the QUAD string representation of the instruction.
	 */
	virtual std::string generate() const = 0;
};

}}
