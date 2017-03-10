#include <iostream>

#include "driver.hh"

int main(int argc, char *argv[])
{
	if (2 != argc) {
		std::cerr << "Usage: " << argv[0] << " <file>" << std::endl;
		return 1;
	}

	cpq::driver driver;
	return driver.parse(argv[1]);
}
