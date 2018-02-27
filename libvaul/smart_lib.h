// -*- C++ -*- $Id: smart_lib.h,v 1.2 1996/02/15 22:03:38 mvo Exp $

/* Neunmalkluge Design Library.
**
** Design Units in "smart_lib"s werden automatisch durch
** analysieren ihrer Quelltexte aufgefrischt, wenn sie ver-
** altet sind.
**
** Die Dateinamen der Libraries sind "vlib/libname/.cache" um
** die automatische Verwaltung der Graphen auf Platte zu betonen.
*/

#ifndef SMART_LIB_H
#define SMART_LIB_H

#include <vaul/vaul.h>

enum smart_library_flags {
    slf_smart = 0x0001,
    slf_verbose = 0x0002,
};

struct smart_library : vaul_library {

    smart_library(char *name, int flags, int timeout = 5);

    vaul_design_unit *get(char *name, vaul_libpool *pool);
    vaul_design_unit *get(int  index, vaul_libpool *pool);

protected:

    char *build_filename(char *n);

    int flags;
    bool analyze(char *fn, vaul_libpool *);
};

/* Der passende Library Pool
*/

struct smart_libpool : vaul_libpool {

    smart_libpool(int flags, int timeout = 5) 
	: vaul_libpool(timeout)
    {
	this->flags = flags;
    }

    int flags;

protected:
    vaul_library *new_vaul_library(char *name)
    {
	return new smart_library(name, flags, timeout);
    }

    char *package_body_name(const char *package);
    char *architecture_name(const char *entity, const char *arch);
};

#endif // !SMART_LIB_H
