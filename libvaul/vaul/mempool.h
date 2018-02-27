/* memory management

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

#ifndef VAUL_MEMPOOL_H
#define VAUL_MEMPOOL_H

#include <vaul/common.h>

#include <stddef.h>

//	vaul_memregion hält beliebeigen Speicher zusammen, der
//	vom Destruktor freigegeben wird.
//
struct vaul_memregion {

	vaul_memregion();
	~vaul_memregion();

	bool contains(void *mem);
		// Ermittelt, ob 'mem' irgendwo innerhalb eines Blockes
		// liegt, der zu dieser 'vaul_memregion' gehört.

	struct block {
		block *link;
		size_t size;
		char  mem[0];
	} *blocks;
};

//	vaul_mempool kann Speicher besorgen, der automatisch mit dem
//	'vaul_mempool' zusammen freigegeben wird.
//
//
struct vaul_mempool : protected vaul_memregion {

	vaul_mempool();
	~vaul_mempool();
		// Gibt allen gemerkten Speicher frei.

	void *alloc(size_t size);
		// Besorgt 'size' Bytes Speicher und merkt ihn sich.

	void store(vaul_memregion *r);
	void store(vaul_memregion &r);
		// Transferiert allen gemerkten Speicher nach 'r' und 
		// vergisst ihn.

	bool contains(void *mem)
		{ return vaul_memregion::contains(mem); }

private:
	char *mem;
	size_t free;
};

inline void *operator new(size_t size, vaul_mempool *pool)
 { return pool->alloc(size); }

inline void *operator new(size_t size, vaul_mempool &pool)
 { return pool.alloc(size); }

//	vaul_stringpool besorgt Speicher für Strings und vermeidet
//	Duplikate
//
struct vaul_stringpool : private vaul_mempool {

	vaul_stringpool();
	~vaul_stringpool();

	char *add(char *str);
		// Fügt 'str' zum Pool hinzu, falls es noch nicht drin
		// ist. Strings mit gleichem Inhalt führen zu gleichen
		// Rückgabewerten.

	void store(vaul_memregion *m)	{ vaul_mempool::store(m); }
	void store(vaul_memregion &m)	{ vaul_mempool::store(m); }
};

#endif


