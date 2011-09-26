/* $Id: parser.yy 48 2009-09-05 08:07:10Z tb $ -*- mode: c++ -*- */
/** \file parser.yy Contains the example Bison parser source */

%{ /*** C/C++ Declarations ***/

#include <stdio.h>
#include <string>
#include <vector>

#include "syntaxtree.h"

struct labels{
	int	for_jmpfalse;
	int	for_jmp;
};

%}

/*** yacc/bison Declarations ***/

/* Require bison 2.3 or later */
%require "2.3"

/* add debug output code to generated parser. disable this for release
 * versions. */
%debug

/* start symbol is named "start" */
%start program

/* write out a header file containing the token defines */
%defines

/* use newer C++ skeleton file */
%skeleton "lalr1.cc"

/* namespace to enclose parser in */
%name-prefix="example"

/* set the parser's class identifier */
%define "parser_class_name" "Parser"

/* keep track of the current position within the input */
%locations
%initial-action
{
    // initialize the initial location object
    @$.begin.filename = @$.end.filename = &driver.streamname;
};

/* The driver is passed by reference to the parser and to the scanner. This
 * provides a simple but effective pure interface, not relying on global
 * variables. */
%parse-param { class Driver& driver }

/* verbose error messages */
%error-verbose

 /*** BEGIN EXAMPLE - Change the example grammar's tokens below ***/

%union {
    int  			integerVal;
    double 			doubleVal;
    std::string*		stringVal;
    struct labels*		lbls;
    class SyntaxTreeNode* node;
    class BlockList* blocks;
    class StatementList* statements;
}

%token			END	     0	"end of file"
%token			EOL		"end of line"
%token <integerVal> 	INTEGER		"integer"
%token <doubleVal> 	DOUBLE		"double"
%token <stringVal> 	STRING		"string"
%token <stringVal> 	IDENTIFIER		"identifier"
%token T_BEGIN T_END SEMICOLON ASSIGNOP
%token <lbls> IF
%token  THEN  ELSE
%token  WHILE DO
%token READ WRITE DEF CALL LPAREN RPAREN

%type <node> program blok functiondef statement assignment expression iteration
%type <blocks> blocks 
%type <statements> statements stmtblock

%left '-' '+'
%left '*' '/'
%left LPAREN RPAREN
%nonassoc REDUCE
%nonassoc ELSE

%destructor { delete $$; } IDENTIFIER STRING  

 /*** END EXAMPLE - Change the example grammar's tokens above ***/

%{

#include "driver.h"
#include "scanner.h"

#include "codeobject.h"

/* this "connects" the bison parser in the driver to the flex scanner class
 * object. it defines the yylex() function call to pull the next token from the
 * current lexer object of the driver context. */
#undef yylex
#define yylex driver.lexer->lex


%}

%% /*** Grammar Rules ***/

 /*** BEGIN EXAMPLE - Change the example grammar rules below ***/

program
	: blocks { $$ = new MainNode($1); driver.context.addSyntaxtTreeRoot($$); }

blocks
	: blocks blok { $$ = $1; $$->addBlock($2); }
	| /* empty */ {$$ = new BlockList(); }

blok
	: functiondef stmtblock { $$ = new FunctionNode($1, $2); }
	
functiondef
	: DEF IDENTIFIER { $$ = new FunctionDefinition(*$2, "type"); }

statements
	: statements statement  { $$ = $1; $$->addStatement($2); }
	| /* empty */ {$$ = new StatementList(); }

stmtblock
	: T_BEGIN statements T_END {$$ = $2;}

statement
	: stmtblock  {$$ = $1; }
	| assignment {$$ = $1; }
	| iteration  {$$ = $1; }
	| READ IDENTIFIER SEMICOLON {}
	| WRITE IDENTIFIER SEMICOLON {  }
	| ifstatement
	| CALL IDENTIFIER  SEMICOLON {  }
	| DEF IDENTIFIER  SEMICOLON { $$ = new VariableDefinition(*$2, "type"); std::cout << "def" << std::endl; }
	
ifstatement
	: IF expression THEN statement %prec REDUCE
	| IF expression THEN statement ELSE statement

iteration
	: WHILE expression statement  { $$ = new WhileNode($2, $3); }
	| DO statement WHILE expression SEMICOLON { $$ = new DoWhileNode($2, $4); }

assignment
	: IDENTIFIER ASSIGNOP expression SEMICOLON { $$ = new AssignmentNode(*$1, $3);  std::cout << "assignment" << std::endl; }

expression
	: IDENTIFIER { $$ = new IDexpression(*$1); std::cout << *$1 << std::endl; }
	| INTEGER {   driver.context.addInstruction(LOAD, $1); }                           // TODO
	| expression '+' expression { $$ = new AdditionNode($1, $3); std::cout << "plus" << std::endl; }
	| expression '-' expression { $$ = new SubtractionNode($1, $3); std::cout << "minus" << std::endl; }
	| expression '*' expression { $$ = new MultiplicationNode($1, $3);  }
	| expression '/' expression { $$ = new DivisionNode($1, $3); }
	| LPAREN expression RPAREN  { $$ = $2; }


 /*** END EXAMPLE - Change the example grammar rules above ***/

%% /*** Additional Code ***/

void example::Parser::error(const Parser::location_type& l,
			    const std::string& m)
{
    driver.error(l, m);
}
