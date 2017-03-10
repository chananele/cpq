#include <stdexcept>
#include <string>

#include "symbol.hh"

namespace cpq {

std::string to_string(symbol_type_e type)
{
	switch (type) {
	case symbol_type_e::INT:
		return "int";
	case symbol_type_e::REAL:
		return "real";
	default:
		throw std::runtime_error("invalid type");
	}
}

static std::string variable() {
	static unsigned i = 0;
	return std::string("var_") + std::to_string(i++);
}

Symbol::Symbol(symbol_type_e type)
	: m_constant(false)
	, m_type(type)
	, m_var(variable())
{}
	
}