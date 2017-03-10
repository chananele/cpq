#include <string>

#include "symbol.h"
#include "instructions/Inequality.h"

namespace cpq { namespace instructions {

static const std::string instruction[symbol_type_e::LAST_TYPE][comparison_e::LAST_COMPARISON] = {
	/* [symbol_type_e::INT] */
	{
		/* [comparison_e::GT]	= */ "IGRT",
		/* [comparison_e::LT]	= */ "ILSS",
		/* [comparison_e::EQ]	= */ "IEQL",
		/* [comparison_e::NEQ]	= */ "INQL",
	},
	/* [symbol_type_e::REAL] */
	{
		/* [comparison_e::GT]	= */ "RGRT",
		/* [comparison_e::LT]	= */ "RLSS",
		/* [comparison_e::EQ]	= */ "REQL",
		/* [comparison_e::NEQ]	= */ "RNQL",
	},
};

Inequality::Inequality(
	comparison_e	comp,
	symbol_type_e 	type,
	const std::string& dest,
	const std::string& left,
	const std::string& right
)
	: m_comp(comp)
	, m_type(type)
	, m_dest(dest)
	, m_left(left)
	, m_right(right)
{}

std::string Inequality::generate() const
{
	bool change = m_comp & 0b100; // see documentation of comparison_type_e.
	auto left 	= change ? m_left : m_right;
	auto right	= change ? m_right : m_left;

	return instruction[m_type][m_comp & 0b11] + " " + m_dest + " " + left + " " + right;
}

}}
