#include "location.hh"

#include "expressions/Expression.h"

namespace cpq { namespace expressions {

Expression::Expression(const cpq::location& loc)
	: m_loc(loc)
{}
	
}}
