#include <string>

#include "location.hh"
#include "parser.hh"

#include "symbol.hh"
#include "instruction.hh"
#include "statement.hh"

namespace cpq {

namespace statements {
	
Assignment::Assignment(
	const cpq::location& loc,
	const std::shared_ptr<Symbol> symbol,
	const std::shared_ptr<Expression> expression)
	: Statement(loc)
	, m_symbol(symbol)
	, m_expression(expression)
{
	if (symbol_type_e::INT == symbol->type() && symbol_type_e::REAL == expression->type()) {
		std::string err = "type mismatch: ";
		err += "variable declared of type int but assigned value of type real";
		throw cpq::parser::syntax_error(m_loc, err);
	}
	
	if (symbol->constant()) {
		throw cpq::parser::syntax_error(m_loc, "attempting to modify constant variable");
	}
}

void Assignment::generate(
		std::vector<std::unique_ptr<instructions::Instruction>>& insts
		) const
{
	auto dest = m_symbol;
	if (m_expression->type() != m_symbol->type()) {
		auto temp = std::make_shared<Symbol>(m_symbol->type());

		m_expression->generate(
			insts,
			temp
			);
		insts.push_back(std::make_unique<instructions::Cast>(
			m_symbol->type(),
			m_symbol->var(),
			temp->var()
			));
	}
	else {
		m_expression->generate(
			insts,
			m_symbol
			);
	}
}

void Load::generate(
		std::vector<std::unique_ptr<instructions::Instruction>>& insts,
		const std::shared_ptr<Symbol> symbol
		) const
{
	insts.push_back(std::make_unique<instructions::Assign>(
		symbol->type(),
		symbol->var(),
		m_symbol->var()
		));
}

void Constant::generate(
		std::vector<std::unique_ptr<instructions::Instruction>>& insts,
		const std::shared_ptr<Symbol> symbol
		) const
{
	insts.push_back(std::make_unique<instructions::Assign>(
		symbol->type(),
		symbol->var(),
		str()
		));
}

void Read::generate(
		std::vector<std::unique_ptr<instructions::Instruction>>& insts
		) const
{
	insts.push_back(std::make_unique<instructions::Read>(
		m_symbol->type(),
		m_symbol->var()
		));
}

void Write::generate(
		std::vector<std::unique_ptr<instructions::Instruction>>& insts
		) const
{
	auto temp(std::make_shared<Symbol>(m_expression->type()));
	m_expression->generate(insts, temp);

	insts.push_back(std::make_unique<instructions::Write>(
		temp->type(),
		temp->var()
		));
}

void Cast::generate(
		std::vector<std::unique_ptr<instructions::Instruction>>& insts
		) const
{
	if (m_symbol->type() == symbol_type_e::REAL) {
		auto temp(std::make_shared<Symbol>(symbol_type_e::INT));
		m_expression->generate(insts, m_symbol);
		insts.push_back(std::make_unique<instructions::Cast>(
			symbol_type_e::INT,
			temp->var(),
			m_symbol->var()
			));
		insts.push_back(std::make_unique<instructions::Cast>(
			symbol_type_e::REAL,
			m_symbol->var(),
			temp->var()
			));
			
	} else {
		auto temp(std::make_shared<Symbol>(symbol_type_e::REAL));
		m_expression->generate(insts, temp);
		insts.push_back(std::make_unique<instructions::Cast>(
			symbol_type_e::INT,
			m_symbol->var(),
			temp->var()
			));
	}
}

void StatementBlock::generate(
		std::vector<std::unique_ptr<instructions::Instruction>>& insts
		) const
{
	for (const auto& stmt : *m_stmts) {
		stmt->generate(insts);
	}
}

std::shared_ptr<Expression> binary(
	const cpq::location& loc,
	char operation,
	const std::shared_ptr<Expression> left,
	const std::shared_ptr<Expression> right
	)
{
	switch (operation)
	{
	case '+': return std::make_shared<Add>(loc, left, right);
	case '-': return std::make_shared<Subtract>(loc, left, right);
	case '*': return std::make_shared<Multiply>(loc, left, right);
	case '/': return std::make_shared<Divide>(loc, left, right);
	default:
		throw cpq::parser::syntax_error(loc, "invalid operation");
	}
}

void Switch::generate(std::vector<std::unique_ptr<instructions::Instruction>>& insts) const {
	auto expr(std::make_shared<Symbol>(m_expression->type()));
	auto comp(std::make_shared<Symbol>(symbol_type_e::INT));

	auto end(std::make_shared<instructions::Label>());

	m_expression->generate(insts, expr);

	for (const auto& c : m_cases) {
		auto next(std::make_shared<instructions::Label>());

		insts.push_back(std::make_unique<instructions::Inequality>(
				comparison_e::EQ,
				expr->type(),
				comp->var(),
				expr->var(),
				c->m_constant->str()
			));
		insts.push_back(std::make_unique<instructions::Branch>(
				comp->var(),
				next
			));
		c->m_statement->generate(insts);
		insts.push_back(std::make_unique<instructions::Jump>(
				end
			));
		next->set(insts.size());
	}

	m_default->generate(insts);
	end->set(insts.size());
}

void Switch::set_expression(const std::shared_ptr<Expression> expr) {
	m_expression = expr;
	
	if (expr->type() == symbol_type_e::INT) {
		for (const auto& c : m_cases) {
			if (c->m_constant->type() == symbol_type_e::REAL) {
				throw cpq::parser::syntax_error(c->m_loc, "case has invalid type");
			}
		}
	}
}


void Switch::add_case(std::unique_ptr<Case> c) {
	auto result = m_options.insert(std::stof(c->m_constant->str()));
	if (!result.second) {
		throw cpq::parser::syntax_error(c->m_loc, "case already defined");
	}
	
	m_cases.push_back(std::move(c));	
}

void Switch::set_default_case(std::shared_ptr<Statement> stmt) {
	m_default = stmt;
}

void finish(std::vector<std::unique_ptr<instructions::Instruction>>& insts)
{
	insts.push_back(std::make_unique<instructions::Halt>());
}
	
}

}
