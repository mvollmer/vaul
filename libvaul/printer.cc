/* beefed up printf

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

#include <vaul/printer.h>
#include <string.h>

void vaul_printer::printf(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stdout, fmt, ap);
    va_end(ap);
}

void vaul_printer::fprintf(FILE *f, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vfprintf(f, fmt, ap);
    va_end(ap);
}

void vaul_printer::vfprintf(FILE *out, const char *fmt, va_list ap)
{
    char *mods = "-+ #*0123456789.hlL";

    while(*fmt) {
	const char *cp;
	for(cp = fmt; *fmt && *fmt != '%'; fmt++)
	    ;
	string_out(out, cp, fmt-cp);
	if(*fmt == '%') {	
	    const char *spec = fmt++;
	    while(*fmt && strchr(mods, *fmt))
		fmt++;
	    if(*fmt) {
		fmt++;
		if(fmt[-1] == 'v') {
		    char *vfmt = va_arg(ap, char *);
		    va_list vap = va_arg(ap, va_list);
		    vfprintf(out, vfmt, vap);
		} else
		    format_out(out, spec, fmt-spec, ap);
	    }
	} 
    }
}

void vaul_std_printer::string_out(FILE *f, const char *s, int l)
{
    if(f)
	fwrite(s, sizeof(char), l, f);
}

void vaul_std_printer::format_out(FILE *f, const char *s, int l, va_list &ap)
{
    const char *int_fmts = "cdiouxX";
    const char *float_fmts = "feEgG";
    const char *ptr_fmts = "sp";

    char _s[l+1];
    strncpy(_s, s, l);
    _s[l] = '\0';
    char fmt = s[l-1];

    if(strchr(int_fmts, fmt)) {
	if(f) ::vfprintf(f, _s, ap);
	if(strchr(s, 'l') || strchr(s, 'L'))
	    va_arg(ap, long int);
	else
	    va_arg(ap, int);
    } else if(strchr(float_fmts, fmt)) {
	if(f) ::vfprintf(f, _s, ap);
	if(strchr(s, 'L'))
	    va_arg(ap, long double);
	else
	    va_arg(ap, double);
    } else if(strchr(ptr_fmts, fmt)) {
	if(f) ::vfprintf(f, _s, ap);
	va_arg(ap, void *);
    } else {
	if(f) fprintf(f, "<%s>", _s);
	va_arg(ap, int);
    }
}

	
