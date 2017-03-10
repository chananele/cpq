#include "driver.hh"
#include "parser.hh"

namespace cpq {

int driver::parse(const std::string& path)
{
	m_file = path;
	scan_begin();
	
	cpq::parser parser(*this);
	int result = parser.parse();
	
	scan_end();

	if (result) return result;
	
	for (const auto& inst : m_instructions) {
		std::cout << inst->generate() << std::endl;
	}
	std::cout << "Copyright Chananel Engelberg 2017." << std::endl;
	
	return 0;
}

void driver::error(const cpq::location& l, const std::string& m)
{
	std::cerr << l << ": " << m << std::endl;
}

}
