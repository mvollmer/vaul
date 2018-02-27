/* flex scanner.

 This file is based on `vhdl.lex' by Thomas Dettmer. `vhdl.lex' carries
 the following notice:

 ************** VHDL scanner in LEX format **********
 *
 * Version 0.2 Wed Aug 11, 1993
 *
 * This scanner is derived from a scanner of the ALLIANCE CAD toolset,
 * release 1.1. That toolset was written from:
 *   MASI/CAO-VLSI CAD Team
 *   Laboratoire MASI/CAO-VLSI
 *   Tour 55-65, 2eme etage, Porte 13
 *   Universite Pierre et Marie Curie (PARIS VI)
 *   4, place Jussieu 75252 PARIS Cedex 05, FRANCE
 * The ALLIANCE CAD Toolset can be obtained from ftp site : ftp-masi.ibp.fr    
 *
 * This scanner is avail at: ftp.cs.utwente.nl in pub/src/VHDL/Grammar
 * A corresponding Yacc grammar is available at the same site
 *
 * author of this derived scanner version:
 * Thomas Dettmer
 * Dortmund University
 * Dept. of Computer Scienc, LS1
 * PB 500 500
 * D-44221 Dortmund (Germany)
 * Phone: +49-231-755-6464
 * e-mail: dettmer@ls1.informatik.uni-dortmund.de
 *
 * Please report discovered bugs, critics, suggestions
 * and changes you make to dettmer@ls1.informatik.uni-dortmund.de
 * a list of those that reported repaired (hopefully) errors can be found
 * at the end
 *
 ****************************************************************
 * 
 * This file is intended not to be used for commercial purposes
 * without permission of the University of Dortmund
 *
 * NOTE THAT THERE IS NO WARRANTY FOR CORRECTNES, COMPLETENESS, SUPPORT
 * OR ANYTHING ELSE.
 *******************************************************

 Modifications and actions written by mvo@nt.e-technik.uni-dortmund.de

*/
%{

#include <vaul/lexer.h>

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <vaulutil.h>
#include <ctype.h>

inline static void str_to_name(char *n)
{
    while(*n) {
	*n = tolower(*n);
	n++;
    }
}

#include "parser.tab.h"

#define YY_DECL int vaul_lexer::lex(vaul_yystype *val, vaul_yyltype *loc)

#define set_loc()	{ loc->first_line = lineno; }
#define YY_USER_ACTION	set_loc();

#define MVL_LG_MC 15
#define MVL_NB_MC 81

int MVL_LINNUM=1;

typedef struct {
	char nom[MVL_LG_MC];
	int kval;
	} el_mc;

static el_mc tab_mc []=
  {
    {"abs"	    ,t_ABS},
    {"access"	    ,t_ACCESS},
    {"after"	    ,t_AFTER},
    {"alias"	    ,t_ALIAS},
    {"all"	    ,t_ALL},
    {"and"	    ,t_AND},
    {"architecture" ,t_ARCHITECTURE},
    {"array"	    ,t_ARRAY},
    {"assert"	    ,t_ASSERT},
    {"attribute"    ,t_ATTRIBUTE},

    {"begin"	    ,t_BEGIN},
    {"block"	    ,t_BLOCK},
    {"body"	    ,t_BODY},
    {"buffer"	    ,t_BUFFER},
    {"bus"	    ,t_BUS},

    {"case"	    ,t_CASE},
    {"component"    ,t_COMPONENT},
    {"configuration"	,t_CONFIGURATION},
    {"constant"	    ,t_CONSTANT},

    {"disconnect"   ,t_DISCONNECT},
    {"downto"	    ,t_DOWNTO},

    {"else"	    ,t_ELSE},
    {"elsif"	    ,t_ELSIF},
    {"end"	    ,t_END},
    {"entity"	    ,t_ENTITY},
    {"exit"	    ,t_EXIT},

    {"file"	    ,t_FILE},
    {"for"	    ,t_FOR},
    {"function"	    ,t_FUNCTION},

    {"generate"	    ,t_GENERATE},
    {"generic"	    ,t_GENERIC},
    {"guarded"	    ,t_GUARDED},

    {"if"	    ,t_IF},
    {"in"	    ,t_IN},
    {"inout"	    ,t_INOUT},
    {"is"	    ,t_IS},

    {"label"	    ,t_LABEL},
    {"library"	    ,t_LIBRARY},
    {"linkage"	    ,t_LINKAGE},
    {"loop"	    ,t_LOOP},

    {"map"	    ,t_MAP},
    {"mod"	    ,t_MOD},

    {"nand"	    ,t_NAND},
    {"new"	    ,t_NEW},
    {"next"	    ,t_NEXT},
    {"nor"	    ,t_NOR},
    {"not"	    ,t_NOT},
    {"null"	    ,t_NULL},

    {"of"	    ,t_OF},
    {"on"	    ,t_ON},
    {"open"	    ,t_OPEN},
    {"or"	    ,t_OR},
    {"others"	    ,t_OTHERS},
    {"out"	    ,t_OUT},

    {"package"	    ,t_PACKAGE},
    {"port"	    ,t_PORT},
    {"procedure"    ,t_PROCEDURE},
    {"process"	    ,t_PROCESS},

    {"range"	    ,t_RANGE},
    {"record"	    ,t_RECORD},
    {"register"	    ,t_REGISTER},
    {"rem"	    ,t_REM},
    {"report"	    ,t_REPORT},
    {"return"	    ,t_RETURN},

    {"select"	    ,t_SELECT},
    {"severity"	    ,t_SEVERITY},
    {"signal"	    ,t_SIGNAL},
    {"subtype"	    ,t_SUBTYPE},

    {"then"	    ,t_THEN},
    {"to"	    ,t_TO},
    {"transport"    ,t_TRANSPORT},
    {"type"	    ,t_TYPE},

    {"units"	    ,t_UNITS},
    {"until"	    ,t_UNTIL},
    {"use"	    ,t_USE},

    {"variable"	    ,t_VARIABLE},

    {"wait"	    ,t_WAIT},
    {"when"	    ,t_WHEN},
    {"while"	    ,t_WHILE},
    {"with"	    ,t_WITH},

    {"xor"	    ,t_XOR}
};

typedef int cmpfunc_t(const void *, const void *);

static int find_mc(char *s)
{
    char l[strlen(s)+1];
    strcpy(l, s);
    str_to_name(l);
    el_mc *pt = (el_mc *) bsearch(l, tab_mc, MVL_NB_MC, sizeof(el_mc),
			  (cmpfunc_t *)strcmp);
    if (pt==NULL)
	return(-1);
    else
	return(pt->kval);
}

%}

