/* locking

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

#ifndef VAUL_LOCK_H
#define VAUL_LOCK_H

#include <vaul/errors.h>

#include <stdio.h>
#include <unistd.h>

struct vaul_binfile;

enum vaul_lock_mode {
    vhdllock_none,
    vhdllock_shared,
    vhdllock_exclusive
};

struct vaul_lock : vaul_error_source {

    vaul_lock(int fd, vaul_lock_mode = vhdllock_none, int to = 0);
    vaul_lock(FILE *f, vaul_lock_mode = vhdllock_none, int to = 0);
    ~vaul_lock();

    int set(vaul_lock_mode mode, int timeout = 0);
    vaul_lock_mode get();

    int is_locked();
    int is_exclusive();

private:
    int fd;
    vaul_lock_mode mode;
    void init(int fd, vaul_lock_mode, int to);
    void init_stream(FILE *f, vaul_lock_mode, int to);
};

#endif // VAUL_LOCK_H
