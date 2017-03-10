#pragma once

#include <memory>
#include <string>

#include "instructions/Instruction.h"
#include "instructions/Label.h"

namespace cpq { namespace instructions {

/**
 * @brief Represents a QUAD unconditional branch instruction.
 */
class Jump : public Instruction {
public:

	/**
	 * @brief Creates a JUMP instruction.
	 *
	 * @param label - The target of the jump.
	 */
	Jump(const std::shared_ptr<Label> label);

	virtual std::string generate() const override;

private:
	const std::shared_ptr<Label> m_label;
};

}}
