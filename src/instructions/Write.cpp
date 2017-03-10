#include <string>

#include "symbol.h"
#include "instructions/Write.h"

namespace cpq { namespace instructions {

static const std::string instruction[] = {
	/* [symbol_type_e::INT]  = */ "IPRT",
	/* [symbol_type_e::READ] = */ "RPRT"
};

Write::Write(symbol_type_e type, const std::string& var)
	: m_type(type)
	, m_var(var)
{}

std::string Write::generate() const 
{
	return instruction[m_type] + " " + m_var;
}

}}
