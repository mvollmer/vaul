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

#ifndef VAUL_PRINTF_H
#define VAUL_PRINTF_H

#include <stdio.h>
#include <stdarg.h>
#include <vaul/common.h>

struct vaul_printer {

	void printf(const char *fmt, ...);
	void fprintf(FILE *, const char *fmt, ...);

	void vfprintf(FILE *, const char *fmt, va_list ap);

protected:
	virtual void string_out(FILE *, const char *str, int len) = 0;
	virtual void format_out(FILE *, const char *spec, int len, va_list &ap) = 0;
};

struct vaul_std_printer : vaul_printer {

protected:
	virtual void string_out(FILE *, const char *str, int len);
	virtual void format_out(FILE *, const char *spec, int len, va_list &ap);
};

#endif // PRINTF_H

