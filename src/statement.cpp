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
		std::string err = "type mismatch: " + symbol->name() + " ";
		err += "declared of type int but assigned value of type real";
		throw cpq::parser::syntax_error(m_loc, err);
	}
	
	if (symbol->constant()) {
		throw cpq::parser::syntax_error(m_loc, "attempting to modify constant identifier");
	}
}

void Assignment::generate(
		std::vector<std::unique_ptr<instructions::Instruction>>& insts
		) const
{
	auto dest = m_symbol;
	if (m_expression->type() != m_symbol->type()) {
		auto temp = std::make_shared<Symbol>("", m_symbol->type());

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
	auto temp(std::make_shared<Symbol>("", m_expression->type()));
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
		auto temp(std::make_shared<Symbol>("", symbol_type_e::INT));
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
		auto temp(std::make_shared<Symbol>("", symbol_type_e::REAL));
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

void finish(std::vector<std::unique_ptr<instructions::Instruction>>& insts)
{
	insts.push_back(std::make_unique<instructions::Halt>());
}
	
}

}
