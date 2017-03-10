#pragma once

# include <string>
# include <unordered_map>

#include "symbol.hh"
#include "parser.hh"

# define YY_DECL cpq::parser::symbol_type yylex(cpq::driver& driver)

namespace cpq {

class driver
{
public:
	driver ();
	virtual ~driver ();

	std::unordered_map<std::string, std::shared_ptr<Symbol>> variables;

	// Handling the scanner.
	void scan_begin ();
	void scan_end ();
	bool trace_scanning;

	// Run the parser on file F.
	// Return 0 on success.
	int parse (const std::string& f);

	// The name of the file being parsed.
	// Used later to pass the file name to the location tracker.
	std::string file;

	// Whether parser traces should be generated.
	bool trace_parsing;

	// Error handling.
	void error (const cpq::location& l, const std::string& m);
};

}

YY_DECL;
