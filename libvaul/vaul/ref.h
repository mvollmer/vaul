/* reference counting

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

#ifndef VAUL_REF_H
#define VAUL_REF_H

//  vaul_ref zählt mit, wie viele Referenzen auf 'this' irgendwo da
//  draussen sind.
//
struct vaul_ref {

    vaul_ref();
	// Initialisiert 'ref_count' mit 1.

    virtual ~vaul_ref();
	// Beschwert sich, wenn 'ref_count != 0'.

    void release();
	// Registriert, daß es eine Referenz weniger gibt.
	// Wenn 'ref_count == 0' wird, gibt es ein 'delete this'.
	
    void retain();
	// Registriert, daß es eine Referenz mehr gibt.

    unsigned int ref_count;
	// Die Anzahl der Referenzen. Handle with care.
};

inline void retain_ref(vaul_ref *r)     { if(r) r->retain(); }
inline void release_ref(vaul_ref *r)	{ if(r) r->release(); }
    // Wie 'r->release()'/'r->retain()', geht aber auch wenn 'r == NULL'.

#endif
