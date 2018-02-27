/* implementation of vaul_design_file

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

#include <vaul/design_file.h>
#include <vaul/library.h>

vaul_design_file::vaul_design_file(char *fn, FILE *f, bool v)
  : lexer(fn, f),
    parser(&lexer)
{
    set_error(lexer);
    parser.verbose = v;
}

vaul_design_file::~vaul_design_file()
{
}

vaul_design_unit *vaul_design_file::read_design_unit(vaul_libpool *p)
{
    if(is_error())
	return NULL;

    vaul_design_unit *du = parser.parse(p);
    return du;
}

void vaul_design_file::sync_vaul_errors()
{
    if(!is_error())
	set_error(parser);
}

void vaul_design_file::clear_vaul_errors()
{
    parser.clear_errors();
}

bool vaul_design_file::insert(vaul_libpool *p)
{
    if(is_error())
	return false;

    vaul_library *wl;
    if(!(wl = p->get_worklib())) {
	set_error(vhdlerr_usage, "no active session");
	return false;
    }

    do {
	vaul_design_unit *du = read_design_unit(p);
	if(du) {
	    if(checkpoint(du) && !wl->insert(du))
		set_error(wl, "%s: ", wl->get_name());
	    du->release();
	} 
    } while(!is_error() && !was_eof());
    sync_vaul_errors();
    return !is_error();
}

bool vaul_design_file::checkpoint(vaul_design_unit *)
{
    return true;
}
