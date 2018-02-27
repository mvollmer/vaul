/* The lexical scanner for ctree

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

/*
**  Der lexer von ctree
**  - keine ordentlichen Fehlermeldungen
**  - primitive Speicherverwaltung
*/

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <vaulutil.h>

#define YYSTYPE char *
#include "parser.tab.h"

extern int enter_code_block;
extern bool do_doc;

void emit_verb_impl(char *);
void emit_verb_head(char *);

char stash_buf[1024], *stash_ptr;

void stash(char *text)
{
    if(stash_ptr + strlen(text) > stash_buf + 1024)
	fatal("too much extra text\n");
    strcpy(stash_ptr, text);
    stash_ptr += strlen(text);
}

char ring_buf[1024];
char *rover = ring_buf;

char *store(char *str)
{
    int l = strlen(str);
    if(l >= 1024)
	fatal("can't store tokens longer than 1023 chars\n");
    if(l > ring_buf+1023-rover)
	rover = ring_buf;
    strcpy(rover, str);
    str = rover;
    rover += l;
    return str;
}

void yyerror(char *msg);

#define MAX_INCLUDE_DEPTH 10
struct source {
    YY_BUFFER_STATE buf;
    int lineno;
    char *filename;
};

source include_stack[MAX_INCLUDE_DEPTH];
int include_stack_ptr = 0;

#define YY_USER_ACTION count_nl(yytext);

int lineno = 1;
char *filename = NULL;

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

%}

%option noyywrap

INT	[0-9]+
FLOAT	(([0-9]*"."[0-9]+)|([0-9]+"."[0-9]*))([eE][+-]?{INT})
NUM	{INT}|{FLOAT}
ID	[a-zA-Z_][a-zA-Z_0-9]*

%x tree imp code incl comm

%%

    int brace_nest;

    if(enter_code_block) {
	enter_code_block = 0;
	brace_nest = 0;
	stash_ptr = stash_buf;
	BEGIN(code);
    }

"%%graph"   { BEGIN(tree); }

<tree>"node"	{ RETURN(t_NODE); }
<tree>"attr"	{ RETURN(t_ATTR); }
<tree>"prefixes" { RETURN(t_PREFIXES); }
<tree>"reverse" { RETURN(t_REVERSE); }
<tree>"extra"	{ RETURN(t_EXTRA); }
<tree>"extra-init" { RETURN(t_INIT_EXTRA); }
<tree>"arg"	{ RETURN(t_ARG); }
<tree>"include" { BEGIN(incl); }
<tree>{ID}	{ yylval = xstrdup(yytext); RETURN(t_ID); }
<tree>{NUM}	{ yylval = xstrdup(yytext); RETURN(t_NUM); }
<tree>"{"|"}"|"("|")"|":"|";"|"*"|"&"|","|"="	{ yylval = store(yytext);
					      RETURN(yytext[0]); }
<tree>[ \t\n]*	    { }
<tree>"//".*\n	    { }
<tree>"/*"	{ BEGIN(comm); }
<tree>"%%endgraph".*\n	{ BEGIN(INITIAL); RETURN(t_STOP); }
<tree>[^ [\t\n{}():;*&,=]+ { yylval = xstrdup(yytext); 
			    RETURN(t_OTHER); }

<tree>"["[^]]*"]" {  yylval = do_doc? xstrdup(yytext) : NULL;
		       RETURN (t_DOC); }

<incl>[ \t]*	{ }
<incl>[^ \t\n]+ { 
		    if(include_stack_ptr >= MAX_INCLUDE_DEPTH) {
			yyerror("includes nested too deep.");
			exit(10);
		    }
		    source *s = include_stack+include_stack_ptr++;
		    s->buf = YY_CURRENT_BUFFER;
		    s->lineno = lineno;
		    s->filename = filename;
		    yyin = fopen(yytext, "r");
		    if(yyin == NULL) {
			yyerror(NULL);
			fprintf(stderr, "%s: %s\n", yytext, strerror(errno));
			exit(10);
		    }
		    lineno = 1;
		    filename = xstrdup(yytext);
		    yy_switch_to_buffer(yy_new_buffer(yyin, YY_BUF_SIZE));
		    BEGIN(tree);
		}
<tree><<EOF>>	{   if(include_stack_ptr-- < 0)
			yyterminate();
		    else {
			yy_delete_buffer(YY_CURRENT_BUFFER);
			source *s = include_stack + include_stack_ptr;
			lineno = s->lineno;
			filename = s->filename;
			yy_switch_to_buffer(s->buf);
		    }
		}

<comm>[^*]*	    { }
<comm>"*"+[^*/]*    { }
<comm>"*"+"/"	    { BEGIN(tree); }

"%%implementation" { BEGIN(imp); }

<imp>(.|\n)*	{ emit_verb_impl(yytext); }

.|\n*		{ emit_verb_head(yytext); }

<code>[ \t\n]*"}"   { 
	if(--brace_nest == 0) {
	    stash("\n");
	    yylval = xstrdup(stash_buf);
	    BEGIN(tree);
	    RETURN(t_CODE);
	} else
	    stash(yytext);
    }

<code>"{"[ \t\n]*   { if(brace_nest++ > 0)
			 stash(yytext);
		      else if(yytext[yyleng-1] != '\n')
			 stash("\t");
		    }
<code>^[ \t\n]*	    { stash("\t"); }
<code>.|\n	    { stash(yytext); }

%%

void yyerror(char *str)
{
    if(filename)
	fprintf(stderr, "%s:", filename);
    fprintf(stderr, "%d:", lineno);
    if(str)
	fprintf(stderr, " %s\n", str);
}

