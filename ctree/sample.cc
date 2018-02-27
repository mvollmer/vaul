// generated by ctree



#include <stream.h>

#include "sample.h"
#include "sample.m"

void set_region(nNode n, int region)
{
    graph_writer wr(region);
    wr.write(n);
}

void print(const char *lab, nNode n)
{
    printf("%s: ", lab);
    if(n == NULL)
	printf("<nil>\n");
    else {
	printf("region %d\n", n->region);
	graph_printer pr(n->region);
	pr.print(n);
    }
}

main()
{
    nExpr e = mBinOp('*', mConst(2, true), mConst(3.141, false));
    set_region(e, 1);

    nStats s = mStats(NULL, mIf(42, e, NULL, NULL));
    s->next = s;
    nIf(s->stat)->then_stats = s;
    set_region(s, 2);

    print("stats", s);
    print("expr", e);

    cout << "true: " << int(true) << "\n";
}

#ifndef PRINT_ATTR
#define PRINT_ATTR(x, gr) (cout << (x) << "\n")
#endif
#ifndef WRITE_ATTR
#define WRITE_ATTR(x, gr)
#endif

#ifndef READ_ATTR
#define READ_ATTR(x, gr)
#endif

#define GRAPH_INVALID_KIND nkInvalid
#define GRAPH_XREF_KIND nkXRef
#define GRAPH_LREF_KIND nkLRef
/* inserted into graph implementation

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
       Fakult�t f�r Elektrotechnik
       Universit�t Dortmund
       44221 Dortmund, Germany

  or `msipc@nt.e-technik.uni-dortmund.de' any improvements or
  extensions that they make and grant the University of Dortmund
  the unlimited use and distribution of these changes.

  Read README.LICENSING for further information.

*/

#include <assert.h>

#ifndef GRAPH_MARKER_GROW
#define GRAPH_MARKER_GROW 512
#endif

graph_marker::graph_marker(graph_region_id id)
{
    assert(id != GRAPH_NULL_REGION_ID);

    region_id = id;
    n_marks = max_marks = 0;
    marks = NULL;
}

graph_marker::~graph_marker()
{
    delete[] marks;
}

bool graph_marker::marked(graph_base_node *n)
{
    assert(n->get_region_id() != GRAPH_NULL_REGION_ID);

    if(graph_base_node *n2 = find_node(n->label)) {
	assert(n2 == n);
	return true;
    }
    return false;
}

void graph_marker::mark(graph_base_node *n)
{
    assert(n->get_region_id() != GRAPH_NULL_REGION_ID);

    if(n->label < 0)
	n->label = n_marks;

    if(n->label >= max_marks) {
	int new_max = n->label + (GRAPH_MARKER_GROW 
				  - (n->label % GRAPH_MARKER_GROW));
	graph_base_node **new_marks = new (graph_base_node *)[new_max];
	memcpy(new_marks, marks, sizeof(graph_base_node*) * n_marks);
	memset(new_marks+n_marks, 0, 
	       sizeof(graph_base_node*) * (new_max-n_marks));
	delete[] marks;
	marks = new_marks;
	max_marks = new_max;
    }

    assert(marks[n->label] == NULL);
    marks[n->label] = n;
    if(n->label >= n_marks)
	n_marks = n->label+1;
}

graph_base_node *graph_marker::find_node(int label)
{
    if(label >= 0 && label < n_marks)
	return marks[label];
    return NULL;
}


graph_visitor::graph_visitor(graph_region_id id)
    : graph_marker(id)
{
}

graph_visitor::~graph_visitor()
{
}

void graph_visitor::visit(graph_visit_f *pre,
			  graph_visit_f *post,
			  graph_base_node *n)
{
    if(n && n->get_region_id() == region_id && !marked(n)) {
	mark(n);
	if(pre)
	    pre(n, this);
	n->visit(pre, post, this);
	if(post)
	    post(n, this);
    }
}

graph_printer::graph_printer(graph_region_id id)
    : graph_marker(id)
{
    level = 0;
}

graph_printer::~graph_printer()
{
}

void graph_printer::print(graph_base_node *n)
{
    if(n == NULL) {
	print_string("<nil>\n");
    } else if(n->get_region_id() != region_id) {
	print_xref(n);
	print_string("\n");
    } else if(!marked(n)) {
	mark(n);
	print_header(n);
	level++;
	n->print(this);
	level--;
    } else {
	print_lref(n);
	print_string("\n");
    }
}

void graph_printer::print_xref(graph_base_node *n)
{
    printf("xref to %p,%d (%s)", 
	   (void *)(n->get_region_id()), n->get_label(), n->type_id());
}

void graph_printer::print_lref(graph_base_node *n)
{
    printf("%s[%d]", n->type_id(), n->get_label());
}

void graph_printer::print_header(graph_base_node *n)
{
    printf("%s[%d]:\n", n->type_id(), n->get_label());
}

