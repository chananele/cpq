#include "instructions/Halt.h"

namespace cpq { namespace instructions {

std::string Halt::generate() const
{
	return "HALT";
}

}}
