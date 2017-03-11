#include "symbol.h"

#include "booleans/Boolean.h"

namespace cpq { namespace booleans {

symbol_type_e Boolean::type() const 
{
	return symbol_type_e::BOOLEAN;
}
	
}}
