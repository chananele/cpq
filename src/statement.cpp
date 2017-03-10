#include "location.hh"
#include "parser.hh"

#include "symbol.hh"
#include "statement.hh"

namespace cpq {
	
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
	
}