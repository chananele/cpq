#include <cstdio>
#include <iostream>

#include "driver.hh"

int main(int argc, char *argv[])
{
	if (3 != argc) {
		std::cerr << "Usage: " << argv[0] << " <file> [<out>|-]" << std::endl;
		return 1;
	}
	
	std::string target(argv[2]);
	if (target != "-") {
		if (!freopen(target.c_str(), "w", stdout)) {
			std::cerr << "Failed to open target file: " << target << std::endl; 
		}
	}

	cpq::driver driver;
	return driver.parse(argv[1]);
}
