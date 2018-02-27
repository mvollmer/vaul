// -*- c++ -*- $Id: sample.tree,v 1.2 1996/06/07 19:39:28 mvo Exp $

#ifndef SAMPLE_TREE_H
#define SAMPLE_TREE_H

#include <vaulutil.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef int graph_region_id;
const graph_region_id GRAPH_NULL_REGION_ID = -1;

enum nKind {
	nkInvalid, nkLRef, nkXRef,
	nkNode, 
	nkExpr, 
	nkConst, 
	nkBinOp, 
	nkStats, 
	nkStat, 
	nkWhile, 
	nkIf
};

typedef unsigned char nKind_ShortRep;

struct sNode;
typedef sNode *nNode;
struct sStat;
typedef sStat *nStat;
struct sIf;
typedef sIf *nIf;
struct sWhile;
typedef sWhile *nWhile;
struct sStats;
typedef sStats *nStats;
struct sExpr;
typedef sExpr *nExpr;
struct sBinOp;
typedef sBinOp *nBinOp;
struct sConst;
typedef sConst *nConst;
typedef sNode graph_base_node;
typedef nKind graph_node_kind;
/* this file is inserted into the graph header file

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

// The whole graph is devided into several regions. Every node belongs
// to exactly one region. One of the regions is the special `region of
// the homeless nodes' (rohn).

// All regions except the rohn are considered static and each node of
// such a region is identified by a label. Thus, a node is completely
// identified by a region and a label. Nodes of the rohn can not be
// identified at all. Likewise, nodes of the rohn can not be
// systematically visited. They can only be written to a file and
// thereby become ordinary 'homed' nodes.


#if 0
// identifies a region. All values denote valid regions, including
// NULL which denotes the special `region of the homeless nodes'.
//
typedef blah graph_region_id; // defined by user

struct graph_base_node {

    graph_base_node()   { label = -1 }
    ~graph_base_node()  { }

    graph_region_id get_region_id();     // implemented by user
    void set_region_id(graph_region_id); // implemented by user

    int get_label()     { return label; }

private:
    friend struct graph_runner;
    int label;
};
#endif

struct graph_marker {

    graph_marker(graph_region_id);
    ~graph_marker();

    bool marked(graph_base_node *);
    void mark(graph_base_node *);

    graph_base_node *find_node(int label);

protected:
    graph_region_id region_id;

private:
    int n_marks, max_marks;
    graph_base_node **marks;
};

struct graph_visitor;
typedef void graph_visit_f(graph_base_node *, graph_visitor *);

struct graph_visitor : private graph_marker {

    graph_visitor(graph_region_id);
    ~graph_visitor();

    void visit(graph_visit_f *pre,
	       graph_visit_f *post,
	       graph_base_node *start);
};

struct graph_reader : private graph_marker {

    graph_reader(graph_region_id);
    ~graph_reader();

    graph_base_node *read(graph_node_kind expected);

    graph_base_node *find_node(int label);

private:
    virtual graph_node_kind read_kind() = 0;
    virtual graph_base_node *read_xref() = 0;
    virtual int read_label() = 0;
    virtual void read_error(graph_node_kind read,
			    graph_node_kind expected) = 0;
};

struct graph_writer : private graph_marker {

    graph_writer(graph_region_id);
    ~graph_writer();

    void write(graph_base_node *head);

private:
    virtual void write_kind(graph_node_kind);
    virtual void write_xref(graph_base_node *);
    virtual void write_label(int);
};

struct graph_printer : private graph_marker {

    graph_printer(graph_region_id);
    ~graph_printer();

    void print(graph_base_node *);

    void print_lab(const char *lab);

private:
    virtual void print_lref(graph_base_node *);
    virtual void print_xref(graph_base_node *);
    virtual void print_header(graph_base_node *);
    virtual void print_string(const char *);

    int level;
};
typedef void graph_visit_f(graph_base_node *, graph_visitor *);

struct sNode {

	sNode()
	{
		this->label = -1;
		set_region_id(GRAPH_NULL_REGION_ID);
	}

 	int region;
	graph_region_id get_region_id() { return region; }
	void set_region_id(graph_region_id id) { region = id; }
	int label;

	virtual nKind kind();
	char *type_id();
	static char *type_id(nKind);
	bool is(nKind k);
	nKind base_kind() { return base_kind(kind()); }
	static nKind base_kind(nKind);
	static bool is_base_of(nKind b, nKind d);
	static nNode create(nKind, void *placement_hint);
	int get_label() { return label; }

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sStat : sNode {

	sStat(int lineno)
	  : sNode()
	{
		this->lineno = lineno;
	}
	sStat() : sNode() {

}

	int lineno;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sIf : sStat {

	sIf(int lineno, sExpr *cond, sStats *then_stats, sStats *else_stats)
	  : sStat(lineno)
	{
		this->cond = cond;
		this->then_stats = then_stats;
		this->else_stats = else_stats;
	}
	sIf() : sStat() {

}

	nExpr cond;
	nStats then_stats;
	nStats else_stats;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sWhile : sStat {

	sWhile(int lineno, sExpr *cond, sStats *stats)
	  : sStat(lineno)
	{
		this->cond = cond;
		this->stats = stats;
	}
	sWhile() : sStat() {

}

	nExpr cond;
	nStats stats;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sStats : sNode {

	sStats(sStats *next, sStat *stat)
	  : sNode()
	{
		this->next = next;
		this->stat = stat;
	}
	sStats() : sNode() {

}

	nStats next;
	nStat stat;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sExpr : sNode {

	sExpr()
	  : sNode()
	{
	}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sBinOp : sExpr {

	sBinOp(char op, sExpr *left, sExpr *right)
	  : sExpr()
	{
		this->op = op;
		this->left = left;
		this->right = right;
	}
	sBinOp() : sExpr() {

}

	char op;
	nExpr left;
	nExpr right;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sConst : sExpr {

	sConst(double val, bool buffer)
	  : sExpr()
	{
		this->val = val;
		this->buffer = buffer;
	}
	sConst() : sExpr() {

}

	double val;
	bool buffer;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

nStats reverse(nStats &);

union nTree {
	nNode Node;
	nStat Stat;
	nIf If;
	nWhile While;
	nStats Stats;
	nExpr Expr;
	nBinOp BinOp;
	nConst Const;
};


#endif // SAMPLE_TREE_H

