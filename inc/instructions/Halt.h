#pragma once

#include "instructions/Instruction.h"

namespace cpq { namespace instructions {

/**
 * @brief Represent the QUAD end of program instruction.
 */
class Halt : public Instruction {
public:
	virtual std::string generate() const override;
};

}}
