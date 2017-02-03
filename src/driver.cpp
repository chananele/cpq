#include "driver.hh"
#include "parser.hh"

namespace cpq {

driver::driver ()
	: trace_scanning (false)
	, trace_parsing (false)
{}

driver::~driver () {}

int driver::parse(const std::string& path)
{
	file = path;
	scan_begin ();
	cpq::parser parser(*this);
	parser.set_debug_level(trace_parsing);
	int res = parser.parse();
	scan_end();
	return res;
}

void driver::error(const cpq::location& l, const std::string& m)
{
	std::cerr << l << ": " << m << std::endl;
}

void driver::error (const std::string& m)
{
	std::cerr << m << std::endl;
}

}
