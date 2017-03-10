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
	#include <iostream>
	#include <string>
	#include <memory>
	
	#include "symbol.hh"
	#include "statement.hh"
	
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

%type <symbol_type_e> TYPE
%type <std::shared_ptr<Constant>> NUMBER

%type <std::shared_ptr<StatementList>> CONSTS
%type <std::shared_ptr<std::vector<std::string>>> IDENTIFIERS

%type <std::shared_ptr<Expression>> EXPRESSION
%type <std::shared_ptr<Expression>> FACTOR
%type <std::shared_ptr<Expression>> TERM

%type <std::shared_ptr<Assignment>> ASSIGNMENT_STMT

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
	IDENTIFIERS ":" TYPE ";" {
		for (const std::string& id : *$1) {
			if (driver.variables.find(id) != driver.variables.end()) {
				throw cpq::parser::syntax_error(@$, "variable redefinition: " + id);
			}
			driver.variables[id] = std::make_shared<Symbol>(id, $3);
		}
	}
;

IDENTIFIERS :
	"identifier"					{
		$$ = std::make_shared<std::vector<std::string>>(); 
		$$->push_back($1);
	}
|	IDENTIFIERS "," "identifier"	{
		$$ = $1;
		$$->push_back($3);
	}
;

CONSTS :
	%empty												{
		$$ = std::make_shared<StatementList>();
	}
|	CONSTS "const" TYPE "identifier" ":=" NUMBER ";"	{
		$$ = $1;
		if (driver.variables.find($4) != driver.variables.end()) {
			throw cpq::parser::syntax_error(@$, "variable redefinition: " + $4);
		}
		auto symbol = std::make_shared<Symbol>($4, $3);
		driver.variables[$4] = symbol;
		
		$$->push_back(std::make_unique<Assignment>(@$, symbol, $6));
		symbol->lock();
	}
;

TYPE :
	"INT"	{ $$ = cpq::symbol_type_e::INT; }
|	"REAL"	{ $$ = cpq::symbol_type_e::REAL; }
;

STATEMENTS :
	%empty					{}
|	STATEMENTS STATEMENT	{}
;

STATEMENT :
	ASSIGNMENT_STMT	{
		std::vector<std::unique_ptr<Instruction>> instructions;
		$1->generate(instructions);
		for (const auto& inst: instructions) {
			inst->generate(std::cout);
		}
	}
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
	"identifier" ":=" EXPRESSION ";" { 
		try
		{
			$$ = std::make_shared<Assignment>(@$, driver.variables[$1], $3);
		} catch (const std::out_of_range&) {
			throw cpq::parser::syntax_error(@$, "assigning to undefined variable: " + $1);
		} 
	}

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
	EXPRESSION "addition" TERM	{ 
		switch ($2) {
			case '+': $$ = std::make_shared<Addition>(@$, $1, $3);		break;
			case '-': $$ = std::make_shared<Subtraction>(@$, $1, $3);	break;
			default:
				throw cpq::parser::syntax_error(@$, "invalid operation");
		}
		 
	}
|	TERM						{ $$ = $1; }
;

TERM :
	TERM "multiplication" FACTOR	{
		switch ($2) {
			case '*': $$ = std::make_shared<Multiplication>(@$, $1, $3);	break;
			case '/': $$ = std::make_shared<Division>(@$, $1, $3);			break;
			default:
				throw cpq::parser::syntax_error(@$, "invalid operation");
		}
	}
|	FACTOR							{ $$ = $1; }
;
	 
FACTOR :
	"(" EXPRESSION ")"			{ $$ = $2; }
|	"identifier"				{ $$ = std::make_shared<Load>(@$, driver.variables[$1]); }
|	NUMBER						{ $$ = $1; }
;

NUMBER:
	"integer" 	{ $$ = std::make_shared<Int>(@$, $1);  }
|	"real"		{ $$ = std::make_shared<Real>(@$, $1); }


%%

void cpq::parser::error(const location_type& l, const std::string& m)
{
	driver.error(l, m);
}
