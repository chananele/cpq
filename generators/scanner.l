%{

#include <iostream>
#include <cerrno>
#include <climits>
#include <cstdlib>
#include <string>

#include "driver.hh"
#include "parser.hh"

// Work around an incompatibility in flex (at least versions
// 2.5.31 through 2.5.33): it generates code that does
// not conform to C89.  See Debian bug 333231
// <http://bugs.debian.org/cgi-bin/bugreport.cgi?bug=333231>.
#undef yywrap
#define yywrap() 1

// The location of the current token.
static cpq::location loc;

%}

%option noyywrap nounput batch debug noinput
%x comment

id		[[:alpha:]][[:alnum:]]*
int		[[:digit:]]+
real	{int}"."[[:digit:]]*
blank	[ \t]

%{
	// Code run each time a pattern is matched.
	#define YY_USER_ACTION  loc.columns(yyleng);
%}

%%

%{
	// Code run each time yylex is called.
	loc.step ();
%}

<*>{blank}+   loc.step();
<*>[\n]+      loc.lines(yyleng); loc.step();

"/*"			{ BEGIN(comment); }
<comment>.		{}
<comment>"*/"	{ BEGIN(0); }

"instructions"		return cpq::parser::make_INSTRUCTIONS(loc);

"start"				return cpq::parser::make_START(loc);
"end"				return cpq::parser::make_END(loc);

"variables"			return cpq::parser::make_VARIABLES(loc);
	
"const"				return cpq::parser::make_CONST(loc);
"for"				return cpq::parser::make_FOR(loc);

"switch"			return cpq::parser::make_SWITCH(loc);
"while"				return cpq::parser::make_WHILE(loc);
"then"				return cpq::parser::make_THEN(loc);
"do"				return cpq::parser::make_DO(loc);
"until"				return cpq::parser::make_UNTIL(loc);
"break"				return cpq::parser::make_BREAK(loc);
	
"case"				return cpq::parser::make_CASE(loc);
"default"			return cpq::parser::make_DEFAULT(loc);
	
"int"				return cpq::parser::make_INT_TYPE(loc);
"real"				return cpq::parser::make_REAL_TYPE(loc);
	
"not"				return cpq::parser::make_NOT(loc);
"&&"				return cpq::parser::make_AND(loc);
"||"				return cpq::parser::make_OR(loc);
	
"rval"				return cpq::parser::make_RVAL(loc);
"ival"				return cpq::parser::make_IVAL(loc);
	
"if"				return cpq::parser::make_IF(loc);
"else"				return cpq::parser::make_ELSE(loc);
	
"print"				return cpq::parser::make_PRINT(loc);
"read"				return cpq::parser::make_READ(loc);

":="	return cpq::parser::make_ASSIGNMENT(loc);

"<"		return cpq::parser::make_COMPARISON(cpq::comparison_e::LT,  loc);
">"		return cpq::parser::make_COMPARISON(cpq::comparison_e::GT,  loc);
"<="	return cpq::parser::make_COMPARISON(cpq::comparison_e::LTE, loc);
">="	return cpq::parser::make_COMPARISON(cpq::comparison_e::GTE, loc);
"=="	return cpq::parser::make_COMPARISON(cpq::comparison_e::EQ,  loc);
"!="	return cpq::parser::make_COMPARISON(cpq::comparison_e::NEQ, loc);

"+"	|
"-" return cpq::parser::make_ADDITION(yytext[0], loc);

"*" |
"/" return cpq::parser::make_MULTIPLICATION(yytext[0], loc);

{int} {
	int n = std::stoi(yytext);
	return cpq::parser::make_INTEGER(n, loc);
}

{real} {
	float n = std::stof(yytext);
	return cpq::parser::make_REAL(n, loc);
}

"(" return cpq::parser::make_OPEN_PAREN (loc);
")" return cpq::parser::make_CLOSE_PAREN(loc);
"{" return cpq::parser::make_OPEN_BRACE (loc);
"}" return cpq::parser::make_CLOSE_BRACE(loc);
":" return cpq::parser::make_COLON		(loc);
";" return cpq::parser::make_SEMI_COLON (loc);
"," return cpq::parser::make_COMMA		(loc);

{id}       return cpq::parser::make_IDENTIFIER(yytext, loc);
.          driver.error(loc, "invalid character");
<<EOF>>    return cpq::parser::make_DONE(loc);
%%

namespace cpq {

void driver::scan_begin()
{
	yy_flex_debug = trace_scanning;
	if (file.empty() || file == "-") {
    	yyin = stdin;
	}
	else if (!(yyin = fopen(file.c_str(), "r"))) {
		std::cerr << "cannot open " << file << ": " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
    }
}

void driver::scan_end()
{
	fclose(yyin);
}

}