void graph_printer::print_string(const char *str)
{
    printf("%s", str);
}

void graph_printer::print_lab(const char *lab)
{
    for(int i = 0; i < level; i++)
	print_string(" ");
    print_string(lab);
    print_string(": ");
}


graph_writer::graph_writer(graph_region_id id)
    : graph_marker(id)
{
}

graph_writer::~graph_writer()
{
}

void graph_writer::write(graph_base_node *n)
{
    if(n == NULL) {
	write_kind(GRAPH_INVALID_KIND);
	return;
    }

    if(n->get_region_id() == GRAPH_NULL_REGION_ID)
	n->set_region_id(region_id);
    else if(n->get_region_id() != region_id) {
	write_kind(GRAPH_XREF_KIND);
	write_xref(n);
	return;
    }

    if(!marked(n)) {
	mark(n);
	write_kind(n->kind());
	n->write(this);
    } else {
	write_kind(GRAPH_LREF_KIND);
	write_label(n->label);
    }
}

void graph_writer::write_kind(graph_node_kind k)
{
}

void graph_writer::write_xref(graph_base_node *n)
{
}

void graph_writer::write_label(int l)
{
}


graph_reader::graph_reader(graph_region_id id)
    : graph_marker(id)
{
}

graph_reader::~graph_reader()
{
}

graph_base_node *graph_reader::read(graph_node_kind expected)
{
    graph_node_kind k;
    graph_base_node *n;

    k = read_kind();

    if(k == GRAPH_INVALID_KIND)
	return NULL;
    else if(k == GRAPH_XREF_KIND)
	n = read_xref();
    else if(k == GRAPH_LREF_KIND) {
	int label = read_label();
	n = find_node(label);
	assert(n);
    } else {
	n = graph_base_node::create(k, this);
	assert(n != NULL);
	n->set_region_id(region_id);
	mark(n);
	n->read(this);
    }

    if(n && !n->is(expected)) {
	read_error(k, expected);
	n = NULL;
    }

    return n;
}

graph_base_node *graph_reader::find_node(int label)
{
    return graph_marker::find_node(label);
}
char *__nIds[] = {
	"Invalid!", "?", "?",
	"Node", 
	"Expr", 
	"Const", 
	"BinOp", 
	"Stats", 
	"Stat", 
	"While", 
	"If"	/* 8 kinds */

};

nKind __nLastDerived[] = {
	nkInvalid, nkInvalid, nkInvalid,
	nkIf, 
	nkBinOp, 
	nkConst, 
	nkBinOp, 
	nkStats, 
	nkIf, 
	nkWhile, 
	nkIf
};
nKind __nBaseKind[] = {
	nkInvalid, nkInvalid, nkInvalid,
	nkInvalid, 
	nkNode, 
	nkExpr, 
	nkExpr, 
	nkNode, 
	nkNode, 
	nkStat, 
	nkStat
};


void sNode::print(graph_printer *gp)
{
}

void sNode::visit(graph_visit_f *pre, graph_visit_f *post, graph_visitor *gv)
{
}

void sNode::write(graph_writer *gw)
{
}

void sNode::read(graph_reader *gr)
{
}

nKind sNode::kind()
{
	return nkNode;
}

char *sNode::type_id()
{
	return type_id(kind());
}

bool sNode::is(nKind k)
{
	if(this == NULL) return false;
	nKind this_k = kind();
	return this_k >= k && this_k <= __nLastDerived[k];
}

bool sNode::is_base_of(nKind b, nKind d)
{
	return d >= b && d <= __nLastDerived[b];
}

char *sNode::type_id(nKind k)
{
	return __nIds[k];
}

nKind sNode::base_kind(nKind k)
{
	return __nBaseKind[k];
}

nNode sNode::create(nKind k, void *placement_hint)
{
	switch(k) {
	case nkInvalid:
		return NULL;
	case nkNode:
		return mNode();
	case nkStat:
		return mStat();
	case nkIf:
		return mIf();
	case nkWhile:
		return mWhile();
	case nkStats:
		return mStats();
	case nkExpr:
		return mExpr();
	case nkBinOp:
		return mBinOp();
	case nkConst:
		return mConst();
	default:
		assert(false);
	}
}

void sStat::print(graph_printer *gp)
{
	sNode::print(gp);
	gp->print_lab("lineno"); PRINT_ATTR(lineno, gp);
}

void sStat::visit(graph_visit_f *pre, graph_visit_f *post, graph_visitor *gv)
{
	sNode::visit(pre, post, gv);
}

void sStat::write(graph_writer *gw)
{
	sNode::write(gw);
	WRITE_ATTR(lineno, gw);
}

void sStat::read(graph_reader *gr)
{
	sNode::read(gr);
	READ_ATTR(lineno, gr);
}

nKind sStat::kind()
{
	return nkStat;
}

