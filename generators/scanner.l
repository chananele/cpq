%{

# include <cerrno>
# include <climits>
# include <cstdlib>
# include <string>

# include "driver.hh"
# include "parser.hh"

// Work around an incompatibility in flex (at least versions
// 2.5.31 through 2.5.33): it generates code that does
// not conform to C89.  See Debian bug 333231
// <http://bugs.debian.org/cgi-bin/bugreport.cgi?bug=333231>.
# undef yywrap
# define yywrap() 1

// The location of the current token.
static cpq::location loc;

%}

%option noyywrap nounput batch debug noinput

id		[[:alpha:]][[:alnum:]]*
int		[[:digit:]]+
real	{int}.[[:digit:]]*
blank	[ \t]

%{
	// Code run each time a pattern is matched.
	# define YY_USER_ACTION  loc.columns(yyleng);
%}

%%

%{
	// Code run each time yylex is called.
	loc.step ();
%}

{blank}+   loc.step();
[\n]+      loc.lines(yyleng); loc.step();

":="     return cpq::parser::make_ASSIGN(loc);

{int} {
	int n = std::stoi(yytext);
	return cpq::parser::make_INTEGER(n, loc);
}

{real} {
	float n = std::stof(yytext);
	return cpq::parser::make_REAL(n, loc);
}

{id}       return cpq::parser::make_IDENTIFIER(yytext, loc);
.          driver.error(loc, "invalid character");
<<EOF>>    return cpq::parser::make_END(loc);
%%

namespace cpq {

void driver::scan_begin()
{
	yy_flex_debug = trace_scanning;
	if (file.empty() || file == "-") {
    	yyin = stdin;
	}
	else if (!(yyin = fopen(file.c_str(), "r"))) {
		error("cannot open " + file + ": " + strerror(errno));
		exit(EXIT_FAILURE);
    }
}

void driver::scan_end()
{
	fclose(yyin);
}

}
