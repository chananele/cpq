#pragma once

#include <memory>
#include <string>

#include "location.hh"

#include "symbol.h"

#include "instructions/Instruction.h"
#include "instructions/Inequality.h"
#include "instructions/Branch.h"

#include "booleans/Boolean.h"

namespace cpq { namespace booleans {

/**
 * @brief Represent a basic binary logical connective.
 * @param res - Describes when the evaluation should short circuit
 *		(true for the OR operator, false for the AND operator).
 */
template <bool res>
class Connective : public Boolean {
public:

	/**
	 * @brief Create a connective of type corresponding to res.
	 *
	 * @param loc - The location of this boolean expression in the source code.
	 * @param left - The left operand of this expression.
	 * @param right - The right operand of this expression.
	 */
	Connective(
		const cpq::location& loc,
		const std::shared_ptr<Boolean> left,
		const std::shared_ptr<Boolean> right
	)
		: Boolean(loc)
		, m_left(left)
		, m_right(right)
	{}

	virtual void generate(
		instructions::InstructionList& insts,
		const std::shared_ptr<Symbol> symbol
	) const override
	{
		auto label(std::make_shared<instructions::Label>());
		m_left->generate(insts, symbol);
		auto comp(std::make_shared<Symbol>(symbol_type_e::BOOLEAN));
		
		// This instruction will short circuit depending on res.
		insts.push_back(std::make_unique<instructions::Inequality>(
			comparison_e::NEQ,
			symbol_type_e::INT,
			comp->var(),
			symbol->var(),
			std::to_string(int(res))
		));
		
		insts.push_back(std::make_unique<instructions::Branch>(comp->var(), label));
		m_right->generate(insts, symbol);
		label->set(insts.size());
	}

private:
	const std::shared_ptr<Boolean> m_left;
	const std::shared_ptr<Boolean> m_right;
};

using Or =  Connective<true>;
using And = Connective<false>;

/**
 * @brief Represents the boolean negation operation.
 */
class Negate : public Boolean {
public:

	/**
	 * @brief Create a boolean negaction expression.
	 *
	 * @param loc - The location of this expression in source code.
	 * @param boolean - The expression to negate.
	 */
	Negate(const cpq::location& loc, const std::shared_ptr<Boolean> boolean);

	virtual void generate(
		std::vector<std::unique_ptr<instructions::Instruction>>& insts,
		const std::shared_ptr<Symbol> symbol
	) const override;

private:
	const std::shared_ptr<Boolean> m_boolean;
};

}}
