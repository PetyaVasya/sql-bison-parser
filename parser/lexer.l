%{ /* -*- C++ -*- */
#include <cerrno>
#include <climits>
#include <cstdlib>
#include <cstring> // strerror
#include <string>
#include "driver.h"
#include "parser.hpp"

# define YY_DECL \
yy::parser::symbol_type yylex (DBDriver& drv)
YY_DECL;

%}

%option noyywrap nounput noinput batch

%{
  // A number symbol corresponding to the value in S.
  yy::parser::symbol_type
  make_NUMBER (const std::string &s, const yy::parser::location_type& loc);
%}

id            [a-zA-Z_][a-zA-Z_0-9]*
int           [0-9]+
blank         [ \t\r]
quoted_string '[^']*'

%{
  // Code run each time a pattern is matched.
  # define YY_USER_ACTION  loc.columns (yyleng);
%}
%%
%{
  // A handy shortcut to the location held by the driver.
  yy::location& loc = drv.location;
  // Code run each time yylex is called.
  loc.step ();
%}
{blank}+   loc.step();//return yy::parser::make_BLANK(loc);
\n+        loc.lines (yyleng); loc.step ();

"="         return yy::parser::make_EQUAL  (loc);
"!="        return yy::parser::make_NOT_EQUAL   (loc);
"<"         return yy::parser::make_LESS   (loc);
">"         return yy::parser::make_GREATER  (loc);
"("         return yy::parser::make_LPAREN (loc);
")"         return yy::parser::make_RPAREN (loc);
"["         return yy::parser::make_LSBRACKET (loc);
"]"         return yy::parser::make_RSBRACKET (loc);
","         return yy::parser::make_COMMA (loc);
"."         return yy::parser::make_DOT(loc);
"'"         return yy::parser::make_SQUOTE (loc);
"\""        return yy::parser::make_DQUOTE (loc);
"SELECT"    return yy::parser::make_SELECT(loc);
"FROM"      return yy::parser::make_FROM(loc);
"WHERE"     return yy::parser::make_WHERE(loc);
"JOIN"      return yy::parser::make_JOIN(loc);
"AND"       return yy::parser::make_AND(loc);
"OR"        return yy::parser::make_OR(loc);
"ON"        return yy::parser::make_ON(loc);
"AS"        return yy::parser::make_AS(loc);
"ARRAY"     return yy::parser::make_ARRAY(loc);

{int}            return make_NUMBER (yytext, loc);
{id}             return yy::parser::make_IDENTIFIER (yytext, loc);
{quoted_string}  return yy::parser::make_TEXT(yytext, loc);
.          {
             throw yy::parser::syntax_error
               (loc, "invalid character: " + std::string(yytext));
}
<<EOF>>    return yy::parser::make_YYEOF (loc);
%%

yy::parser::symbol_type
make_NUMBER (const std::string &s, const yy::parser::location_type& loc)
{
  errno = 0;
  long n = strtol (s.c_str(), NULL, 10);
  if (! (INT_MIN <= n && n <= INT_MAX && errno != ERANGE))
    throw yy::parser::syntax_error (loc, "integer is out of range: " + s);
  return yy::parser::make_NUMBER ((int) n, loc);
}


void DBDriver::scan_begin(const std::string &file)
{
  if (file.empty () || file == "-")
    yyin = stdin;
  else if (!(yyin = fopen (file.c_str (), "r")))
    {
      std::cerr << "cannot open " << file << ": " << strerror (errno) << '\n';
      exit (EXIT_FAILURE);
    }
}

void DBDriver::scan_end()
{
  fclose (yyin);
}
