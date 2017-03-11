#include <memory>

#include "location.hh"
#include "parser.hh"

#include "expressions/Expression.h"
#include "expressions/Math.h"

namespace cpq { namespace expressions {
	
std::shared_ptr<Expression> binary(
	const cpq::location& loc,
	char operation,
	const std::shared_ptr<Expression> left,
	const std::shared_ptr<Expression> right
	)
{
	switch (operation)
	{
	case '+': return std::make_shared<Add>(loc, left, right);
	case '-': return std::make_shared<Subtract>(loc, left, right);
	case '*': return std::make_shared<Multiply>(loc, left, right);
	case '/': return std::make_shared<Divide>(loc, left, right);
	default:
		throw cpq::parser::syntax_error(loc, "invalid operation");
	}
}
	
}}
