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
		
		enum comparison_e {
			GT,
			LT,
			GTE,
			LTE,
			EQ,
			NEQ
		};
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
	DONE  0  "end of file"
	
	OPEN_PAREN 	"("
	CLOSE_PAREN	")"
	
	OPEN_BRACE	"{"
	CLOSE_BRACE	"}"
	
	COLON		":"
	SEMI_COLON	";"
	COMMA		","
	
	INSTRUCTIONS "instructions"
	START	"start"
	END		"end"
	
	VARIABLES "variables"
	
	CONST "const"
	
	FOR		"for"
	SWITCH	"switch"
	WHILE	"while"
	THEN	"then"
	DO		"do"
	UNTIL	"until"
	BREAK	"break"
	
	CASE	"case"
	DEFAULT	"default"
	
	INT_TYPE	"INT"
	REAL_TYPE	"REAL"
	
	NOT	"not"
	AND	"and"
	OR	"or"
	
	ASSIGNMENT  ":="
	
	RVAL	"rval"
	IVAL	"ival"
	
	IF		"if"
	ELSE	"else"
	
	PRINT	"print"
	READ	"read"
;

%type <std::shared_ptr<cpq::INumber>> NUMBER

%token <std::string> IDENTIFIER "identifier"

%token <char> ADDITION "addition"
%token <char> MULTIPLICATION "multiplication"

%token <cpq::comparison_e> COMPARISON "comparison"

%token <int> 	INTEGER "integer"
%token <float>	REAL	"real"

%printer { yyoutput << $$; } <*>;

%%
%start PROGRAM;

PROGRAM : 
	"instructions" "identifier" DECLARATIONS "start" STATEMENTS "end" {} 
;

DECLARATIONS : 
	%empty					{}
|	"variables" VARS CONSTS	{}
;

VARS : 
	%empty					{}
|	VARS DECLARATION		{}
;

DECLARATION :
	IDENTIFIERS ":" TYPE ";" {}
;

IDENTIFIERS :
	"identifier"					{}
|	IDENTIFIERS "," "identifier"	{}
;

CONSTS :
	%empty												{}
|	CONSTS "const" TYPE "identifier" ":=" NUMBER ";"	{}
;

TYPE :
	"INT"	{}
|	"REAL"	{}
;

STATEMENTS :
	%empty					{}
|	STATEMENTS STATEMENT	{}
;

STATEMENT :
	ASSIGNMENT_STMT	{}
|	CAST_STMT		{}
|	BLOCK_STMT		{}
|	CONTROL_STMT	{}
|	READ_STMT		{}
|	WRITE_STMT		{}
;

WRITE_STMT :
	"print" "(" EXPRESSION ")" ";" {}
;

READ_STMT :
	"read" "(" "identifier" ")" ";" {}
;

ASSIGNMENT_STMT :
	"identifier" ":=" EXPRESSION ";" {}

CAST_STMT :
	"identifier" ":=" "ival" "(" EXPRESSION ")" ";" {}
|	"identifier" ":=" "rval" "(" EXPRESSION ")" ";" {}
;

CONTROL_STMT :
	"if" 	"(" BOOLEAN ")" "then" STATEMENT "else" STATEMENT		{}
|	"while" "(" BOOLEAN ")"  STATEMENT								{}
|	"for"	"(" ASSIGNMENT_STMT BOOLEAN ";" STEP ")" STATEMENT		{}
|	"do" STATEMENT "until" "(" BOOLEAN ")" ";"						{}
|	SWITCH_STMT														{}
;

BLOCK_STMT :
	"{" STATEMENTS "}" {}
;

SWITCH_STMT :
	"switch" "(" EXPRESSION ")" "{" CASES "default" ":" STATEMENTS "}" {}
;

CASES :
	%empty											{}
|	CASES "case" NUMBER ":" STATEMENTS "break" ";"	{}
;
	  
STEP :
	"identifier" ":=" "identifier" "addition" NUMBER		{}
|	"identifier" ":=" "identifier" "multiplication" NUMBER	{} 
;

BOOLEAN :
	BEXPR	{}
;

BEXPR :
	BEXPR "or" BTERM	{}
|	BTERM				{}
;

BTERM :
	BTERM "and" BFACTOR {}
|	BFACTOR				{}
;

BFACTOR :
	"not" "(" BEXPR ")"					{}
|	EXPRESSION "comparison" EXPRESSION	{}
;

EXPRESSION :
	EXPRESSION "addition" TERM	{}
|	TERM						{}
;

TERM :
	TERM "multiplication" FACTOR	{}
|	FACTOR							{}
;
	 
FACTOR :
	"(" EXPRESSION ")"			{}
|	"identifier"				{}
|	NUMBER						{}
;

NUMBER:
	"integer" 	{ std::shared_ptr<INumber> result(std::make_shared<Int>($1)); $$ = result;  }
|	"real"		{ std::shared_ptr<INumber> result(std::make_shared<Real>($1)); $$ = result; }


%%

void cpq::parser::error(const location_type& l, const std::string& m)
{
	driver.error(l, m);
}
