%skeleton "lalr1.cc" // -*- C++ -*-
%require "3.8"
%header

%define api.token.raw

%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires {
    #include <string>
    struct DBDriver;
    #include "driver.h"
    #include "parser.hpp"
}

// The parsing context.
%param { DBDriver& drv }

%locations

%define parse.trace
%define parse.error detailed
%define parse.lac full

%code {
    #include "db/result.h"
    #include "driver.h"

    # define YY_DECL \
    yy::parser::symbol_type yylex (DBDriver& drv)
    YY_DECL;

    void yy::parser::error(const location_type& l, const std::string& m)
    {
    std::cerr << l << ": " << m << '\n';
    }
}

%define api.token.prefix {TOK_}
%token
  EQUAL     "="
  NOT_EQUAL "!="
  LESS      "<"
  GREATER   ">"
  LPAREN    "("
  RPAREN    ")"
  COMMA     ","
  SQUOTE    "'"
  DQUOTE    "\""
  DOT       "."
  LSBRACKET "["
  RSBRACKET "]"
;
%token SELECT
%token FROM
%token WHERE
%token JOIN
%token AND
%token OR
%token ON
%token AS
%token ARRAY
// %ntoken <std::string> full_identifier "full_identifier"
%token <std::string> IDENTIFIER "identifier"
%token <std::string> TEXT "text"
%token <int> NUMBER "number"
%nterm <int> exp

%printer { yyo << $$; } <*>;

%%
%start comand;
comand: query;

query: select;

select: SELECT qualified_values mb_from mb_join mb_where;

mb_from: %empty | FROM from ;
from: mb_qualified_table_value | from COMMA mb_qualified_table_value;

mb_where: %empty | WHERE predicate;

mb_join: %empty | mb_join JOIN mb_qualified_table_value mb_on;
mb_on: %empty | ON predicate;

predicate: value predicate_op value
    | predicate OR predicate
    | predicate AND predicate;

predicate_op: EQUAL | NOT_EQUAL | LESS | GREATER;

value: full_identifier | TEXT | NUMBER | array | function;
table_value: full_identifier | function;

full_identifier: IDENTIFIER | full_identifier DOT IDENTIFIER;

array: ARRAY LSBRACKET args RSBRACKET;
function: full_identifier LPAREN args RPAREN;
args: value | args COMMA value;

mb_identifier: %empty | AS IDENTIFIER;
mb_qualified_value: value mb_identifier;
mb_qualified_table_value: table_value mb_identifier;

qualified_values: mb_qualified_value | qualified_values COMMA mb_qualified_value;




// %start unit;
// unit: assignments exp  {  };

// assignments:
//   %empty                 {}
// | assignments assignment {};

// assignment:
//   IDENTIFIER ASSIGN exp {  };

// %left PLUS MINUS;
// %left STAR SLASH;
// exp:
//   NUMBER
// | IDENTIFIER  { $$ = 0; }
// | exp BLANK PLUS BLANK exp   { $$ = $1 + $5; }
// | exp BLANK MINUS BLANK exp   { $$ = $1 - $5; }
// | exp BLANK STAR BLANK exp   { $$ = $1 * $5; }
// | exp BLANK SLASH BLANK exp   { $$ = $1 / $5; }
// | "(" exp ")"   { $$ = $2; }
%%