/* error handling

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

#include <vaul/errors.h>

#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <vaulutil.h>

vaul_error_source::vaul_error_source()
{
    error_code = 0;
    error_desc = NULL;
}

vaul_error_source::~vaul_error_source()
{
    set_error_desc(NULL);
}

void vaul_error_source::set_error()
{
    set_error("");
}

void vaul_error_source::set_error(char *fmt, ...)
{
    if(errno) {
	error_code = errno;
	va_list ap;
	va_start(ap, fmt);
	set_error_desc(vaprintf(fmt, ap));
	more_error_desc(strerror(error_code));
	va_end(ap);
    } else
	clear_error();
}

void vaul_error_source::set_error(int code, char *fmt, ...)
{
    if(code) {
	error_code = code;
	va_list ap;
	va_start(ap, fmt);
	set_error_desc(vaprintf(fmt, ap));
	va_end(ap);
    } else
	clear_error();
}

void vaul_error_source::set_error(vaul_error_source *es, char *fmt, ...)
{
    if(es->error_code) {
	error_code = es->error_code;
	va_list ap;
	va_start(ap, fmt);
	set_error_desc(vaprintf(fmt, ap));
	more_error_desc(es->error_desc);
	va_end(ap);
    } else
	clear_error();
}

void vaul_error_source::set_error(vaul_error_source &es, char *fmt, ...)
{
    if(es.error_code) {
	error_code = es.error_code;
	va_list ap;
	va_start(ap, fmt);
	set_error_desc(vaprintf(fmt, ap));
	more_error_desc(es.error_desc);
	va_end(ap);
    } else
	clear_error();
}

void vaul_error_source::clear_error()
{
    error_code = 0;
    set_error_desc(NULL);
}

void vaul_error_source::print_err(FILE *f, char *msg)
{
    fprintf(f, "%s: ", base_progname);
    if(msg)
	fprintf(f, "%s: ", msg);
    if(error_desc)
	fprintf(f, "%s\n", error_desc);
    else
	fprintf(f, "BOGUS ERROR REPORT\n");
}

void vaul_error_source::print_err(char *msg)
{
    print_err(stderr, msg);
}

void vaul_error_source::set_error_desc(char *s)
{
    free(error_desc);
    error_desc = s;
}

void vaul_error_source::more_error_desc(char *s)
{
    if(error_desc == NULL)
	set_error_desc(s);
    else
	set_error_desc(aprintf("%s%s", error_desc, s));
}
