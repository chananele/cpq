#pragma once

#include <string>

namespace cpq {

enum symbol_type_e {
	INT		= 0,
	REAL	= 1,

	LAST_TYPE
};

/**
 * The order here is important. It is assumed that the 3rd bit signals that operands
 * should be compared in reverse order, using the bottom 2 bits to decide the comparison.
 */
enum comparison_e {
	GT	= 0,
	LT	= 1,
	EQ	= 2,
	NEQ = 3,
	GTE	= 4,
	LTE = 5,

	LAST_COMPARISON
};

std::string to_string(symbol_type_e type);

class Symbol {
public:
	Symbol(const std::string& name, symbol_type_e type);

	const std::string& var() const 		{ return m_var;			}
	const symbol_type_e type() const 	{ return m_type;		}
	const bool constant() const			{ return m_constant; 	}
	std::string name() const 			{ return m_name;		}

	void lock()							{ m_constant = true;	}

private:
	bool		m_constant;
	const std::string 	m_var;
	const symbol_type_e m_type;
	const std::string	m_name;
};

}
