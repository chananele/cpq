%skeleton "lalr1.cc"
%require "3.0.4"
%defines

%define api.namespace { cpq }
%define parser_class_name { parser }

%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires
{
	#include <string>
	#include <memory>
	
	#include "numbers.hh"
	
	namespace cpq {
		class driver;
	}
}

// The parsing context.
%param { cpq::driver& driver }

%locations
%initial-action
{
	// Initialize the initial location.
	@$.begin.filename = @$.end.filename = &driver.file;
};

%define parse.trace
%define parse.error verbose

%code
{
	#include "driver.hh"
}

%token
	END  0  "end of file"
	ASSIGN  ":="
;

%type <std::shared_ptr<cpq::INumber>> number

%token <std::string> IDENTIFIER "identifier"
%token <int> 	INTEGER "integer"
%token <float>	REAL	"real"

%printer { yyoutput << $$; } <*>;

%%
%start unit;
unit: assignments		 {}

assignments:
	%empty                 {}
|	assignments assignment {};

assignment:
	"identifier" ":=" number { driver.variables[$1] = std::move($3); };
|	error { yyerrok; }

number:
	"integer" 	{ std::shared_ptr<INumber> result(std::make_shared<Int>($1)); $$ = result;  }
|	"real"		{ std::shared_ptr<INumber> result(std::make_shared<Real>($1)); $$ = result; }


%%

void cpq::parser::error(const location_type& l, const std::string& m)
{
	driver.error(l, m);
}
