/* locking files

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

#include <vaul/lock.h>
#include <vaul/binfile.h>

#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

vaul_lock::vaul_lock(int fd, vaul_lock_mode m, int to)
{
	init(fd, m, to);
}

vaul_lock::vaul_lock(FILE *f, vaul_lock_mode m, int to)
{
	init_stream(f, m, to);
}

void vaul_lock::init_stream(FILE *f, vaul_lock_mode m, int to)
{
	init(f? fileno(f) : -1, m, to);
}

void vaul_lock::init(int fd, vaul_lock_mode m, int to)
{
	this->fd = fd;
	mode = vhdllock_none;
	if(m != mode)
		set(m, to);
}

vaul_lock::~vaul_lock()
{
	set(vhdllock_none);
}

static void ignore_alarm(int)
{
//	printf("Alarm went off\n");
}

static short mode_map[] = {
	F_UNLCK,
	F_RDLCK,
	F_WRLCK
};

int vaul_lock::set(vaul_lock_mode m, int to)
{
	struct flock fl = {
		mode_map[m],
		SEEK_SET,
		0, 0
	};

	clear_error();

	signal(SIGALRM, ignore_alarm);
	alarm(to);
	if(fcntl(fd, F_SETLKW, &fl) < 0) {
		if(errno == EINTR)
			set_error(vhdlerr_timeout, "time out after %ds", to);
		else
			set_error();
	} else
		mode = m;
	alarm(0);
}

vaul_lock_mode vaul_lock::get()
{
	return mode;
}

int vaul_lock::is_locked()
{
	return mode != vhdllock_none;
}

int vaul_lock::is_exclusive()
{
	return mode == vhdllock_exclusive;
}
