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
       Fakultät für Elektrotechnik
       Universität Dortmund
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
