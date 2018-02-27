/* mapping from strings to integers

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

#include <vaul/xmap.h>
#include <vaul/binfile.h>

#include <vaulutil.h>

vaul_xmap::vaul_xmap()
{
    map = NULL;
}

vaul_xmap::~vaul_xmap()
{
    reset();
}

void vaul_xmap::reset()
{
    xmap *x, *x2;
    for(x = map; x; x = x2) {
	x2 = x->next;
	free(x->str);
	delete x;
    }
    map = NULL;
}

int vaul_xmap::s2i(char *s)
{
    xmap *x;
    for(x = map; x; x = x->next)
	if(!strcmp(x->str, s)) {
	    /* x->needed = true; */
	    return x->id;
	}
    int id = 1;
    xmap **xp;
    for(xp = &map; *xp && (*xp)->id == id; xp = &(*xp)->next, id++)
	;
    x = new xmap;
    x->id = id;
    /* x->needed = true; */
    x->str = xstrdup(s);
    x->next = *xp;
    *xp = x;
    return id;
}

char *vaul_xmap::i2s(int i)
{
    xmap *x;
    for(x = map; x && x->id < i; x = x->next)
	;
    if(x && x->id == i)
	return x->str;
    return NULL;
}

void vaul_xmap::write(vaul_binfile *f)
{
    for(xmap *x = map; x; x = x->next) {
	/* if(x->needed) */ {
	    f->put(x->id);
	    f->put(x->str);
	}
    }
    f->put(0);
}

void vaul_xmap::read(vaul_binfile *f)
{
    reset();
    xmap **xp = &map;
    while(!f->is_error()) {
	int id;
	f->get(id);
	if(id == 0)
	    break;
	xmap *x = new xmap;
	x->id = id;
	/* x->needed = false; */
	f->get(x->str);
	*xp = x;
	xp = &x->next;
    }
    *xp = NULL;
}

