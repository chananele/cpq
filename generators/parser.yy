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
	
	#include "symbol.h"
	
	#include "statements/Statement.h"
	#include "statements/Assignment.h"
	#include "statements/Cast.h"
	#include "statements/For.h"
	#include "statements/If.h"
	#include "statements/Read.h"
	#include "statements/Write.h"
	#include "statements/StatementBlock.h"
	#include "statements/Switch.h"
	#include "statements/Until.h"
	#include "statements/While.h"
	
	#include "expressions/Expression.h"
	#include "expressions/Constant.h"
	#include "expressions/Load.h"
	#include "expressions/Math.h"
	
	#include "booleans/Boolean.h"
	#include "booleans/Basic.h"
	#include "booleans/Comparison.h"
	
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
	@$.begin.filename = @$.end.filename = &driver.m_file;
};

%define parse.trace
%define parse.error verbose

%code
{
	#include "driver.hh"
	
	using namespace cpq::expressions;
	using namespace cpq::statements;
	using namespace cpq::booleans;
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
%type <std::shared_ptr<expressions::Constant>> NUMBER

%type <std::shared_ptr<statements::StatementList>> DECLARATIONS
%type <std::shared_ptr<statements::StatementList>> STATEMENTS
%type <std::shared_ptr<statements::StatementList>> CONSTS

%type <std::shared_ptr<std::vector<std::string>>> IDENTIFIERS

%type <std::shared_ptr<expressions::Expression>> EXPRESSION
%type <std::shared_ptr<expressions::Expression>> FACTOR
%type <std::shared_ptr<expressions::Expression>> TERM

%type <std::shared_ptr<statements::Statement>> STATEMENT

%type <std::shared_ptr<statements::Statement>> _ASSIGNMENT_STMT
%type <std::shared_ptr<statements::Statement>> ASSIGNMENT_STMT

%type <std::shared_ptr<statements::Statement>> BLOCK_STMT
%type <std::shared_ptr<statements::Statement>> WRITE_STMT


%type <std::shared_ptr<statements::Statement>> _READ_STMT
%type <std::shared_ptr<statements::Statement>> READ_STMT

%type <std::shared_ptr<statements::Statement>> CAST_STMT
%type <std::shared_ptr<statements::Statement>> CONTROL_STMT
%type <std::shared_ptr<statements::Statement>> SWITCH_STMT

%type <std::shared_ptr<statements::Switch>>		CASES
%type <std::shared_ptr<statements::Statement>>	STEP

%type <std::shared_ptr<booleans::Boolean>> BOOLEAN
%type <std::shared_ptr<booleans::Boolean>> BFACTOR
%type <std::shared_ptr<booleans::Boolean>> BEXPR
%type <std::shared_ptr<booleans::Boolean>> BTERM

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
	"instructions" "identifier" DECLARATIONS "start" STATEMENTS "end" {
		
		if (driver.m_error)
			YYABORT;
		
		auto& insts = driver.m_instructions;
		
		for (const auto& assn : *$3) {
			assn->generate(insts);
		}
		for (const auto& stmt : *$5) {
			stmt->generate(insts);
		}
		
		finish(insts);
	} 
;

DECLARATIONS : 
	%empty					{ $$ = std::make_shared<StatementList>(); }
|	"variables" VARS CONSTS	{ $$ = $3; }
;

VARS : 
	%empty					{}
|	VARS DECLARATION		{}
;

DECLARATION :
	IDENTIFIERS ":" TYPE ";" {
		for (const std::string& id : *$1) {
			if (driver.m_variables.find(id) != driver.m_variables.end()) {
				error(@$, "variable redefinition: " + id);
			} else {
				driver.m_variables[id] = std::make_shared<Symbol>($3);
			}
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
		if (driver.m_variables.find($4) != driver.m_variables.end()) {
			error(@$, "variable redefinition: " + $4);
		} else {
			auto symbol = std::make_shared<Symbol>($3);
			driver.m_variables[$4] = symbol;
			
			$$->push_back(std::make_unique<Assignment>(@$, symbol, $6));
			symbol->lock();
		}
	}
;

TYPE :
	"INT"	{ $$ = cpq::symbol_type_e::INT; }
|	"REAL"	{ $$ = cpq::symbol_type_e::REAL; }
;

STATEMENTS :
	%empty					{ $$ = std::make_shared<StatementList>(); }
|	STATEMENTS STATEMENT	{ $$ = $1; $$->push_back($2); }
;

STATEMENT :
	ASSIGNMENT_STMT	{ $$ = $1; }
|	CAST_STMT		{ $$ = $1; }
|	BLOCK_STMT		{ $$ = $1; }
|	CONTROL_STMT	{ $$ = $1; }
|	READ_STMT		{ $$ = $1; }
|	WRITE_STMT		{ $$ = $1; }
;

WRITE_STMT :
	"print" "(" EXPRESSION ")" ";" { $$ = std::make_shared<Write>(@$, $3); }
|	"print" "(" error	   ")" ";" {}
;

_READ_STMT :
	"identifier" {
		try
		{
			$$ = std::make_shared<Read>(@$, driver.m_variables.at($1));
		} catch (const std::out_of_range&) {
			throw cpq::parser::syntax_error(@$, "reading to undefined variable: " + $1);
		} 
	}
;

READ_STMT :
	"read" "(" _READ_STMT ")" ";" { $$ = $3; }
|	"read" "(" error	  ")" ";" {}
;

_ASSIGNMENT_STMT :
	"identifier" ":=" EXPRESSION { 
		try
		{
			$$ = std::make_shared<Assignment>(@$, driver.m_variables.at($1), $3);
		} catch (const std::out_of_range&) {
			throw cpq::parser::syntax_error(@$, "assigning to undefined variable: " + $1);
		} 
	}
;

ASSIGNMENT_STMT :
	_ASSIGNMENT_STMT 	";" { $$ = $1; }
|	error 				";" {}

CAST_STMT :
	"identifier" ":=" "ival" "(" EXPRESSION ")" ";" { 
		try
		{
			auto symbol = driver.m_variables[$1];
			if ($5->type() == symbol_type_e::INT) {
				$$ = std::make_shared<Assignment>(@$, symbol, $5);
			} else {
				$$ = std::make_shared<Cast>(@$, symbol, $5);
			}
		} catch (const std::out_of_range&) {
			throw cpq::parser::syntax_error(@$, "assigning to undefined variable: " + $1);
		}
	}
|	"identifier" ":=" "rval" "(" EXPRESSION ")" ";" {
		try
		{
			auto symbol = driver.m_variables[$1];
			$$ = std::make_shared<Assignment>(@$, symbol, $5);
		} catch (const std::out_of_range&) {
			throw cpq::parser::syntax_error(@$, "assigning to undefined variable: " + $1);
		}
	}
;

CONTROL_STMT :
	"if" 	"(" BOOLEAN ")" "then" STATEMENT "else" STATEMENT		{
		$$ = std::make_shared<If>(@$, $3, $6, $8);
	}
|	"while" "(" BOOLEAN ")"  STATEMENT								{
		$$ = std::make_shared<While>(@$, $3, $5);
	}
|	"for"	"(" ASSIGNMENT_STMT BOOLEAN ";" STEP ")" STATEMENT		{
		$$ = std::make_shared<For>(@$, $3, $4, $6, $8);
	}
|	"do" STATEMENT "until" "(" BOOLEAN ")" ";"						{
		$$ = std::make_shared<Until>(@$, $5, $2);
	}
|	SWITCH_STMT														{
		$$ = $1;
	}
;

BLOCK_STMT :
	"{" STATEMENTS "}" {
		$$ = std::make_shared<StatementBlock>(@$, $2);
	}
;

SWITCH_STMT :
	"switch" "(" EXPRESSION ")" "{" CASES "default" ":" STATEMENTS "}" {
		$6->set_default_case(std::make_shared<StatementBlock>(@9, $9));
		$6->set_expression($3);
		$$ = $6;
	}
;

CASES :
	%empty											{
		$$ = std::make_shared<Switch>(@$); 
	}
|	CASES "case" NUMBER ":" STATEMENTS "break" ";"	{
		$$ = $1;
		$$->add_case(std::make_unique<Case>(@3, $3, std::make_shared<StatementBlock>(
			@5, $5
		)));
	}
|	CASES "case" error "break" ";" {}
;
	  
STEP :
	"identifier" ":=" "identifier" "addition" NUMBER		{
		if (driver.m_variables.find($1) == driver.m_variables.end()) {
			throw cpq::parser::syntax_error(@1, "assigning to undefined variable: " + $1);
		}
		if (driver.m_variables.find($3) == driver.m_variables.end()) {
			throw cpq::parser::syntax_error(@3, "loading undefined variable: " + $3);
		}
		$$ = std::make_shared<Assignment>(@$, driver.m_variables[$1], binary(
				@$, $4, std::make_shared<Load>(@3, driver.m_variables[$3]), $5
			)
		);
	}
|	"identifier" ":=" "identifier" "multiplication" NUMBER	{
		if (driver.m_variables.find($1) == driver.m_variables.end()) {
			throw cpq::parser::syntax_error(@1, "assigning to undefined variable: " + $1);
		}
		if (driver.m_variables.find($3) == driver.m_variables.end()) {
			throw cpq::parser::syntax_error(@3, "loading undefined variable: " + $3);
		}
		$$ = std::make_shared<Assignment>(@$, driver.m_variables[$1], binary(
				@$, $4, std::make_shared<Load>(@3, driver.m_variables[$3]), $5
			)
		);
	} 
|	error {}
;

BOOLEAN :
	BEXPR	{ $$ = $1; }
;

BEXPR :
	BEXPR "or" BTERM	{ $$ = std::make_shared<Or>(@$, $1, $3); }
|	BTERM				{ $$ = $1; }
;

BTERM :
	BTERM "and" BFACTOR { $$ = std::make_shared<And>(@$, $1, $3); }
|	BFACTOR				{ $$ = $1; }
;

BFACTOR :
	"not" "(" BEXPR ")"					{ $$ = std::make_shared<Negate>(@$, $3); 				}
|	EXPRESSION "comparison" EXPRESSION	{ $$ = std::make_shared<Comparison>(@$, $2, $1, $3); 	}
;

EXPRESSION :
	EXPRESSION "addition" TERM	{ 
		$$ = binary(@$, $2, $1, $3);
		 
	}
|	TERM						{ $$ = $1; }
;

TERM :
	TERM "multiplication" FACTOR	{
		$$ = binary(@$, $2, $1, $3);
	}
|	FACTOR							{ $$ = $1; }
;
	 
FACTOR :
	"(" EXPRESSION ")"			{ $$ = $2; }
|	"identifier"				{
		try 
		{
			$$ = std::make_shared<Load>(@$, driver.m_variables.at($1));
		}
		catch (const std::out_of_range&) {
			throw cpq::parser::syntax_error(@$, "loading undefined variable: " + $1);
		}
	}
|	NUMBER						{ $$ = $1; }
;

NUMBER:
	"integer" 	{ $$ = std::make_shared<Int>(@$, $1);	}
|	"real"		{ $$ = std::make_shared<Real>(@$, $1);	}
;


%%

void cpq::parser::error(const location_type& l, const std::string& m)
{
	driver.error(l, m);
}
