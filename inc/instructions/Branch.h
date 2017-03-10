#pragma once

#include <memory>
#include <string>

#include "instructions/Instruction.h"
#include "instructions/Label.h"

namespace cpq { namespace instructions {

/**
 * @brief Represent a QUAD conditional branch instruction.
 */
class Branch : public Instruction {
public:

	/**
	 * @brief Creates a branch instruction.
	 *
	 * @param var   - If this variable is zero, branch.
	 * @param label - Target of the branch.
	 */
	Branch(const std::string& var, const std::shared_ptr<Label> label);

	virtual std::string generate() const override;

private:
	const std::string m_var;
	const std::shared_ptr<Label> m_label;
};

}}
