/* the `smart' library

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

#include "smart_lib.h"

#include <unistd.h>
#include <ctype.h>
#include <sys/stat.h>

smart_library::smart_library(char *n, int f, int t)
    : vaul_library(n, t)
{
    flags = f;
}

bool smart_library::analyze(char *fn, vaul_libpool *p)
{
    fprintf(stderr, "analyzing '%s'...\n", fn);
    vaul_design_file df(fn, NULL, (flags & slf_verbose) != 0);
    if(!p->begin_session(get_name())) {
	fprintf(stderr, "sorry: %s\n", p->get_error_desc());
	return false;
    }
    if(!df.insert(p)) {
	df.print_err(fn);
	p->end_session(true);
	return false;
    }
    p->end_session();
    return true;
}

static bool source_is_newer(vaul_design_unit *du)
{
    if(char *source = du->get_source()) {
	struct stat attr;
	if(stat(source, &attr) == 0)
	    return attr.st_mtime >= du->get_ctime();
    }
    return false;
}

static char *find_source(char *lib, char *unit)
{
    struct stat attr;
    char lunit[strlen(unit)+1];
    int i;
    for(i = 0; unit[i]; i++)
	lunit[i] = tolower(unit[i]);
    lunit[i] = '\0';
    char *fn = aprintf("vlib/%s/%s.vhd", lib, lunit);
    if(stat(fn, &attr) == 0)
	return fn;
    free(fn);
    return aprintf("vlib/%s/%s.vhdl", lib, lunit);
}

vaul_design_unit *smart_library::get(char *n, vaul_libpool *p)
{
    vaul_design_unit *du = vaul_library::get(n, p);
    if((flags & slf_smart)
       && (du == NULL || du->is_error() || source_is_newer(du))) {
	if(du && du->is_error())
	    du->print_err();
	fprintf(stderr, "for '%s': ", n);
	char *fn;
	if(du == NULL || du->get_source() == NULL)
	    fn = find_source(get_name(), n);
	else {
	    // 'du' might vanish anytime, so dup the source
	    // garbage collection would really help
	    fn = xstrdup(du->get_source());
	}
	release_ref(du);
	if(!analyze(fn, p)) {
	    free(fn);
	    return NULL;
	}
	du = vaul_library::get(n, p);
	free(fn);
    } 
    return du;
}

vaul_design_unit *smart_library::get(int i, vaul_libpool *p)
{
    vaul_design_unit *du = vaul_library::get(i, p);
    if((flags & slf_smart) && du && du->get_source()
       && (du->is_error() || source_is_newer(du))) {
	fprintf(stderr, "for [%d]: ", i);
	char *fn = xstrdup(du->get_source());
	release_ref(du);
	if(!analyze(fn, p)) {
	    free(fn);
	    return NULL;
	}
	du = vaul_library::get(i, p);
	free(fn);
    }
    return du;
}

char *smart_library::build_filename(char *n)
{
    return aprintf("vlib/%s/.cache", n);
}

char *smart_libpool::package_body_name(const char *p)
{
    return aprintf("%s-body", p);
}

char *smart_libpool::architecture_name(const char *e, const char *a)
{
    return aprintf("%s-of-%s", a, e);
}
