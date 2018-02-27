/* the lexical scanner

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
       Fakult�t f�r Elektrotechnik
       Universit�t Dortmund
       44221 Dortmund, Germany

  or `msipc@nt.e-technik.uni-dortmund.de' any improvements or
  extensions that they make and grant the University of Dortmund
  the unlimited use and distribution of these changes.

  Read README.LICENSING for further information.

*/

#ifndef VAUL_LEX_H
#define VAUL_LEX_H

#include <vaul/errors.h>
#include <vaul/tree.h>
#include <vaul/mempool.h>

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <FlexLexer.h>

// sind zwei VHDL-Namen gleich?
//
bool vaul_name_eq(const char *n1, const char *n2);

// vaul_yystype h�lt den semantischen Wert von Token
//
union vaul_yystype {
    Id id;
    nTree tree;
    RangeDirection dir;
    ObjectClass obj_class;
    Mode mode;
    SignalKind signal_kind;
    char *text;
    bool boolean;
    int kind;
};

// vaul_yyltype h�lt Informationen �ber die Position eines Tokens
//
struct vaul_yyltype {
    int first_line,
	last_line,
	first_column,
	last_column;
    char *text;

    operator int () { return first_line; }
};

bool is_abstract_literal(char *tok);
bool is_integer_literal(char *tok);
bool is_real_literal(char *tok);
bool is_character_literal(char *tok);
bool is_string_literal(char *tok);

// vaul_lexer enth�lt den von 'flex++' generierten lexikalischen
// Analysator.
//
struct vaul_lexer : public yyFlexLexer, vaul_error_source {

    vaul_lexer(char *filename, FILE *f = NULL);
	// �ffnet die Datei 'filename' zum Lesen, wenn 'f == NULL'.
	// Sonst wird aus 'f' gelesen und 'filename' wird nur f�r
	// Fehlermeldungen verwendet.

    ~vaul_lexer();

    int lex(vaul_yystype *val, vaul_yyltype *loc);
	// Sucht das n�chste Token und f�llt '*val' und '*loc' mit
	// passenden Werten.

    int lineno;
    char *filename;

    vaul_stringpool pool;
	// h�lt alle gelesenen Stringkonstanten

    void set_printer(vaul_printer *p, FILE *f)	{ prt = p; log = f; }

    char *context()
	{ return yytext[0]? yytext : "end of input"; }

    void stop();

private:
    FILE *file;
    bool close_file;

    vaul_printer *prt;
    FILE *log;

    bool stopped;

    char *expand_bitstring(const char *);

    int LexerInput(char *buf, int max_size);
    void LexerError(char *msg);
    void message(char *fmt, va_list ap);
    void message(vaul_yyltype &loc, char *fmt, va_list ap);
};

#endif // VAUL_LEX_H

