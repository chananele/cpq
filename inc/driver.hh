#pragma once

# include <string>
# include <unordered_map>

#include "symbol.h"

#include "instructions/Instruction.h"
#include "parser.hh"

#define YY_DECL cpq::parser::symbol_type yylex(cpq::driver& driver)

namespace cpq {

class driver
{
public:
	friend class parser;

	driver() {}
	virtual ~driver() {}

	// Handling the scanner.
	void scan_begin();
	void scan_end();

	/**
	 * @brief Runs the parser on the given file, Printing the compiled
	 * 		  code to the standard output.
	 *
	 * @param file - The name of the file to parse.
	 *
	 * @return 0 on success.
	 */
	int parse(const std::string& file);

	// Error handling.
	void error(const cpq::location& l, const std::string& m);

private:
	std::string 												m_file;
	std::unordered_map<std::string, std::shared_ptr<Symbol>>	m_variables;
	std::vector<std::unique_ptr<instructions::Instruction>>		m_instructions;
};

}

YY_DECL;
