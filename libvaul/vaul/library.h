/* design librarie management

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

#ifndef VAUL_LIBRARY_H
#define VAUL_LIBRARY_H

#include <vaul/common.h>
#include <vaul/errors.h>
#include <vaul/lock.h>
#include <vaul/binfile.h>
#include <vaul/design_unit.h>
#include <vaul/xmap.h>

#include <time.h>

struct vaul_lib_stats;
struct vaul_library;

struct sStandardPackage;

//  Während VHDL-Libraries bearbeitet werden (einfügen, lesen), sind
//  sie halb auf Platte und halb im Speicher, deshalb müssen die Zugriffe
//  koordiniert werden. Innerhalb eines Programms wird ein vaul_libpool
//  angelegt, der sicherstellt, das es für jede Library auf Platte nur ein
//  Object im Speicher gibt, das sie repräsentiert. Zwischen mehreren
//  Programmen sorgt file locking dafür, daß niemand die Library in
//  einem inkonsistenten Zustand antrifft.
//
//  VHDL-Libraries sind entweder 'Working Libraries' oder
//  'Resource Libraries' (in die nicht eingefügt werden darf).

// Zu 'Referenzen' siehe "vhdl/ref.h"

//  Ein vaul_libpool ist die einfachste und empfohlene Methode an
//  vaul_libraries und vaul_design_units zu kommen.
//  Zu Namen von vaul_libraries siehe 'struct vaul_library', zu Namen
//  von vaul_design_units siehe "vhdl/design_unit.h"
//
struct vaul_libpool : vaul_error_source {

    vaul_libpool(int timeout = 30);
    virtual ~vaul_libpool();

    int timeout;
        // Die Auszeit in Sekunden für alle Operationen mit vaul_libraries,
	// kann jederzeit geändert werden.

    virtual
    vaul_library *get(char *name);
	// liefert die vaul_library <lib> mit Namen <name>.
	// <lib> ist immer !=NULL, kann aber eine Fehlermeldung produziert
	// haben. Übernimmt den Fehler von <lib>.
	// <lib> ist eine Referenz.

    virtual
    vaul_design_unit *get(char *libname, char *unitname, bool no_tree = false);
	// liefert die vaul_design_unit <du> mit Namen <unitname> aus der
	// vaul_library <libname>. Siehe 'vaul_library::get'.
	// <this> übernimmt den Fehler von vaul_library <libname>.
	// Wenn 'no_tree == true' ist, wird der eigentliche Baum nicht gelesen.

    virtual
    vaul_design_unit *get(char *libname, int unitnum, bool no_tree = false);
	// dito, nur mit Nummer

    virtual
    vaul_library *get_worklib();
	// eine ausgewählte Library, in die gerade eingefügt wird.
	// Siehe vaul_libpool::insert, vaul_design_file::insert
	// <worklib> ist eine Referenz.

    virtual
    bool begin_session(char *name = NULL);

    virtual
    bool inserted_in_this_session(vaul_design_unit *);
    virtual
    bool used_in_this_session(vaul_design_unit *);

    virtual
    void end_session(bool abort = false);

    virtual
    void flush();
	// Alle vaul_libraries in diesem pool, auf die niemand mehr
	// eine Referenz hat, werden beseitigt.

    const char *get_version_string();
        // Gibt die Versionskennung zurück, z.B. "0.5"

    virtual char *package_body_name(const char *package);
    virtual char *architecture_name(const char *entity, const char *arch);

protected:
    virtual
    vaul_library *new_vaul_library(char *name);

    struct session {
	session(vaul_library *worklib, session *prev);
	~session();

	session *prev;
	vaul_library *worklib;

	void add_used(char *name);
	bool is_used(char *name);

    private:
	char **used;
	int n_used;
    };
    session *cur_ses;

    virtual
    void activate(session *);

    virtual
    void deactivate(session *);

    struct lib_entry {
	lib_entry *link;
	vaul_library *lib;
    };
    lib_entry *libs;

};

//  vaul_libraries verwalten Sammlungen von vaul_design_units auf Platte
//  und im Speicher.
//  Die Namen von vaul_libraries sind die Namen aus der 'library clause'
//  von VHDL, auf Platte können sie ganz anders heissen.
//  Alle Auszeiten sind in Sekunden.
//
struct vaul_library : vaul_error_source, vaul_ref {

    vaul_library(char *name, int timeout);
	// Legt eine neue vaul_library mit Namen <name> an.
	// Wenn <name> == NULL, wird eine unbenannte vaul_library
	// angelegt, die nie auf Platte erscheint.
	// Andernfalls:
	//  Falls eingefügt werden soll, darf es nur eine
	// einzige vaul_library <name> pro Programm geben, also 
	// vaul_libpool benutzen. Es wird versucht ein "shared lock"
	// für die entsprechende Datei zu bekommen, so daß Schreib-
	// versuche von Programmen (die sich an das Locking Protocol
	// halten) scheitern. Es ist kein Fehler, wenn es die Datei
	// auf Platte nicht gibt.

    virtual
    ~vaul_library();

    int timeout;

    virtual 
    char *get_name()		{ return name; }

    virtual
    char *get_file_name()	{ return file_name; }
	// Der Dateiname auf Platte

    virtual
    bool insert(vaul_design_unit *du);
	// Fügt <du> in die vaul_library ein und gibt Erfolgs-
	// meldung zurück.
	// Alle neu eingehenden vaul_design_units werden in eine
	// Zwischendatei geschrieben und von 'flush' endgültig in die
	// Datei eingefügt, sodaß andere Programme <du> erst nach 'flush'
	// sehen, 'get' findet <du> aber sofort nach 'insert'.

    virtual
    bool is_inserted(vaul_design_unit *du);

    virtual
    bool flush();
	// Macht alle Einfügungen permanent und gibt Erfolgsmeldung
	// zurück. 'flush' (oder 'abort') muß aufgerufen werden,
	// nachdem eingefügt wurde.
	// Alle vaul_design_units, die nicht durch Neue ersetzt
	// wurden, werden aus der originalen Datei in die Zwischendatei
	// übertragen. Die Zwischendatei wird dann in die originale um-
	// benannt. Dabei wird versucht ein "exclusive lock" für die
	// originale Datei zu bekommen.
	// Wenn alles gutging, wird <this> neu initialisiert.

    virtual
    bool abort();
	// Ignoriert den Fehlerstatus, verwirft alle Änderungen und
	// löscht die Zwischendatei.
	// Dann wird <this> neu initialisiert.

    virtual
    vaul_lib_stats *get_stats();
	// liefert Statistiken zu <this>, niemals NULL.

    virtual
    vaul_design_unit *get(char *name, vaul_libpool *pool);
	// liefert vaul_design_unit <name>, falls vorhanden, oder
	// NULL wenn nicht und im Fehlerfall.
	// vaul_libraries, die von vaul_design_unit <name> gebraucht
	// werden, werden über <pool> geladen. Wenn <pool == NULL> ist
	// wird der Baum von <name> nicht geladen, wohl aber die
	// anderen Sachen (wie sourcename, ctime, ...).

    virtual
    vaul_design_unit *get(int label, vaul_libpool *);
	// dito, Identifikation über Index.

    vaul_xmap libmap;
	// Zuordnung zwischen Namen und Nummern für vaul_library Namen
	// für Speicherung von Referenzen auf externe vaul_design_units.

    virtual void freeze();
    virtual void unfreeze();

protected:
    virtual
    char *build_filename(char *name);

private:
    char *name, *file_name;
    vaul_binfile *new_file;

    void init();

    bool start_reading();
    bool start_writing();
    bool fatal_reading_error(int);

    enum entry_state { normal, inserted };
    struct entry {
	entry(char *name, entry **&tail);
	~entry();

	entry *next;
	entry_state state;

	char *name;
	int offset, length;
	int label;

	vaul_design_unit *du;
	bool tree_read;

	void attach(vaul_design_unit *du, vaul_library *lib);
	void detach();
	void write(vaul_binfile *);
	void read(vaul_binfile *);
    } *directory, **dir_tail;
    int next_label;

    int temperature;

    entry *lib_entry(char *);
    void attach(vaul_design_unit *du, entry *e);
    void detach(entry *e);
    void read_dir(vaul_binfile *);
    void write_dir();
    vaul_design_unit *read_entry(entry *, vaul_libpool *);
};

//  vaul_lib_stats enthält Statistiken über eine vaul_library
//  Wird bestimmt noch erweitert.
//
struct vaul_lib_stats : vaul_error_source {
    friend vaul_library;

    vaul_lib_stats();
    ~vaul_lib_stats();

    int n_entries;
    struct stat {
	char *name;
	vaul_design_unit *du;	// <du->tree == NULL> ist möglich, obwohl
				// <du> eigentlich einen Baum hat.
    };
    stat *stats;
	// für jede vaul_design_unit ein Eintrag, insgesamt <n_entries> Stück.
};

#endif // VAUL_LIBRARY_H
