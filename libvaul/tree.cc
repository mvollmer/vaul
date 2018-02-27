/* reading and writing the abstract graph

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

#include <vaul/tree.h>
#include <vaul/design_unit.h>
#include <vaul/library.h>
#include <vaul/lexer.h>
#include <vaul/parser.h>

#include <vaulutil.h>
#include <stream.h>
#include <assert.h>
#include <stdlib.h>

vaul_id_set::vaul_id_set()
{
    ids = NULL;
    n_ids = 0;
}

void vaul_id_set::init()
{
    free(ids);
    ids = NULL;
    n_ids = 0;
}

vaul_id_set::~vaul_id_set()
{
    init();
}

void vaul_id_set::add(Id id)
{
    for(int i = 0; i < n_ids; i++)
	if(ids[i] == NULL) {
	    ids[i] = id;
	    return;
	}
    ids = (Id *)xrealloc(ids, (n_ids+1)*sizeof(Id *));
    ids[n_ids++] = id;
}

void vaul_id_set::remove(Id id)
{
    for(int i = 0; i < n_ids; i++)
	if(vaul_name_eq(ids[i], id))
	    ids[i] = NULL;
}

bool vaul_id_set::contains(Id id)
{
    for(int i = 0; i < n_ids; i++)
	if(vaul_name_eq(ids[i], id))
	    return true;
    return false;
}

vaul_graph_writer::vaul_graph_writer(vaul_design_unit *region_id,
				     vaul_binfile *f)
    : graph_writer(region_id)
{
    unit_owner = region_id;
    file = f;
}

vaul_graph_writer::~vaul_graph_writer()
{
}

vaul_graph_reader::vaul_graph_reader(vaul_design_unit *region_id,
				     vaul_binfile *f,
				     vaul_libpool *l)
    : graph_reader(region_id)
{
    unit_owner = region_id;
    file = f;
    libs = l;
}

vaul_graph_reader::~vaul_graph_reader()
{
}

void vaul_graph_writer::write_label(int l)
{
    if(file)
	file->put(l);
}

int vaul_graph_reader::read_label()
{
    int l = -1;
    if(!is_error() && file && !file->is_error())
	file->get(l);
    return l;
}

void vaul_graph_writer::write_kind(nKind k)
{
    if(file)
	file->put(nKind_ShortRep(k));
}

nKind vaul_graph_reader::read_kind()
{
    nKind_ShortRep sk = nkInvalid;
    if(!is_error() && file && !file->is_error())
	file->get(sk);
    return nKind(sk);
}

#define LID_BITS    5		// max 32 libs
#define DID_BITS    10		//     1024 dunits
#define NID_BITS    16		//     65535 nodes

#define MAX_LID (1 << LID_BITS)
#define MAX_DID (1 << DID_BITS)
#define MAX_NID (1 << NID_BITS)

#if LID_BITS + DID_BITS + NID_BITS + 1 != 32
#error LID_BITS + DID_BITS + NID_BITS + 1 != 32
#endif

union xref {
    struct {
	unsigned
	    mark    : 1,
	    lid	    : LID_BITS,
	    did	    : DID_BITS,
	    nid	    : NID_BITS;
    } bits;
    long l;
};

void vaul_graph_writer::write_xref(nNode n)
{
    int lid, did, nid;

    assert(n);
    assert(n->owner);
    assert(n->owner->lib);

    did = n->owner->label;
    assert(did != 0);
    if(n->owner->lib == unit_owner->lib)
	lid = 0;
    else if(n->owner->lib->get_name() == NULL) {
	set_error(vhdlerr_usage, "can't save reference to unnamed library");
	return;
    } else
	lid = unit_owner->lib->libmap.s2i(n->owner->lib->get_name());
    nid = n->get_label();

    if(lid < MAX_LID && did < MAX_DID && nid < MAX_NID) {
	xref x;
	x.bits.mark = 1;
	x.bits.lid = lid;
	x.bits.did = did;
	x.bits.nid = nid;
	file->put(x.l);
    } else {
	message("\nYou have used (%d,%d,%d) (libs,dunits,nodes) !\n\n",
		lid, did, nid);
	file->put(lid);
	file->put(did);
	file->put(nid);
    }
}

nNode vaul_graph_reader::read_xref()
{
    nNode n = NULL;

    int lid, did, nid;
    xref x;

    if(file == NULL)
	return NULL;

    file->get(x.l);
    if(x.bits.mark) {
	lid = x.bits.lid;
	did = x.bits.did;
	nid = x.bits.nid;
    } else {
	lid = x.l;
	file->get(did);
	file->get(nid);
    }

    assert(did != 0);

    if(file->is_error())
	return NULL;
 
    vaul_design_unit *n_owner;
    char *ln;
    vaul_library *lib;
    if(lid == 0) {
	lib = unit_owner->lib;
	retain_ref(lib);
	ln = lib->get_name();
    } else {	
	ln = unit_owner->lib->libmap.i2s(lid);
	if(!ln) {
	    set_error(vhdlerr_lib_corrupt, "dangling xref to lib %d", lid);
	    return NULL;
	}
	if(!libs) {
	    set_error(vhdlerr_usage, "no libs for reading graphs");
	    return NULL;
	}
	lib = libs->get(ln);
    }
    n_owner = lib->get(did, libs);
    if(n_owner == NULL || n_owner->is_error()) {
	if(n_owner)
	    set_error(n_owner);
	if(!is_error())
	    set_error(vhdlerr_lib_corrupt, "dangling xref to %s.%d", ln, did);
	release_ref(lib);
	release_ref(n_owner);
	return NULL;
    }
    lib->release();
    if(unit_owner->is_older_than(n_owner))
	set_error(vhdlerr_out_of_date, "%s.%s is newer than %s",
		  n_owner->lib->get_name(), n_owner->get_name(),
		  unit_owner->get_name());
    else {
	unit_owner->uses(n_owner);
	n = n_owner->find_node(nid);
	if(n == NULL)
	    set_error(vhdlerr_lib_corrupt,
		      "dangling xref to %s.%s.%d",
		      ln, n_owner->get_name(), nid);
    }	
    release_ref(n_owner);

    return n;
}

void vaul_graph_reader::read_error(nKind r, nKind x)
{
    if(!is_error())
	set_error(vhdlerr_lib_corrupt,
		  "tree corrupt (got %s instead of %s)",
		  sNode::type_id(r), sNode::type_id(x));
}

vaul_graph_printer::vaul_graph_printer(vaul_design_unit *region_id)
    : graph_printer(region_id)
{
}

vaul_graph_printer::~vaul_graph_printer()
{
}

void vaul_graph_printer::print_xref(nNode n)
{
    if(n->owner && n->owner->lib)
	printf("[%s/%s/%d]", n->owner->lib->get_name(),
	       n->owner->get_name(), n->get_label());
    else
	printf("[?]\n");
}

void vaul_node_printer::format_out(FILE *f, const char *s, int l, va_list &ap)
{
    switch(s[l-1]) {

    case 'n': {
	print_node(f, va_arg(ap, nNode), 0);
	break;
    }

    case ':': {
	nPosNode n = va_arg(ap, nPosNode);
	if(n)
	    fprintf(f, "%s:%d: ", get_source(n), n->lineno);
	else
	    fprintf(f, "?:?: ");
	break;
    }

    case '?': {
	vaul_lexer *l = va_arg(ap, vaul_lexer *);
	if(l) 
	    fprintf(f, "%s:%d: ", l->filename, l->lineno);
	break;
    }

    case '!': {
	vaul_lexer *l = va_arg(ap, vaul_lexer *);
	int lineno = va_arg(ap, int);
	if(l)
	    fprintf(f, "%s:", l->filename);
	if(lineno > 0)
	    fprintf(f, "%d:", lineno);
	if(l || lineno > 0)
	    fprintf(f, " ");
	break;
    }

    case 'C': {
	fprintf(f, "at %s", va_arg(ap, vaul_lexer *)->context());
	break;
    }

    default:
	vaul_std_printer::format_out(f, s, l, ap);
	break;
    }
}

void vaul_node_printer::print_node(FILE *f, nNode n, int arg)
{
    if(n == NULL)
	fprintf(f, "(null)");
    else if(n->is(nkName)) {
	switch(n->kind()) {
	case nkSimpleName:
	    fprintf(f, "%s", nSimpleName(n)->id);
	    break;
	case nkOpSymbol:
	    fprintf(f, "%s", nOpSymbol(n)->op);
	    break;
	case nkAttributeName:
	    fprintf(f, "%n'%s", nAttributeName(n)->prefix, 
		                nAttributeName(n)->attribute);
	    break;
	case nkSelName:
	    fprintf(f, "%n.%s", nSelName(n)->prefix, nSelName(n)->suffix);
	    break;
	case nkIftsName:
	    fprintf(f, "%n(...)", nIftsName(n)->prefix);
	    break;
	default:
	    fprintf(f, "FIXME(%s)", n->type_id());
	    break;
	}
    } else if(n->is(nkDeclaration)) {
	if(nDeclaration(n)->id == NULL) {
	    if(n->is(nkSubType)) {
		nType t;
		for(t = nType(n);
		    t->is(nkSubType) && t->id == NULL;
		    t = nSubType(t)->base)
		    ;
		fprintf(f, "%n", t);
		if(nSubType(n)->constraint)
		    fprintf(f, " %n", nSubType(n)->constraint);
	    } else if(n->is(nkArrayType)) {
		fprintf(f, "%sarray(", n->is(nkSubarrayType)? "sub":"");
		for(nIndexType it = nArrayType(n)->first_index; it; it=it->next) {
		    fprintf(f, "%n", it->index_type);
		    if(it->next)
			fprintf(f, ", ");
		}
		fprintf(f, ") of %n", nArrayType(n)->element_type);
	    } else if(n->is(nkAccessType)) {
		fprintf(f, "access to %n", nAccessType(n)->designated);
	    } else if(n->is(nkPhysicalType)) {
		fprintf(f, "%n", nPhysicalType(n)->declaring_subtype);
	    } else
		fprintf(f, "<anonymous>");
	} else
	    fprintf(f, "%s", nDeclaration(n)->idstr());
	if(n->is(nkEnumLiteral))
	    fprintf(f, " (%n)", nEnumLiteral(n)->type);
	else if(n->is(nkSubprogram)) {
	    fprintf(f, "(");
	    for(nInterface i = nSubprogram(n)->first_formal; i;
		     i = i->next_element)
		fprintf(f, "%n%s", i->type, i->next_element? ",":"");
	    fprintf(f, ")");
	    if(n->is(nkFunction))
		fprintf(f, " return %n", nFunction(n)->return_type);
	}
    } else if(n->is(nkExplicitRange)) {
	nExplicitRange r = nExplicitRange(n);
	fprintf(f, "%n %s %n", r->first,
		r->dir == DirectionUp? "to":"downto", r->last);
    } else if(n->is(nkArrayRange)) {
	nArrayRange r = nArrayRange(n);
	fprintf(f, "%n'%s[%d]",
		r->array,
		r->is(nkArrayAttr_RANGE)? "RANGE" : "REVERSE_RANGE",
		r->index);
    } else if(n->is(nkIndexConstraint)) {
	nIndexConstraint ic = nIndexConstraint(n);
	fprintf(f, "(");
	while(ic) {
	    fprintf(f, "%n", ic->type);
	    ic = ic->next;
	    if(ic)
		fprintf(f, ", ");
	}
	fprintf(f, ")");
    } else if(n->is(nkSimpleObjectRef)) {
	nSimpleObjectRef so = nSimpleObjectRef(n);
	fprintf(f, "%n", so->object);
    } else if(n->is(nkFunctionCall)) {
	fprintf(f, "call of %n", nFunctionCall(n)->func);
    } else if(n->is(nkLiteralRef)) {
	fprintf(f, "%s", nLiteralRef(n)->value);
	if(n->is(nkPhysicalLiteralRef))
	    fprintf(f, " %n", nPhysicalLiteralRef(n)->unit);
    } else if(n->is(nkAmbgArrayLitRef)) {
	fprintf(f, "%s", nAmbgArrayLitRef(n)->value);
    } else if(n->is(nkArrayObjectRef)) {
	nArrayObjectRef aor = nArrayObjectRef(n);
	fprintf(f, "array element of %n", aor->array);
    } else if(n->is(nkRecordObjectRef)) {
	nRecordObjectRef ror = nRecordObjectRef(n);
	if(ror->element && ror->element->id)
	    fprintf(f, "%n::%s", ror->record_type, ror->element->id);
	else
	    fprintf(f, "element of %n", ror->record_type);
    } else if(n->is(nkRecordElement))
	fprintf(f, "%s", nRecordElement(n)->id);
    else if(n->is(nkExprChoice))
	fprintf(f, "%n", nExprChoice(n)->expr);
    else if(n->is(nkNamedChoice))
	fprintf(f, "%n", nNamedChoice(n)->name);
    else if(n->is(nkRangeChoice))
	fprintf(f, "%n", nRangeChoice(n)->range);
    else if(n->is(nkUnresolvedName))
	fprintf(f, "%n", nUnresolvedName(n)->name);
    else if(n->is(nkQualifiedExpr))
	fprintf(f, "%n'(...)", nQualifiedExpr(n)->type);
    else if(n->is(nkAmbgAggregate)) {
	nAmbgAggregate a = nAmbgAggregate(n);
	fprintf(f, "( ");
	for(nElemAssoc ea = a->first_assoc; ea; ea = ea->next) {
	    for(nChoice c = ea->first_choice; c; c = c->next) {
		fprintf(f, "%n", c);
		if(c->next)
		    fprintf(f, "| ");
	    }
	    if(ea->first_choice)
		fprintf(f, " => ");
	    fprintf(f, "%n", ea->actual);
	    if(ea->next)
		fprintf(f, ", ");
	}
	fprintf(f, " )");
    } else if(n->is(nkRangeAssocElem))
	fprintf(f, "%n", nRangeAssocElem(n)->range);
    else if(n->is(nkNamedAssocElem)) {
	nNamedAssocElem ne = nNamedAssocElem(n);
	if(ne->formal)
	    fprintf(f, "%n => ", ne->formal);
	fprintf(f, "%n", ne->actual);
    } else if(n->is(nkAmbgEnumLitRef))
	fprintf(f, "%n", nAmbgEnumLitRef(n)->set->name);
    else if(n->is(nkNewExpr))
	fprintf(f, "new ...");
    else
	fprintf(f, "FIXME(%s)", n->type_id());
}
    
char *vaul_node_printer::get_source(nNode n)
{
    return (n && n->owner)? n->owner->get_source() : "?";
}

nType sType::get_base()
{
    if(this->is(nkSubType)) {
	assert(nSubType(this)->base);
	return nSubType(this)->base->get_base();
    } else
	return this;
}
