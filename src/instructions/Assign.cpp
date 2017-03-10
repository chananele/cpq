#include <string>

#include "symbol.h"
#include "instructions/Assign.h"

namespace cpq { namespace instructions {

static const std::string instruction[] = {
	/* [symbol_type_e::INT]  = */ "IASN",
	/* [symbol_type_e::READ] = */ "RASN"
};

Assign::Assign(symbol_type_e type, const std::string& var, const std::string& val)
	: m_type(type)
	, m_var(var)
	, m_val(val)
{}

std::string Assign::generate() const 
{
	return instruction[m_type] + " " + m_var + " " + m_val;
}

}}
