/* reading and writing binary data
 
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

#include <vaul/binfile.h>
#include <vaul/lock.h>
#include <vaul/tree.h>
#include <vaul/design_unit.h>
#include <vaul/library.h>
#include <vaul/debug.h>

#include <stdio.h>
#include <vaulutil.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <assert.h>
#include <stream.h>

vaul_binfile::vaul_binfile()
{
    f = NULL;
    name = NULL;
}

vaul_binfile::vaul_binfile(char *n, char *mode)
{
    f = NULL;
    name = NULL;
    set(n, mode);
}

bool vaul_binfile::set(char *n, char *mode)
{
    debug("opening %s %s\n", n, mode);

    if(f)
	fclose(f);
    clear_error();
    if(name != n) {
	free(name);
	name = xstrdup(n);
    }

    while(!(f = fopen(name, mode)) && errno == ENOENT)
	if(!strcmp(mode, "r+"))
	    mode = "w+";
	else
	    break;
    if(!f) {
	set_error("%s: ", name);
	return false;
    }

    return true;
}

bool vaul_binfile::set(char *mode)
{
    return set(name, mode);
}

vaul_binfile::~vaul_binfile()
{
    free(name);
    if(f)
	fclose(f);
}

bool vaul_binfile::seek(long off, int whence)
{
    if(f == NULL)
	return false;
    if(fseek(f, off, whence) < 0) {
	set_error("%s: ", name);
	return false;
    }
    return true;
}

long vaul_binfile::tell()
{
    if(f == NULL)
	return -1;
    long off = ftell(f);
    if(off < 0)
	set_error("%s: ", name);
    return off;
}

bool vaul_binfile::stat(struct stat &st)
{
    if(f == NULL)
	return false;
    if(fstat(fileno(f), &st) < 0) {
	set_error("%s: ", name);
	return false;
    }
    return true;
}

void vaul_binfile::put(void *mem, size_t n)
{
    if(n == 0)
	return;
    if(f && fwrite(mem, n, 1, f) != 1)
	set_error("%s: ", name);
}

void vaul_binfile::put(char *s)
{
    if(s == NULL) {
	put((unsigned char)(UCHAR_MAX-1));
	return;
    }

    int l = strlen(s);
    if(l < UCHAR_MAX-1)
	put((unsigned char)l);
    else {
	put((unsigned char)UCHAR_MAX);
	put(l);
    }
    put(s, l);
}

void vaul_binfile::put(vaul_binfile *bf, int l)
{
    if(l == 0 || f == NULL || bf->f == NULL)
	return;

    char buf[1024];
    int n;
    while((n = fread(buf, 1, l>1024? 1024:l, bf->f)) > 0) {
	l -= n;
	if(fwrite(buf, 1, n, f) != n) {
	    set_error("%s: ", name);
	    return;
	}
    }
    if(n < 0) 
	bf->set_error("%s: ", name);
    else if(l > 0)
	bf->set_error(vhdlerr_eof, "%s: EOF encountered", name); 
}

void vaul_binfile::get(void *mem, size_t n)
{
    if(n == 0)
	return;

    if(f && fread(mem, n, 1, f) != 1) {
	if(feof(f))
	    set_error(vhdlerr_eof, "%s: EOF encountered", name);
	else
	    set_error("%s: ", name);
    }
}

void vaul_binfile::get(char *&s)
{
    s = NULL;

    int l;
    unsigned char ucl;
    get(ucl);
    if(ucl == UCHAR_MAX-1)
	return;
    if(ucl == UCHAR_MAX)
	get(l);
    else
	l = ucl;
    if(is_error())
	return;

    s = (char *)xmalloc(l+1);
    get(s, l);
    s[l] = '\0';
}

bool vaul_binfile::flush()
{
    if(f == NULL)
	return false;
    if(fflush(f) < 0) {
	set_error("%s: ", name);
	return false;
    }
    return true;
}

bool vaul_binfile::is_eof()
{
    if(!f || is_error())
	return true;
    int c = fgetc(f);
    ungetc(c, f);
    return c == EOF;
}

#if 0
ostream &ostream::operator << (bool b)
{
    return *this << int(b);
}
#endif
