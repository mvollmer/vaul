/* library and libpool implementation

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

#include <vaul/library.h>
#include <vaul/lock.h>
#include <vaul/design_unit.h>
#include <vaul/design_file.h>
#include <vaul/debug.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <errno.h>
#include <vaulutil.h>
#include <assert.h>
#include <ctype.h>

static char *xstrdup_to_lower(const char *str)
{
    char *xstr = xstrdup(str);
    for(char *cp = xstr; *cp; cp++)
	*cp = tolower(*cp);
    return xstr;
}

vaul_library::vaul_library(char *n, int to)
{
    name = xstrdup_to_lower(n);
    file_name = NULL;
    new_file = NULL;
    directory = NULL;
    temperature = 0;
    timeout = to;
    next_label = 0;
}

void vaul_library::init()
{
    debug("%s: init\n", name);

    clear_error();

    entry *e, *e2;
    for(e = directory; e; e = e2) {
	e2 = e->next;
	delete e;
    }
    directory = NULL;
    dir_tail = &directory;
    next_label = 1;

    libmap.reset();

    if(name) {
	file_name = build_filename(name);
	debug("%s: reading directory\n", name);
	vaul_binfile file(file_name, "r");
	if(!file.is_error()) {
	    read_dir(&file);
	    libmap.read(&file);
	} else if(fatal_reading_error(file.get_error()))
	    set_error(file);
    } else
	file_name = NULL;
}

char *vaul_library::build_filename(char *n)
{
    char *tmp = getenv("VAUL_LIBRARY_TEMPLATE");
    if(tmp == NULL)
	tmp = "%s.vaul";
    return aprintf(tmp, n);
}

vaul_library::~vaul_library()
{
    free(name);
    free(file_name);

    if(new_file) {
	if(!is_error())
	    message("\nUnflushed library destroyed!\n"
		    "(You should get a 'vhdlerr_usage',"
		    " but I have no way to report it properly.)\n\n");
	this->abort();
    }

    entry *e, *e2;
    for(e = directory; e; e = e2) {
	e2 = e->next;
	delete e;
    }
}

void vaul_library::freeze()
{
    temperature--;
}

void vaul_library::unfreeze()
{
    if(temperature < 0)
	temperature++;
}

bool vaul_library::insert(vaul_design_unit *du)
{
    if(is_error())
	return false;

    if(temperature < 0) {
	set_error(vhdlerr_frozen, "can't insert into frozen library");
	return false;
    }

    du->lib = this;
    du->label = 0;	// if this is ever used, we'll get a fatal error

    int off = 0, end = 0;

    if(name && start_writing()) {
	debug("%s: writing %s\n", name, du->get_name());
	off = new_file->tell();
	du->write(new_file);
	end = new_file->tell();
	debug("%s: done writing %s\n", name, du->get_name());
	set_error(du);
    } 

    if(is_error()) {
	du->lib = NULL;
	return false;
    }

    entry *e = lib_entry(du->get_name());
    debug("%s: inserting %s\n", name, e->name);
    e->attach(du, this);
    e->offset = off;
    e->length = end - off;
    e->state = inserted;
    e->tree_read = (du->tree != NULL);

    return true;
}

bool vaul_library::is_inserted(vaul_design_unit *du)
{
    if(du == NULL)
	return false;
    for(entry *e = directory; e; e = e->next)
	if(vaul_name_eq(e->name, du->get_name()))
	    return e->state == inserted;
    return false;
}

vaul_design_unit *vaul_library::get(char *name, vaul_libpool *libs)
{
    if(!start_reading())
	return NULL;

    entry *e;
    for(e = directory; e; e = e->next)
	if(vaul_name_eq(e->name, name))
	    return read_entry(e, libs);
    return NULL;
}

vaul_design_unit *vaul_library::get(int label, vaul_libpool *libs)
{
    if(!start_reading())
	return NULL;

    for(entry *e = directory; e; e = e->next)
	if(e->label == label)
	    return read_entry(e, libs);
    return NULL;
}

bool vaul_library::fatal_reading_error(int e)
{
    return e != 0 && e != ENOENT;
}

bool vaul_library::start_reading()
{
    if(is_error())
	return false;
    if(next_label == 0)
	init();
    return !is_error();
}

bool vaul_library::start_writing()
{
    if(!start_reading())
	return false;

    if(!new_file) {
	char *tmp = getenv("VAUL_TEMPLIB_TEMPLATE");
	if(tmp == NULL)
	    tmp = "%s.new";
	char *fn = aprintf(tmp, file_name);
	debug("%s: starting to write\n", name);
	new_file = new vaul_binfile(fn, "w");
	free(fn);

	// XXX - put real header
	new_file->put(0);

	if(new_file->is_error()) {
	    set_error(new_file);
	    delete new_file;
	    new_file = NULL;
	    return false;
	}
    }
    return true;
}

bool vaul_library::flush()
{
    if(is_error())
	return false;

    if(new_file) {
	debug("%s: flushing\n", name);
	vaul_binfile file(file_name, "r");
	for(entry *e = directory; e; e = e->next) {
	    if(e->state == normal) {
		file.seek(e->offset);
		e->offset = new_file->tell();
		new_file->put(&file, e->length);
		e->state = inserted;
		if(file.is_error()) {
		    set_error(file);
		    return false;
		}
	    }
	}

	int dir_off = new_file->tell();
	write_dir();
	libmap.write(new_file);
	new_file->seek(0);
	new_file->put(dir_off);

	if(!new_file->flush()) {
	    set_error(new_file);
	    return false;
	} 
    
	if(rename(new_file->get_name(), file.get_name()) < 0) {
	    set_error("%s: ", new_file->get_name());
	    return false;
	}

	delete new_file;
	new_file = NULL;
    }

    init();
    return !is_error();
}

bool vaul_library::abort()
{
    if(new_file) {
	unlink(new_file->get_name());
	delete new_file;
	new_file = NULL;
    }

    init();
    return !is_error();
}

vaul_design_unit *vaul_library::read_entry(entry *e, vaul_libpool *libs)
{
    if(e->du == NULL || (!e->tree_read && libs != NULL)) {
	assert(name != NULL);
	assert(e->state == normal);

	debug("%s: reading `%s'\n", name, e->name);
	vaul_binfile file(file_name, "r");
	file.seek(e->offset);
	vaul_design_unit *du;
	if(e->du == NULL) {
	    du = new vaul_design_unit(e->name);
	    e->attach(du, this);
	} else {
	    du = e->du;
	    retain_ref(e->du);
	}

	int du_label = du->label; // DU might be detached during the READ
	du->read(&file, libs);

	if(du->label == 0) {
	    // DU has in fact been detached, E is now invalid

	    if(!du->is_error() || du->get_error() == vhdlerr_out_of_date) {
		// read the graph of the new unit
		debug("%s: generation gap while reading %s, recursing\n",
		      name, du->get_name());
		for(e = directory; e; e = e->next)
		    if(e->label == du_label) {
			release_ref(du);
			return read_entry(e, libs);
		    }
	    }

	    return du; // return the error
	}

	// see if e is still valid, as it should be
	entry *e2;
	for(e2 = directory; e2; e2 = e2->next)
	    if(e2->du == du)
		break;
	assert(e2 == e);

	debug("%s: done reading `%s'\n", name, e->name);
	release_ref(du);

	e->tree_read = (libs != NULL);
	if(fatal_reading_error(file.get_error())) {
	    debug("%s: error\n", name);
	    e->detach();
	}
    }

    retain_ref(e->du);
    return e->du;
}

vaul_library::entry::entry(char *n, entry **&tail)
{
    state = normal;
    name = xstrdup(n);
    du = NULL;
    tree_read = false;
    next = NULL;
    *tail = this;
    tail = &next;
}

vaul_library::entry::~entry()
{
    detach();
    free(name);
}
    
void vaul_library::entry::attach(vaul_design_unit *du, vaul_library *lib)
{
    retain_ref(du);
    detach();
    this->du = du;
    du->label = label;
    du->lib = lib;
    if(name == NULL || strcmp(name, du->get_name())) {
	if(name)
	    free(name);
	name = xstrdup(du->get_name());
    }
    debug("%s: attached %p (label %d)\n", lib->name, du, du->label);
}

void vaul_library::entry::detach()
{
    if(du) {
	du->label = 0;
	debug("detached %p\n", du);
    }
    release_ref(du);
    du = NULL;
}

void vaul_library::entry::write(vaul_binfile *f)
{
    f->put(name);
    f->put(offset);
    f->put(length);
}

void vaul_library::entry::read(vaul_binfile *f)
{
    f->get(name);
    f->get(offset);
    f->get(length);
}

vaul_library::entry *vaul_library::lib_entry(char *n)
{
    for(entry *e = directory; e; e = e->next)
	if(vaul_name_eq(e->name, n))
	    return e;

    entry *new_e = new entry(n, dir_tail);
    new_e->label = next_label++;
    return new_e;
}

void vaul_library::read_dir(vaul_binfile *f)
{
    int off;
    f->seek(0);
    f->get(off);
    f->seek(off);

    int n;
    f->get(n);
    while(!f->is_error() && --n >= 0) {
	entry *e = new entry(NULL, dir_tail);
	e->read(f);
	e->label = next_label++;
	debug("%s: %s has label %d\n", name, e->name, e->label);
    }
}

void vaul_library::write_dir()
{
    new_file->put(next_label-1);
    for(entry *e = directory; e; e = e->next)
	e->write(new_file);
}

vaul_lib_stats *vaul_library::get_stats()
{
    vaul_lib_stats *st = new vaul_lib_stats;

    if(!start_reading()) {
	st->set_error(this);
	return st;
    }

    debug("%s: getting stats\n", name);
    vaul_binfile file(file_name, "r");
    if(file.is_error()) {
	st->set_error(file);
	return st;
    }

    for(entry *e = directory; e; e = e->next) 
	st->n_entries++;
    st->stats = new vaul_lib_stats::stat[st->n_entries];
    int i = 0;
    for(entry *_e = directory; _e; _e = _e->next) {
	st->stats[i].name = xstrdup(_e->name);
	st->stats[i].du = read_entry(_e, NULL);
	i++;
    }
    return st;
}

vaul_lib_stats::vaul_lib_stats()
{
    n_entries = 0;
    stats = NULL;
}

vaul_lib_stats::~vaul_lib_stats()
{
    for(int i = 0; i < n_entries; i++) {
	free(stats[i].name);
	release_ref(stats[i].du);
    }
    delete[] stats;
}

vaul_libpool::vaul_libpool(int to)
{
    timeout = to;
    libs = NULL;
    cur_ses = NULL;
}

vaul_libpool::~vaul_libpool()
{
    lib_entry *l, *l2;
    for(l = libs; l; l = l2) {
	l2 = l->link;
	l->lib->release();
	delete l;
    }
    session *s, *s2;
    for(s = cur_ses; s; s = s2) {
	s2 = s->prev;
	delete s;
    }
}

vaul_library *vaul_libpool::get(char *n)
{
    lib_entry *l;
    for(l = libs; l; l = l->link)
	if(vaul_name_eq(l->lib->get_name(), n))
	    break;

    if(l == NULL) {
	l = new lib_entry;
	l->lib = new_vaul_library(n);
	l->link = libs;
	libs = l;
	if(cur_ses)
	    l->lib->freeze();
    }
    set_error(l->lib, "%s: ", l->lib->get_name());
    retain_ref(l->lib);
    return l->lib;
}

vaul_library *vaul_libpool::new_vaul_library(char *n)
{
    return new vaul_library(n, timeout);
}

vaul_design_unit *vaul_libpool::get(char *ln, char *un, bool nt)
{
    vaul_library *lib = get(ln);
    vaul_design_unit *du = lib->get(un, nt? NULL : this);
    set_error(lib, "%s: ", lib->get_name());
    lib->release();
    if(du && cur_ses)
	cur_ses->add_used(du->get_name());
    return du;
}

vaul_design_unit *vaul_libpool::get(char *ln, int un, bool nt)
{
    vaul_library *lib = get(ln);
    vaul_design_unit *du = lib->get(un, nt? NULL : this);
    set_error(lib, "%s: ", lib->get_name());
    lib->release();
    if(du && cur_ses)
	cur_ses->add_used(du->get_name());
    return du;
}

void vaul_libpool::flush()
{
    for(lib_entry **l = &libs; *l;) {
	vaul_library *lib = (*l)->lib;
	if(lib->ref_count == 1) {
	    lib->release();
	    *l = (*l)->link;
	} else
	    l = &(*l)->link;
    }
}

vaul_library *vaul_libpool::get_worklib()
{
    return cur_ses? cur_ses->worklib : NULL;
}

bool vaul_libpool::begin_session(char *worklib_name)
{
    vaul_library *wlib;
    if(worklib_name)
	wlib = get(worklib_name);
    else {
	wlib = new vaul_library(NULL, timeout);
	set_error(wlib, "%s: ", wlib->get_name());
    }
    if(is_error()) {
	release_ref(wlib);
	return false;
    }
    if(cur_ses)
        deactivate(cur_ses);
    cur_ses = new session(wlib, cur_ses);
    activate(cur_ses);
    return true;
}

void vaul_libpool::end_session(bool abort)
{
    if(cur_ses == NULL) {
	set_error(vhdlerr_usage, "no active session");
	return;
    }

    if(is_error() || cur_ses->worklib->is_error())
	abort = true;
    if(abort)
	cur_ses->worklib->abort();
    else
	cur_ses->worklib->flush();
    set_error(cur_ses->worklib, "%s: ", cur_ses->worklib->get_name());

    deactivate(cur_ses);
    session *prev = cur_ses->prev;
    delete cur_ses;
    cur_ses = prev;
    if(cur_ses)
	activate(cur_ses);
}

bool vaul_libpool::inserted_in_this_session(vaul_design_unit *du)
{
    return cur_ses? cur_ses->worklib->is_inserted(du) : false;
}

bool vaul_libpool::used_in_this_session(vaul_design_unit *du)
{
    return cur_ses? cur_ses->is_used(du->get_name()) : false;
}

void vaul_libpool::activate(session *s)
{
    for(lib_entry *le = libs; le; le = le->link)
	if(le->lib != s->worklib)
	    le->lib->freeze();
}

void vaul_libpool::deactivate(session *s)
{
    for(lib_entry *le = libs; le; le = le->link)
	if(le->lib != s->worklib)
	    le->lib->unfreeze();
}

vaul_libpool::session::session(vaul_library *w, session *p)
{
    used = NULL;
    n_used = 0;
    retain_ref(worklib = w);
    prev = p;
}

vaul_libpool::session::~session()
{
    release_ref(worklib);
    free(used);
}

void vaul_libpool::session::add_used(char *name)
{
    if(!is_used(name)) {
	used = (char **)xrealloc(used, (n_used+1)*sizeof(char **));
	used[n_used] = xstrdup(name);
	n_used++;
    }
}

bool vaul_libpool::session::is_used(char *name)
{
    for(int i = 0; i < n_used; i++)
	if(vaul_name_eq(used[i], name))
	    return true;
    return false;
}

char *vaul_libpool::package_body_name(const char *p)
{
    return aprintf(".%s", p);
}

char *vaul_libpool::architecture_name(const char *e, const char *a)
{
    return aprintf("%s(%s)", e, a);
}
