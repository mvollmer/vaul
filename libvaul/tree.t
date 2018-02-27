/* definition of the abstract graph nodes

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

#ifndef VAUL_TREE_H
#define VAUL_TREE_H

#include <stdio.h>
#include <stddef.h>
#include <vaulutil.h>

#include <vaul/errors.h>
#include <vaul/mempool.h>
#include <vaul/printer.h>

struct vaul_binfile;
struct vaul_design_unit;
struct vaul_libpool;
struct vaul_error_printer;
struct vaul_decl_set;

/* Typen für Attribute
*/
typedef char *Id;
typedef char *Literal;

#define _enum(e) typedef unsigned char e; enum

_enum(RangeDirection) {
    DirectionUp = 'u',
    DirectionDown = 'd'
};

_enum(ObjectClass) {
    ObjClass_None = '-',
    ObjClass_Signal = 's',
    ObjClass_Variable = 'v',
    ObjClass_Constant = 'c',
    ObjClass_File = 'f'
};

_enum(Mode) {
    Mode_None = '-',
    Mode_In = 'i',
    Mode_Out = 'o',
    Mode_InOut = 'x',
    Mode_Buffer = 'b',
    Mode_Linkage = 'l'
};

_enum(SignalKind) {
    SigKind_None = '-',
    SigKind_Bus = 'b',
    SigKind_Register = 'r'
};

struct vaul_id_set {

    vaul_id_set();
    ~vaul_id_set();

    void add(Id);
    void remove(Id);
    void init();

    bool contains(Id);

private:
    Id *ids;
    int n_ids;
};

typedef vaul_design_unit *graph_region_id;
#define GRAPH_NULL_REGION_ID NULL

%%graph

[ \section{Basis der Typenhierarchie} ]

node Node 
[ Alle anderen Knotentypen sind von <Node> abgeleitet; einer muß ja
  mal den Anfang machen.
]
{
    extra {
	vaul_design_unit *owner;
	vaul_design_unit *get_region_id() { return owner; }
	void set_region_id(vaul_design_unit *o) { owner = o; }
    }
[ \begin{desc}{"vaul_design_unit *" \b{owner};}
   Ordnet jedem Knoten die "vaul_design_unit" zu, aus dessen Analyse er
   hervorgegangen ist. Knoten, die von einer "vaul_library" oder einem
   "vaul_lib\-pool" kommen, haben garantiert $<owner> \neq "NULL"$, alle
   anderen können auch $<owner> = "NULL"$ haben.
  \end{desc}
]
};

node PosNode : Node
[ Ausgewählte Knotentypen (fast alle) haben zusätzlich zum <owner> noch
  eine Angabe über die Zeilennummer des VHDL Textes, der zu diesem
  Knoten geführt hat. Sehr nützlich für Fehlermeldungen.
]
{
    attr int lineno;
};

/* Declarationen
*/
include decls.t

/* Anweisungen
*/
include stats.t

/* Was noch übrig ist
*/
include misc.t

%%endgraph

struct vaul_graph_writer : graph_writer, vaul_error_source {

    vaul_graph_writer(vaul_design_unit *, vaul_binfile *);
    ~vaul_graph_writer();

    vaul_binfile *file;
   
private:
    vaul_design_unit *unit_owner;

    void write_kind(nKind k);
    void write_xref(nNode);
    void write_label(int);
};

struct vaul_graph_reader : graph_reader, vaul_error_source {

    vaul_graph_reader(vaul_design_unit *, vaul_binfile *, vaul_libpool *);
    ~vaul_graph_reader();

    vaul_binfile *file;
    vaul_mempool pool;

private:
    vaul_design_unit *unit_owner;
    vaul_libpool *libs;

    nKind read_kind();
    nNode read_xref();
    int read_label();
    void read_error(nKind read, nKind expected);
}; 

struct vaul_graph_printer : graph_printer {

    vaul_graph_printer(vaul_design_unit *);
    ~vaul_graph_printer();

private:
    void print_xref(nNode);
};

#if 0
struct vaul_graph_runner : graph_runner, vaul_error_source {

    vaul_graph_runner();
    ~vaul_graph_runner();

    void reset(nNode head);
    void mark(nNode);
    bool marked(nNode);
    bool same_unit(nNode n);

    void write_kind(nKind k);
    void read_kind(nKind &k);

    void print_xref(nNode);
    void write_xref(nNode);
    void read_xref(nNode &);

    void read_error(nKind, nKind);

    vaul_binfile *file;
    vaul_mempool pool;

    vaul_design_unit *unit_owner;
    vaul_libpool *libs;

    nNode found;
    int label;

    void *closure;

    vaul_design_unit *seen[20];
    int n_seen;
};
#endif

struct vaul_node_printer : vaul_std_printer {

protected:
    virtual void format_out(FILE *, const char *spec, int len, va_list &ap);
    virtual char *get_source(nNode);

    void print_node(FILE *f, nNode n, int arg);
};

#endif // VAUL_TREE_H


%%implementation

#include <vaul/tree.h>
#include <vaul/binfile.h>
#include <vaul/lexer.h>

#include <stdio.h>
#include <stream.h>

#define NEW_NODE_PLACEMENT  (((vaul_graph_reader *)placement_hint)->pool)
#include "tree.m"

#define WRITE_ATTR(x, gr)   (((vaul_graph_writer *)(gr))->file->put(x))
#define READ_ATTR(x, f)	    (((vaul_graph_reader *)(gr))->file->get(x))

#define GRAPH_MARKER_GROW 1024
