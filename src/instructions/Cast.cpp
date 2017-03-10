#include <string>

#include "symbol.h"
#include "instructions/Cast.h"

namespace cpq { namespace instructions {

static const std::string instruction[] = {
	/* [symbol_type_e::INT]  = */ "RTOI",
	/* [symbol_type_e::READ] = */ "ITOR"
};

Cast::Cast(symbol_type_e type, const std::string& var, const std::string val)
	: m_type(type)
	, m_var(var)
	, m_val(val)
{}

std::string Cast::generate() const
{
	return instruction[m_type] + " " + m_var + " " + m_val;
}

}}
