#pragma once

#include <memory>
#include <set>

#include "location.hh"

#include "instructions/Instruction.h"

#include "expressions/Expression.h"
#include "expressions/Constant.h"

#include "statements/Statement.h"

namespace cpq { namespace statements {

/**
 * @brief Represents a case in a CPL switch statement.
 */
class Case
{
public:

	/**
	 * @brief Create a case.
	 *
	 * @param constant - The value the switch subject must equal to take this case.
	 * @param statement - The statement to execute if this case is chosen. 
	 */
	Case(const cpq::location& loc,
		const std::shared_ptr<expressions::Constant> constant,
		const std::shared_ptr<Statement> statement
		);

	const cpq::location& 							m_loc;
	const std::shared_ptr<expressions::Constant>	m_constant;
	const std::shared_ptr<Statement>				m_statement;
};

/**
 * @brief Represents a CPL switch statement.
 */
class Switch : public Statement {
public:
	using Statement::Statement;

	/**
	 * @brief Set the subject of the switch.
	 *
	 * @param expr - The expression to compare to each of the cases constant.
	 */
	void set_expression(const std::shared_ptr<expressions::Expression> expr);
	
	/**
	 * @brief Add a case to the switch statement.
	 *
	 * @param c - The case to add.
	 */
	void add_case(std::unique_ptr<Case> c);
	
	/**
	 * @brief Set the default statement of the switch.
	 *
	 * @param stmt - The statment to execute if no case was matched.
	 */
	void set_default_case(std::shared_ptr<Statement> stmt);

	virtual void generate(instructions::InstructionList& insts) const override;

private:
	std::set<float>								m_options;
	std::vector<std::unique_ptr<Case>>			m_cases;
	std::shared_ptr<Statement>					m_default;
	std::shared_ptr<expressions::Expression>	m_expression;
};

}}