void sIf::print(graph_printer *gp)
{
	sStat::print(gp);
	gp->print_lab("cond"); gp->print(cond);
	gp->print_lab("then_stats"); gp->print(then_stats);
	gp->print_lab("else_stats"); gp->print(else_stats);
}

void sIf::visit(graph_visit_f *pre, graph_visit_f *post, graph_visitor *gv)
{
	sStat::visit(pre, post, gv);
	gv->visit(pre, post, cond);
	gv->visit(pre, post, then_stats);
	gv->visit(pre, post, else_stats);
}

void sIf::write(graph_writer *gw)
{
	sStat::write(gw);
	gw->write(cond);
	gw->write(then_stats);
	gw->write(else_stats);
}

void sIf::read(graph_reader *gr)
{
	sStat::read(gr);
	cond = nExpr(gr->read(nkExpr));
	then_stats = nStats(gr->read(nkStats));
	else_stats = nStats(gr->read(nkStats));
}

nKind sIf::kind()
{
	return nkIf;
}

void sWhile::print(graph_printer *gp)
{
	sStat::print(gp);
	gp->print_lab("cond"); gp->print(cond);
	gp->print_lab("stats"); gp->print(stats);
}

void sWhile::visit(graph_visit_f *pre, graph_visit_f *post, graph_visitor *gv)
{
	sStat::visit(pre, post, gv);
	gv->visit(pre, post, cond);
	gv->visit(pre, post, stats);
}

void sWhile::write(graph_writer *gw)
{
	sStat::write(gw);
	gw->write(cond);
	gw->write(stats);
}

void sWhile::read(graph_reader *gr)
{
	sStat::read(gr);
	cond = nExpr(gr->read(nkExpr));
	stats = nStats(gr->read(nkStats));
}

nKind sWhile::kind()
{
	return nkWhile;
}

void sStats::print(graph_printer *gp)
{
	sNode::print(gp);
	gp->print_lab("next"); gp->print(next);
	gp->print_lab("stat"); gp->print(stat);
}

void sStats::visit(graph_visit_f *pre, graph_visit_f *post, graph_visitor *gv)
{
	sNode::visit(pre, post, gv);
	gv->visit(pre, post, next);
	gv->visit(pre, post, stat);
}

void sStats::write(graph_writer *gw)
{
	sNode::write(gw);
	gw->write(next);
	gw->write(stat);
}

void sStats::read(graph_reader *gr)
{
	sNode::read(gr);
	next = nStats(gr->read(nkStats));
	stat = nStat(gr->read(nkStat));
}

nKind sStats::kind()
{
	return nkStats;
}

nStats reverse(nStats &l)
{
	nStats n, n2, r = NULL;
	for(n = l; n; n = n2) {
		n2 = n->next;
		n->next = r;
		r = n;
	}
	return r;
}

void sExpr::print(graph_printer *gp)
{
	sNode::print(gp);
}

void sExpr::visit(graph_visit_f *pre, graph_visit_f *post, graph_visitor *gv)
{
	sNode::visit(pre, post, gv);
}

void sExpr::write(graph_writer *gw)
{
	sNode::write(gw);
}

void sExpr::read(graph_reader *gr)
{
	sNode::read(gr);
}

nKind sExpr::kind()
{
	return nkExpr;
}

void sBinOp::print(graph_printer *gp)
{
	sExpr::print(gp);
	gp->print_lab("op"); PRINT_ATTR(op, gp);
	gp->print_lab("left"); gp->print(left);
	gp->print_lab("right"); gp->print(right);
}

void sBinOp::visit(graph_visit_f *pre, graph_visit_f *post, graph_visitor *gv)
{
	sExpr::visit(pre, post, gv);
	gv->visit(pre, post, left);
	gv->visit(pre, post, right);
}

void sBinOp::write(graph_writer *gw)
{
	sExpr::write(gw);
	gw->write(left);
	gw->write(right);
	WRITE_ATTR(op, gw);
}

void sBinOp::read(graph_reader *gr)
{
	sExpr::read(gr);
	left = nExpr(gr->read(nkExpr));
	right = nExpr(gr->read(nkExpr));
	READ_ATTR(op, gr);
}

nKind sBinOp::kind()
{
	return nkBinOp;
}

void sConst::print(graph_printer *gp)
{
	sExpr::print(gp);
	gp->print_lab("val"); PRINT_ATTR(val, gp);
	gp->print_lab("buffer"); PRINT_ATTR(buffer, gp);
}

void sConst::visit(graph_visit_f *pre, graph_visit_f *post, graph_visitor *gv)
{
	sExpr::visit(pre, post, gv);
}

void sConst::write(graph_writer *gw)
{
	sExpr::write(gw);
	WRITE_ATTR(val, gw);
	WRITE_ATTR(buffer, gw);
}

void sConst::read(graph_reader *gr)
{
	sExpr::read(gr);
	READ_ATTR(val, gr);
	READ_ATTR(buffer, gr);
}

nKind sConst::kind()
{
	return nkConst;
}