%option C++ noyywrap

upper_case_letter	[A-Z]
digit			[0-9]
special_char		[\#\&\'\(\)\*\+\,\-\.\/\:\;\<\=\>\_\|]
space_char		[ \t]
format_effector		[\t\v\r\l\f]
end_of_line		\n
lower_case_letter	[a-z]
other_special_char	[\!\$\@\?\[\\\]\^\`\{\}\~]

graphic_char {basic_graphic_char}|{lower_case_letter}|{other_special_char}
basic_graphic_char {upper_case_letter}|{digit}|{special_char}|{space_char}
letter		    {upper_case_letter}|{lower_case_letter}
letter_or_digit	    {letter}|{digit}
decimal_literal	    {integer}(\.{integer})?({exponent})?
integer		    {digit}(_?{digit})*
exponent	    [eE][-+]?{integer}
base		    {integer}
based_integer	    {extended_digit}(_?{extended_digit})*
extended_digit	    {digit}|[a-fA-F]
base_specifier	    (B|b|O|o|X|x)
%%

	    if(stopped) return 0;

{space_char}	    /* nothing */

\&	    return t_Ampersand;
\'	    return t_Apostrophe;
\(	    return t_LeftParen;
\)	    return t_RightParen;
"**"	    return t_DoubleStar;
\*	    return t_Star;
\+	    return t_Plus;
\,	    return t_Comma;
\-	    return t_Minus;
":="	    return t_VarAsgn;
\:	    return t_Colon;
\;	    return t_Semicolon;
"<="	    return t_LESym;
">="	    return t_GESym;
\<	    return t_LTSym;
\>	    return t_GTSym;
\=	    return t_EQSym;
"/="	    return t_NESym;
"=>"	    return t_Arrow;
"<>"	    return t_Box;
\|	    return t_Bar;
!	    return t_Bar;
\.	    return t_Dot;
\/	    return t_Slash;

{letter}(_?{letter_or_digit})*	{

	    int itoken;
	    itoken=find_mc(yytext);
	    if (itoken== -1) {
		val->text = pool.add(yytext);
		return t_Identifier;
	    } else
		return itoken;
	}


{decimal_literal}|({base}#{based_integer}(\.{based_integer})?#{exponent}?)|({base}:{based_integer}(\.{based_integer})?:{exponent}?)	{

	    val->text = pool.add(yytext);
	    return t_AbstractLit;
	}


'({graphic_char}|\"|\%)'    {

	    val->text = pool.add(yytext);
	    return t_CharacterLit;
	}

(\"({graphic_char}|(\"\")|\%)*\")|(\%({graphic_char}|(\%\%)|\")*\%) {

	    val->text = pool.add(yytext);
	    return t_StringLit;
	}

{base_specifier}(\"{extended_digit}(_?{extended_digit})*\"|\%{extended_digit}(_?{extended_digit})*\%)		{

	    val->text = expand_bitstring(yytext);
	    return t_StringLit;
	}

\n	    lineno++;

\-\-.*$	    /* comment */

.	{   
	    if(prt)
		prt->fprintf(log, "%?illegal character '0x%02x'\n",
				     this, (unsigned char)yytext[0]);
	}


%%

int yyFlexLexer::yylex()
{
    fatal("internal lexer fuck up\n");
    return 0;
}

vaul_lexer::vaul_lexer(char *fn, FILE *f)
{
    close_file = 0;
    file = NULL;
    filename = xstrdup(fn);
    lineno = 1;
    prt = NULL;

    if(f == NULL) {
	if(!(f = fopen(fn, "r"))) {
	    set_error();
	    return;
	}
	close_file = 1;
    }
    file = f;

    stopped = false;
}

vaul_lexer::~vaul_lexer()
{
    if(close_file)
	fclose(file);
    free(filename);
}

void vaul_lexer::stop()
{
    stopped = true;
}

int vaul_lexer::LexerInput(char *buf, int max_size)
{
    if(!fgets(buf, max_size, file))
	return 0;
    return strlen(buf);
}

void vaul_lexer::LexerError(char *m)
{
    if(prt)
	prt->fprintf(log, "%?%s %C\n", this, m, this);
    else
	fprintf(stderr, "%s:%d: %s at %s\n", filename, lineno, m, context());
}

bool is_abstract_literal(char *tok)
{
    return isdigit(*tok);
}

bool is_integer_literal(char *tok)
{
    return is_abstract_literal(tok) && !strchr(tok, '.');
}

bool is_real_literal(char *tok)
{
    return is_abstract_literal(tok) && strchr(tok, '.');
}

bool is_character_literal(char *tok)
{
    return tok[0] == '\'';
}

bool is_string_literal(char *tok)
{
    return tok[0] == '"';
}

char *vaul_lexer::expand_bitstring(const char *tok)
{
    char buf[strlen(tok)*4];
    char *bp = buf;

    char bid = tolower(tok[0]);
    int bits = (bid == 'b'? 1 : bid == 'o'? 3 : bid == 'x'? 4 : 0);
    if(bits == 0 || tok[1] != '"' || tok[strlen(tok)-1] != '"') {
	prt->fprintf(log, "%?illegal bitstring literal\n", this);
	return NULL;
    }

    *bp++ = '"';
    for(const char *cp = tok+2; *cp != '"'; cp++) {
	if(*cp == '_')
	    continue;
	int dig = tolower(*cp) - '0';
	if(dig > 10)
	    dig -= 'a'-'9'-1;
	if(dig >= (1 << bits)) {
	    prt->fprintf(log, "%?illegal digit '%c' in bitstring literal\n",
			 this, *cp);
	    dig = 0;
	}

	for(int i = bits-1; i >= 0; i--)
	    *bp++ = (dig & (1 << i))? '1' : '0';
    }
    *bp++ = '"';
    *bp++ = '\0';
//  prt->fprintf(log, "%?xxx - expanded '%s' to '%s'\n", this, tok, buf);
    return pool.add(buf);
}

#if defined(sun) && !defined(__sysvr4__)
extern "C" int strcasecmp(const char *, const char *);
#endif

bool vaul_name_eq(const char *n1, const char *n2)
{
    if(n1 == NULL || n2 == NULL)
	return false;
    if(n1[0] == '\\' || n1[0] == '\'')
	return !strcmp(n1, n2);
    else
	return !strcasecmp(n1, n2);
}
