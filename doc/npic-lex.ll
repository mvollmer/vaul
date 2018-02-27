/* lexer for npic -*- c++ -*-

  Copyright (C) 1994-1996 University of Dortmund
  Department of Electrical Engineering, AG SIV
  All Rights Reserved.

  Permission to use, copy, modify and distribute this software and its
  documentation for non-commercial purposes is hereby granted,
  provided that both the copyright notice and this permission notice
  appear in all copies of the software, derivative works or modified
  versions, and any portions thereof, and that both notices appear in
  supporting documentation.

  THE UNIVERSITY OF DORTMUND ALLOWS FREE USE OF THIS SOFTWARE
  IN ITS "AS IS" CONDITION.  THE UNIVERSITY OF DORTMUND DISCLAIMS
  ANY LIABILITY OF ANY KIND FOR ANY DAMAGES WHATSOEVER RESULTING
  FROM THE USE OF THIS SOFTWARE.

  The University of Dortmund requests users of this software
  to return to

       Arbeitsgebiet Schaltungen der Signalverarbeitung
       Fakultät für Elektrotechnik
       Universität Dortmund
       44221 Dortmund, Germany

  or `msipc@nt.e-technik.uni-dortmund.de' any improvements or
  extensions that they make and grant the University of Dortmund
  the unlimited use and distribution of these changes.

  Read README.LICENSING for further information.

*/

%{ 

#include <stdio.h>
#include <vaulutil.h>
#include <errno.h>

#define YYSTYPE char *
#include "npic.tab.h"

void yyerror(char *msg);

#define YY_USER_ACTION count_nl(yytext);

int lineno = 1;

void count_nl(char *str)
{
    while(*str) {
	if(*str == '\n')
	    lineno++;
	str++;
    }
}

//#define RETURN(x) mention(yytext, x); return x
#define RETURN(x) return x

void mention(char *v, int x)
{
    fprintf(stderr, "-> `%s' ", v);
    if(x < 128)
	fprintf(stderr, "'%c'\n", x);
    else
	fprintf(stderr, "%d\n", x);
}

extern char *texify(const char *);

%}

%option noyywrap

%%

[ \t\n]*          /* ignore whitespace */

[-:@,;.!]          { RETURN(yytext[0]); }

[^ \t\n':@,:.%!-]* { yylval = texify(yytext); RETURN(t_TEXT); }

'([^']|(\\'))*'    { yytext[yyleng-1] = '\0'; 
                    yylval = xstrdup(yytext+1); 
                    RETURN(t_TEXT); }

\%.*             /* ignore comments */

%%

void yyerror(char *str)
{
    fprintf(stderr, "%d:", lineno);
    fprintf(stderr, " %s\n", str? str : "lex error");
}

