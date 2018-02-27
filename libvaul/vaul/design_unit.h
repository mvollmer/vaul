/* a design unit represented as an abstract graph

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

#ifndef VAUL_DESIGN_UNIT_H
#define VAUL_DESIGN_UNIT_H

#include <vaul/ref.h>
#include <vaul/mempool.h>
#include <vaul/errors.h>

#include <time.h>

struct vaul_binfile;
struct vaul_libpool;
struct vaul_library;
struct vaul_graph_reader;
struct sScope;
struct sNode;

//  vaul_design_unit repräsentiert eine VHDL Design Unit als
//  abstrakten Baum (siehe "tree.t") und kann ihn auch
//  Lesen und Speichern.
//
struct vaul_design_unit : vaul_error_source, vaul_ref {

    vaul_design_unit(char *name = NULL, char *source_filename = NULL);
	// Namen von Design Units sind bei 'entity', 'package' und
	// 'configuration' die Namen aus dem VHDL-Text, bei 'architecture'
	// in der Form "ent_name(arch_name)" und bei 'package body' in der
	// Form ".pack_name".

    ~vaul_design_unit();

    char *get_name()		{ return name; }
    void set_name(char *name);
	// setzt den Namen nur, wenn er vorher NULL war.

    void set_ctime();

    char *get_source()		{ return source; }
    time_t get_ctime()		{ return ctime; }

    bool is_older_than(vaul_design_unit *du);

    sScope *tree;
	// Der abstrakte Baum.

    vaul_library *lib;
    int label;
	// Beschreiben die 'vaul_library' in der 'this' ist (wenn überhaupt)
	// und die Position im Inhaltsverzeichnis (zum Speichern von
	// Referenzen)

    void uses(vaul_design_unit *du);
	// Merkt sich 'du', sodaß 'du' nicht verschwindet, solange
	// 'this' existiert.

    sNode *find_node(int label);
	// Durchsucht 'tree' nach dem Knoten mit Nummer 'label'.
        // Funktioniert nur, wenn sich der Baum seit dem letzten
        // Suchen nicht geändert hat.

    vaul_memregion mem;
	// Hält den Speicher von 'tree'.

    void write(vaul_binfile *f);
	// Schreibt 'tree' nach 'f', Fehlermeldungen tauchen in 'f' auf.

    void read(vaul_binfile *f, vaul_libpool *p);
	// Liest aus 'f', Referenzen werden durch 'vaul_libraries'
	// aus 'p' aufgelöst. Wenn 'p == NULL' ist wird 'tree' nicht 
	// gelesen. Fehlermeldungen tauchen in 'f' auf.

    void print();
	// Gibt 'tree' zusammen mit 'name', ... nach 'stdout' aus.

    vaul_design_unit *find_du(sNode *n);
	// sucht die 'vaul_design_unit', die 'n' enthält. Es muß eine
	// von den durch 'uses' gemerkten sein, oder 'this'.

    vaul_design_unit *query_used_dus(vaul_design_unit *);

private:
    char *name;
    char *source;
    time_t ctime;
    struct du_entry {
	du_entry *link;
	vaul_design_unit *du;
    } *used_dus;
    vaul_graph_reader *reader;
};

#endif // VAUL_DESIGN_UNIT
