/* printing errors and such

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

#include <vaul/parser.h>
#include <vaul/design_unit.h>
#include <vaul/printer.h>

void vaul_error_printer::info(char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vinfo(fmt, ap);
    va_end(ap);
}

void vaul_error_printer::error(char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vinfo(fmt, ap);
    va_end(ap);
    n_errors++;
}

void vaul_error_printer::vinfo(char *fmt, va_list ap)
{
    vfprintf(log, fmt, ap);
    int l = strlen(fmt);
    if(fmt[l-2] != '%' || fmt[l-1] != '~')
	fprintf(log, "\n");
}

void vaul_parser::BisonError(char *msg)
{
    error("%s %C", msg, lex);
    lex->stop();
}

void vaul_parser::vinfo(char *fmt, va_list ap)
{
    if(announced_scope != cur_scope) {
	nScope s = cur_scope;
	while(s->id == NULL && s->continued)
	    s = s->continued;
	while(s->id == NULL && s->scope)
	    s = s->scope;
	if(announced_scope != s) {
	    announced_scope = s;
	    if(s->is(nkTopScope))
		vaul_error_printer::info("%!at top level:", lex, 0);
	    else if(s->is(nkArchitecture))
		vaul_error_printer::info("%!in %n(%s):", lex, 0,
					 s->continued, s->id);
	    else
		vaul_error_printer::info("%!in %n:", lex, 0, s);
	}
    }
    if(fmt[0] != '%' || !strchr(":?!~", fmt[1]))
	fprintf(log, "%?", lex);
    if(strstr(fmt, "XXX"))
	XXX_seen = true;
    vaul_error_printer::vinfo(fmt, ap);
}

void vaul_parser::format_out(FILE *f, const char *spec, int len, va_list &ap)
{
    if(verbose && spec[len-1] == 'n') {
	// XXX - not portable
	va_list ap2 = ap;
	nNode n = va_arg(ap2, nNode);
	if(n && n->is(nkDeclaration) && nDeclaration(n)->scope
	&& nDeclaration(n)->scope != announced_scope
	&& !nDeclaration(n)->scope->is(nkTopScope)) {
	    nScope as = announced_scope;
	    announced_scope = NULL;
	    fprintf(f, "%n::", nDeclaration(n)->scope);
	    announced_scope = as;
	}
    }
    if(spec[len-1] != '~')
	vaul_error_printer::format_out(f, spec, len, ap);
}

char *vaul_parser::get_source(nNode n)
{
    if(n->owner == NULL)
	return lex->filename;
    return vaul_node_printer::get_source(n);
}
