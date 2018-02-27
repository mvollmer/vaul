/* some list hacks

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

#include <stddef.h>

#include <vaul/list.h>

#define memberat(type, ptr, off) (*((type *)(((char *)(ptr))+(off))))

void *generic_reverse(void *l, size_t noff)
{
    void *rl = NULL, *n;
    while(l) {
	n = memberat(void *, l, noff);
	memberat(void *, l, noff) = rl;
	rl = l;
	l = n;
    }
    return rl;
}

void *generic_concat(void *l1, void *l2, size_t noff)
{
    if(l2 == NULL)
	return l1;

    void **l;
    for(l = &l1; *l; l = &memberat(void *, *l, noff))
	;
    *l = l2;
    return l1;
}
