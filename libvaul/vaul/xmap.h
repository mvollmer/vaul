/* mapping from string to integers

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

#ifndef VAUL_XMAP_H
#define VAUL_XMAP_H

#include <vaul/common.h>

struct vaul_binfile;

// vaul_xmap verwaltet Beziehungen zwischen namen und Integern
// [Die Integer werden/sollen möglichst klein gehalten.]
//
struct vaul_xmap {

	vaul_xmap();
	~vaul_xmap();

	int s2i(char *n);
		// liefert den zu 'n' passenden Integer und merkt sich
		// daß er gebraucht wurde.

	char *i2s(int i);
		// Liefert den zu 'i' passenden Namen oder 'NULL', falls
		// es keinen gibt.

	void write(vaul_binfile *f);
	void read(vaul_binfile *f);
		// Schreibt/liest alle Beziehungen,
		// nach/von 'f', Fehlermeldungen tauchen in 'f' auf.

	void reset();
		// Vergisst alle Beziehungen.

private:
	struct xmap {
		xmap *next;
		int id;
		char *str;
	        /* bool needed; */
	} *map;
};

#endif

