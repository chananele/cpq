#include <string>

#include "symbol.h"
#include "instructions/Read.h"

namespace cpq { namespace instructions {

static const std::string instruction[] = {
	/* [symbol_type_e::INT]  = */ "IINP",
	/* [symbol_type_e::READ] = */ "RINP"
};

Read::Read(symbol_type_e type, const std::string& var)
	: m_type(type)
	, m_var(var)
{}

std::string Read::generate() const 
{
	return instruction[m_type] + " " + m_var;
}

}}
