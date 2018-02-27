/* functions, procedures and interfaces

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
#include <vaul/tree.h>
#include <vaul/design_unit.h>
#include <vaul/library.h>

#include <stdio.h>
#include <vaulutil.h>
#include <stdlib.h>
#include <assert.h>

#define psr vaul_parser

#define NEW_NODE_PLACEMENT (pool)
#include "tree.m"

void psr::validate_interface(nSubprogram s, nInterface i)
{
    if(i->object_class == ObjClass_File) {
	if(i->mode != Mode_None) {
	    error("%:%n can't have a mode", i, i);
	    i->mode = Mode_None;
	}
    } else if(i->mode == Mode_None)
	i->mode = Mode_In;

    if(s->is(nkProcedure)) {
	if(i->mode == Mode_Buffer || i->mode == Mode_Linkage) {
	    error("%:illegal mode for %n", i, i);
	    i->mode = Mode_InOut;
	}
	if(i->object_class == ObjClass_None) {
	    if(i->mode == Mode_In)
		i->object_class = ObjClass_Constant;
	    else
		i->object_class = ObjClass_Variable;
	}
    } else {
	if(i->mode != Mode_In) {
	    error("%:%n must have mode 'in'", i, i);
	    i->mode = Mode_In;
	}
	if(i->object_class == ObjClass_None)
	    i->object_class = ObjClass_Constant;
    }
}

void psr::validate_port(nInterface i)
{
    if(i->object_class != ObjClass_Signal) {
	if(i->object_class != ObjClass_None)
	    error("%:port %n must be a signal", i, i);
	i->object_class = ObjClass_Signal;
    }
    if(i->mode == Mode_None)
	i->mode = Mode_In;
}

void psr::validate_generic(nInterface i)
{
    if(i->object_class != ObjClass_Constant) {
	if(i->object_class != ObjClass_None)
	    error("%:generic %n must be a constant", i, i);
	i->object_class = ObjClass_Constant;
    }
    if(i->mode != Mode_In) {
	if(i->mode != Mode_None)
	    error("%:generic must have mode 'in'", i, i);
	i->mode = Mode_In;
    }
}

