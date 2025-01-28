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

// %define parse.trace
// %define parse.error detailed
// %define parse.lac full

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
%token <std::string> IDENTIFIER "identifier"
%token <std::string> TEXT "text"
%token <int> NUMBER "number"

%printer { yyo << $$; } <*>;

%%
%start comand;
comand: query { drv.result = std::move($1); };

%nterm <Queries> query;
query: select { $$ = std::move($1); };

%nterm <SelectQuery> select;
select: SELECT qualified_values mb_from mb_join mb_where {
    $$ = SelectQuery(
        std::move($2),
        std::move($3),
        std::move($5),
        std::move($4)
    );
};

%nterm <std::optional<FromClause>> mb_from;
mb_from: %empty | FROM from {
    $$ = std::move($2);
};
%nterm <FromClause> from;
from:
    mb_qualified_table_value {
        $$.sources.emplace_back(std::move($1));
    }
    | from COMMA mb_qualified_table_value {
        $$ = std::move($1);
        $$.sources.emplace_back(std::move($3));
    };

%nterm <std::optional<WhereClause>> mb_where;
mb_where: %empty | WHERE predicate { $$ = {std::move($2)}; };

%nterm <std::vector<JoinClause>> mb_join;
mb_join: %empty | mb_join JOIN mb_qualified_table_value mb_on {
    $$ = std::move($1);
    $$.emplace_back(
        std::move($3),
        std::move($4)
    );
};
%nterm <std::unique_ptr<Predicate>> mb_on;
mb_on: %empty | ON predicate { $$ = std::move($2); };

%nterm <std::unique_ptr<Predicate>> predicate;
predicate:
    value predicate_op value {
        $$ = std::make_unique<ValuePredicate>($2, std::move($1), std::move($3));
    }
    | predicate OR predicate { 
        $$ = std::make_unique<OrPredicate>(std::move($1), std::move($3));
    }
    | predicate AND predicate {
        $$ = std::make_unique<AndPredicate>(std::move($1), std::move($3));
    };

%nterm <ValuePredicate::Operator> predicate_op;
predicate_op:
    EQUAL { $$ = ValuePredicate::Operator::EQ; }
    | NOT_EQUAL { $$ = ValuePredicate::Operator::NEQ; }
    | LESS { $$ = ValuePredicate::Operator::LE; }
    | GREATER { $$ = ValuePredicate::Operator::GR; };

%nterm <Value> value;
value:
    full_identifier { $$ = Name{std::move($1)}; }
    | primitive { $$ = std::move($1); }
    | array { $$ = std::move($1); }
    | function { $$ = std::move($1); };
%nterm <Literal> primitive;
primitive: 
    TEXT { $$ = Text{std::move($1.substr(1, $1.size() - 2))}; }
    | NUMBER { $$ = Integer{std::move($1)}; }
%nterm <Source> table_value;
table_value:
    full_identifier { $$ = Table{$1}; }
    | function { $$ = std::move($1); };

%nterm <std::string> full_identifier;
full_identifier:
    IDENTIFIER { $$ = std::move($1); }
    | full_identifier DOT IDENTIFIER { $$ = std::move($1); $$ += "." + std::move($3); };

%nterm <ArrayCall> array;
array: ARRAY LSBRACKET args RSBRACKET { $$ = ArrayCall(std::move($3)); };
%nterm <FunctionCall> function;
function: full_identifier LPAREN args RPAREN { $$ = {std::move($1), std::move($3)}; };
%nterm <std::vector<Value>> args;
args:
    value { $$.emplace_back(std::move($1)); }
    | args COMMA value {
        $$ = std::move($1);
        $$.emplace_back(std::move($3));
    };

%nterm <std::optional<Name>> mb_identifier;
mb_identifier: %empty | AS IDENTIFIER { $$ = std::move($2); };
%nterm <QualifiedValue> mb_qualified_value;
mb_qualified_value: value mb_identifier { $$ = {std::move($1), std::move($2)}; };
%nterm <QualifiedSource> mb_qualified_table_value;
mb_qualified_table_value: table_value mb_identifier { $$ = {std::move($1), std::move($2)}; };

%nterm <std::vector<QualifiedValue>> qualified_values;
qualified_values:
    mb_qualified_value { $$.emplace_back(std::move($1)); }
    | qualified_values COMMA mb_qualified_value { 
        $$ = std::move($1);
        $$.emplace_back(std::move($3));
    };
%%