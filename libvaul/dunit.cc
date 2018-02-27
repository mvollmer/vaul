/* implementation of vaul_design_unit

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

#undef DEBUG

#include <vaul/design_unit.h>
#include <vaul/binfile.h>
#include <vaul/tree.h>
#include <vaul/debug.h>

#include <errno.h>
#include <string.h>
#include <vaulutil.h>

vaul_design_unit::vaul_design_unit(char *n, char *s)
{
    name = xstrdup(n);
    source = xstrdup(s);
    tree = NULL;
    used_dus = NULL;
    lib = NULL;
    ctime = 0;
    reader = NULL;
}

vaul_design_unit::~vaul_design_unit()
{
    debug("deleting unit %p (%s)\n", this, name);

    free(name);
    free(source);
    du_entry *de, *de2;
    for(de = used_dus; de; de = de2) {
	de2 = de->link;
	de->du->release();
	delete de;
    }
    delete reader;
}

void vaul_design_unit::set_name(char *n)
{
    if(name == NULL)
	name = xstrdup(n);
}

void vaul_design_unit::set_ctime()
{
    if(ctime == 0)
	time(&ctime);
}

void vaul_design_unit::uses(vaul_design_unit *du)
{
    if(du == this)
	return;

    du_entry *de;
    for(de = used_dus; de; de = de->link)
	if(de->du == du)
	    return;
    de = new du_entry;
    retain_ref(du);
    de->du = du;
    de->link = used_dus;
    used_dus = de;
}

nNode vaul_design_unit::find_node(int label)
{
    if(tree == NULL || reader == NULL)
	return NULL;
    return reader->find_node(label);
}

void vaul_design_unit::print()
{
    printf("%s", name? name : "<unnamed>");
    if(source)
	printf(" %s", source);
    char tbuf[50];
    strftime(tbuf, 50, "%b %d %H:%M", localtime(&ctime));
    printf(" %s:\n", tbuf);
    if(tree) {
	vaul_graph_printer pr(this);
	pr.print(tree);
    } else
	printf("no tree\n");
}

bool vaul_design_unit::is_older_than(vaul_design_unit *du)
{
    return ctime > 0 && ctime < du->ctime;
}

void vaul_design_unit::write(vaul_binfile *f)
{
    if(ctime == 0) {
	if(name)
	    message("design unit %s has unset creation time.\n", name);
	else
	    message("unnamed design unit has unset creation time.\n");
    }

    // Namen werden ins Inhaltsverzeichnis geschrieben.
    f->put(source);
    f->put(ctime);
    vaul_graph_writer wr(this, f);
    wr.write(tree);
    set_error(wr);
    if(!is_error())
	set_error(f);
}

void vaul_design_unit::read(vaul_binfile *f, vaul_libpool *libs)
{
    free(source);
    f->get(source);
    f->get(ctime);
    if(libs) {
	delete reader;
	reader = new vaul_graph_reader(this, f, libs);
	tree = nScope(reader->read(nkScope));
	if(reader->is_error()) {
	    tree = NULL;
	    set_error(reader);
	} else
	    reader->pool.store(mem);
    }
    if(!is_error())
	set_error(f);
}

vaul_design_unit *vaul_design_unit::find_du(nNode n)
{
    if(this->mem.contains(n))
	return this;
    for(du_entry *de = used_dus; de; de = de->link)
	if(de->du->mem.contains(n))
	    return de->du;
    return NULL;
}

vaul_design_unit *vaul_design_unit::query_used_dus(vaul_design_unit *du)
{
    if(du == NULL)
	return used_dus? used_dus->du : NULL;
    else {
	for(du_entry *de = used_dus; de; de = de->link)
	    if(de->du == du)
		return de->link? de->link->du : NULL;
	return NULL;
    }
}
