/* reading and writing of binary data

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

#ifndef VAUL_BINFILE_H
#define VAUL_BINFILE_H

#include <vaul/errors.h>
#include <vaul/mempool.h>

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

struct vaul_design_unit;
struct vaul_libpool;
struct vaul_graph_runner;

// vaul_binfile kapselt ein FILE in eine Klasse und hält sich an ein
// 'locking protocol' um Inkonsistenzen zwischen mehreren Programmen
// zu vermeiden. <timeout> ist die Auszeit in Sekunden nach der Versuche
// den Lock zu erlangen abgebrochen werden.
//
struct vaul_binfile : vaul_error_source {

    vaul_binfile();
	// erzeugt ein 'nutzloses' Objekt, kann später mit
	// set() aktiviert werden.

    vaul_binfile(char *name, char *mode);
	// öffnet Datei mit Namen <name> und Modus <mode> (wie
	// 'fopen(name, mode)').
	// Ist <mode> "r+" und die Datei existiert nicht, wird sie
	// angelegt.

    ~vaul_binfile();

    char *get_name()	    { return name; }

    bool set(char *name, char *mode);
	// ignoriert den Fehlerstatus und initialisiert alles neu
	// (wie der Konstruktor).

    bool set(char *mode);
	// wie 'set(get_name(), mode)', d.h. der Stream wird geschlossen
        // und wieder geöffnet.

    bool seek(long offset, int whence = 0);
	// wie fseek(offset, whence, ...)

    long tell();
	// wie ftell(...)

    bool stat(struct stat &);
	// wie stat(...);

    void put(void *mem, size_t n);
	// Schreibt <n> Bytes ab Adresse <mem> in die Datei.

    void get(void *mem, size_t n);
	// Liest <n> Bytes aus der Datei in den Speicher ab Adresse <mem>.
	// Es ist ein Fehler, wenn die Datei kürzer ist als nötig.

    void put(char *x);
    // template <class T> put(T x);
	// Schreibt <x> in die Datei

    void put(vaul_binfile *f, int len);
	// Liest <len> bytes aus <f> und schreibt sie in die Datei.

    void get(char *&x);
    // template <class T> get(T &x);
	// Liest <x> aus der Datei

    bool flush();
	// wie fflush()

    bool is_eof();
	// Ermittelt, ob der nächste Lesevorgang ans Dateiende
	// stossen würde. Wenn vorher schon ein Fehler aufgetreten ist,
	// zählt das auch als 'Datei ist zu ende'.

    // C++ kann 'templates' in Klassen nicht verkraften.
    //	Also, sowas :-(
    #define _define_put(t) void put(t x) { put(&x, sizeof(x)); }
    #define _define_get(t) void get(t &x) { get(&x, sizeof(x)); }
    #define _define_pg(t) _define_put(t) _define_get(t)
    #define _define_pg_su(t) _define_pg(signed t) _define_pg(unsigned t)
    _define_pg_su(int)
    _define_pg_su(short)
    _define_pg_su(long)
    _define_pg_su(char)
    _define_pg(char)
    _define_pg(double)
    _define_pg(float)
    _define_pg(long double)
    _define_pg(bool)
    #undef _define_pg_su
    #undef _define_pg
    #undef _define_get
    #undef _define_put

private:
    // friend struct vaul_graph_runner;

    FILE *f;
    char *name;
};

#endif // VAUL_BINFILE_H
