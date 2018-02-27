/* declarations

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

#include <vaul/parser.h>
#include <vaul/design_unit.h>
#include <vaul/library.h>

#include <stdio.h>
#include <vaulutil.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>

#define psr vaul_parser

#define NEW_NODE_PLACEMENT (pool)
#include "tree.m"

void psr::init()
{
    cur_scope = NULL;
    cur_du = new vaul_design_unit(NULL, lex->filename);
    cur_body = NULL;

    int l = lex->lineno;
    push_scope(mTopScope(l, NULL));
    add_decl(mLibNameDecl(l, "work"));
    add_decl(mLibNameDecl(l, "std"));

    std = NULL;

    announced_scope = NULL;
}

void psr::start(nScope u)
{
    if(u->is(nkStandardPackage))
	std = nStandardPackage(u);
    else {
	int l = lex->lineno;
	nSelName sn = mSelName(l, mSimpleName(l, "std"), "standard");
	nDeclaration d = find_single_decl(sn, nkStandardPackage,
					  "(the) standard package");
	if(d == NULL)
	    std = mStandardPackage(l, "pseudo-standard");
	else {
	    std = nStandardPackage(d);
	    add_decl(mIndirectDecl(l, NULL, std));
	}
    } 

    cur_du->tree = u;
    add_decl(u);
    push_scope(u);

    if(u->is(nkArchitecture) && u->continued)
	cur_du->set_name(libs->architecture_name(u->continued->id, u->id));
    else if(u->is(nkPackageBody))
	cur_du->set_name(libs->package_body_name(u->id));
    else
	cur_du->set_name(u->id);

    if(libs->used_in_this_session(cur_du))
	error("%:%n must be declared before its first reference", u, u);
    if(libs->inserted_in_this_session(cur_du))
	error("%:redeclaration of %n", u, u);
}

vaul_design_unit *psr::finish()
{
    if(cur_scope && cur_scope->is(nkTopScope))
	pop_scope(cur_scope);

    if(!eof && cur_du) {
	pool.store(cur_du->mem);
	return cur_du;
    } else {
	release_ref(cur_du);
	return NULL;
    }
}

void psr::add_libs(nIdList ids)
{
    while(ids) {
	add_decl(mLibNameDecl(ids->lineno, ids->id));
	ids = ids->link;
    }
}   

void psr::use(nSelNameList snl)
{
    for(; snl; snl = snl->link) {
	nSelName sn = snl->name;
	nDeclaration d = find_single_decl(sn->prefix, nkPackage, "package");
	if(d == NULL)
	    continue;
	Id id = vaul_name_eq(sn->suffix, "all")? NULL : sn->suffix;
	add_decl(mIndirectDecl(sn->lineno, id, nPackage(d)));
	if(id) {
	    vaul_decl_set dset;
	    find_decls(dset, id, nPackage(d), true);
	    if(dset.found_none())
		info("%:warning: %s is not declared in %n", sn, id, d);
	}
    }
}

static void visit_scope(nScope s, void f(nDeclaration, void *), void *cl)
{
    for(nDeclaration d = s->first(); d; d = d->next()) {
	if(d->is(nkIndirectDecl))
	    visit_scope(nIndirectDecl(d)->ind_scope, f, cl);
	else if(d->is(nkScope))
	    visit_scope(nScope(d), f, cl);
	else
	    f(d, cl);
    }
}

void psr::visit_decls(void f(nDeclaration, void *), void *cl)
{
    nScope s = cur_scope;
    while(s && !s->is(nkTopScope))
	s = s->scope;
    assert(s);

    visit_scope(s, f, cl);
}

void psr::push_scope(nScope s)
{
    s->scope = cur_scope;
    cur_scope = s;

    s->decls_in_flight.init();
    s->first_decl = reverse(s->first_decl);
}

void psr::pop_scope(nScope s)
{
    assert(cur_scope && cur_scope == s);

    s->decls_in_flight.init();
    s->first_decl = reverse(s->first_decl);

    // check incomplete types
    for(nDeclaration d = s->first_decl; d; d = d->next_decl)
	if(d->is(nkIncompleteType)) {
	    error("%n is still incomplete", d);
	    info("%:here is the incomplete declaration", d);
	}

    // check unitialized constants
    if(s->is(nkPackageBody) && s->continued && s->continued->is(nkPackage)) {
	nPackage p = nPackage(s->continued);
	for(nDeclaration d = p->first_decl; d; d = d->next_decl) {
	    if(!d->is(nkConstant) || nConstant(d)->initial_value != NULL)
		continue;
	    nDeclaration d2;
	    for(d2 = s->first_decl; d2; d2 = d2->next_decl)
		if(d2->is(nkConstantBody) && nConstantBody(d2)->decl == d)
		    break;
	    if(d2 == NULL) {
		error("%n is still not initialized", d);
		info("%:here is the declaration", d);
	    }
	}
    }

    if(s->is(nkBlock))
	bind_specs(nBlock(s));

    cur_scope = s->scope;
}

static bool top_can_contain(nDeclaration d)
{
    return d->is(nkLibNameDecl)
	|| d->is(nkIndirectDecl)
	|| d->is(nkScope);
}

nDeclaration sDeclaration::next()
{
    if(next_decl)
	return next_decl;
    else if(scope->continued)
	return scope->continued->first();
    else
	return NULL;
}

nDeclaration sScope::first()
{
    for(nScope s = this; s; s = s->continued)
	if(s->first_decl)
	    return s->first_decl;
    return NULL;
}

static inline bool overloadable(nDeclaration d)
{
    return d->is(nkEnumLiteral) || d->is(nkSubprogram);
}

static bool homograph(nDeclaration d1, nDeclaration d2)
{
    if(!vaul_name_eq(d1->id, d2->id))
	return false;

    nInterface p1 =
     d1->is(nkSubprogram)? nSubprogram(d1)->first_formal : NULL;
    nInterface p2 = 
     d2->is(nkSubprogram)? nSubprogram(d2)->first_formal : NULL;

    while(p1 && p2) {
	if(p1->type->get_base() != p2->type->get_base())
	    return false;
	p1 = p1->next_element;
	p2 = p2->next_element;
    }
    if(p1 || p2)
	return false;

    nType t1 = d1->is(nkFunction)? nFunction(d1)->return_type->get_base() :
	       d1->is(nkEnumLiteral)? nEnumLiteral(d1)->type : NULL;
    nType t2 = d2->is(nkFunction)? nFunction(d2)->return_type->get_base() :
	       d2->is(nkEnumLiteral)? nEnumLiteral(d2)->type : NULL;

    return t1 == t2;
}

/* XXX all this same_* stuff should be done by ctree
*/

static bool same_expr(nExpr e1, nExpr e2)
{
    return false; // XXX
}

static bool same_constraint(nConstraint c1, nConstraint c2)
{
    if(c1 == c2)
	return true;
    if(!c1 || !c2)
	return false;
    if(c1->kind() != c2->kind())
	return false;
    switch(c1->kind()) {
    case nkExplicitRange: {
	nExplicitRange r1 = nExplicitRange(c1);
	nExplicitRange r2 = nExplicitRange(c2);
	return r1->dir == r2->dir
	    && same_expr(r1->first, r2->first)
	    && same_expr(r1->last, r2->last);
    } break;
    case nkArrayRange: {
	nArrayRange r1 = nArrayRange(c1);
	nArrayRange r2 = nArrayRange(c2);
	return r1->array == r2->array && r1->index == r2->index;
    } break;
    default:
	assert(false);
    }
}

static bool same_type(nType t1, nType t2)
{
    if(t1 == t2)
	return true;
    if(!t1 || !t2)
	return false;
    if(t1->get_base() != t2->get_base())
	return false;
    if(t1->is(nkSubType) && t2->is(nkSubType))
	return same_constraint(nSubType(t1)->constraint,
			       nSubType(t2)->constraint);
    return false;
}

static bool conforming(nDeclaration d1, nDeclaration d2)
{
    if(d1->is(nkSubprogram)) {
	if((d1->is(nkFunction) && !d2->is(nkFunction))
	   || (d1->is(nkProcedure) && !d2->is(nkProcedure)))
	    return false;
	nInterface p1 = nSubprogram(d1)->first_formal;
	nInterface p2 = nSubprogram(d2)->first_formal;
	while(p1 && p2) {
	    if(p1->mode != p2->mode
	    || p1->object_class != p2->object_class
	    || p1->buffer != p2->buffer
	    || !same_type(p1->type, p2->type))
		return false;
	    p1 = p1->next_element;
	    p2 = p2->next_element;
	}
	if(d1->is(nkFunction))
	    return same_type(nFunction(d1)->return_type,
			     nFunction(d2)->return_type);
	return true;
    }	    
    return false;
}

static bool immediate_scope(nDeclaration d, nScope s)
{
    while(s) {
	if(d->scope == s)
	    return true;
	s = s->continued;
    }
    return false;
}

nDeclaration psr::add_decl(nScope scope, nDeclaration decl, int lineno, Id id)
{
    unselect_scope();

    if(decl == NULL)
	return NULL;

    assert(!scope->is(nkTopScope) || top_can_contain(decl));

    if(lineno)
	decl->lineno = lineno;
    if(id)
	decl->id = id;

    if(decl->id && !decl->is(nkIndirectDecl)) {
	scope->decls_in_flight.remove(decl->id);

	/* deal with redeclarations
	*/
	nDeclaration prev = NULL;
	nDeclaration d;
	for(d = scope->first(); d; d = d->next())
	    if(homograph(d, decl)) {
		prev = d;
		break;
	    }
	while(prev) {
	    if(decl->is(nkType) && prev->is(nkType)) {
		nType pt = nType(prev);
		nType dt = nType(decl);
		if(pt->is(nkIncompleteType) && !dt->is(nkIncompleteType)) {
		    nIncompleteType(pt)->complete(dt);
		    rem_decl(scope, prev);
		    break;
		} else if(dt->is(nkIncompleteType))
		    return prev;
	    } else if(decl->is(nkConstant) && prev->is(nkConstant)) {
		if(nConstant(prev)->initial_value == NULL) {
		    add_decl(mConstantBody(d->lineno, NULL, nConstant(prev),
			     nConstant(decl)->initial_value));
		    return prev;
		}
	    } else if(decl->is(nkLibNameDecl) && prev->is(nkLibNameDecl))
		return prev;
	    else if(conforming(prev, decl)) {
		if(prev->is(nkPredefOp)) {
		    if(prev->scope == scope) {
			rem_decl(scope, prev);
			break;
		    } else {
			error("%:sorry, you must declare builtin operators"
			      " along with their types", decl);
			decl->scope = scope;
			return decl;
		    }
		}
		return prev;
	    }

	    error("%:redeclaration of %n", decl, prev);
	    info("%: previously declared here", prev);

	    // point redecl to its 'wannabe' scope, but don't link it
	    // into scope's list, so that it can't be found.
	    //
	    decl->scope = scope;
	    return decl;
	}
    }

    decl->scope = scope;
    decl->next_decl = scope->first_decl;
    scope->first_decl = decl;

    if(scope->is(nkStandardPackage) && decl->is(nkType)) {
	nStandardPackage std = nStandardPackage(scope);
	nType t = nType(decl);
	if(vaul_name_eq(t->id, "bit"))
	    std->predef_BIT = t;
	else if(vaul_name_eq(t->id, "boolean"))
	    std->predef_BOOLEAN = t;
	else if(vaul_name_eq(t->id, "integer"))
	    std->predef_INTEGER = t;
	else if(vaul_name_eq(t->id, "real"))
	    std->predef_REAL = t;
	else if(vaul_name_eq(t->id, "time"))
	    std->predef_TIME = t;
	else if(vaul_name_eq(t->id, "string"))
	    std->predef_STRING = t;
	else if(vaul_name_eq(t->id, "bit_vector"))
	    std->predef_BIT_VECTOR = t;
	else if(vaul_name_eq(t->id, "severity_level"))
	    std->predef_SEVERITY_LEVEL = t;
	else if(t->is(nkUniversalInteger))
	    std->universal_integer = nUniversalInteger(t);
	else if(t->is(nkUniversalReal))
	    std->universal_real = nUniversalReal(t);
    }

    if(decl->is(nkScope) && nScope(decl)->continued && scope->is(nkTopScope))
	scope->continued = nScope(decl)->continued->scope;

    return decl;
}

void psr::rem_decl(nScope scope, nDeclaration decl)
{
    for(nDeclaration *d = &scope->first_decl; *d; d = &(*d)->next_decl)
	if((*d) == decl) {
	    *d = decl->next_decl;
	    return;
	}
    assert(false);
}

void psr::start_decl(Id id)
{
    assert(cur_scope != NULL);
    cur_scope->decls_in_flight.add(id);
}

vaul_decl_set::vaul_decl_set(vaul_error_printer *p)
{
    pr = p;
    decls = NULL;
    reset();
}

vaul_decl_set::~vaul_decl_set()
{
    reset();
}

void vaul_decl_set::reset()
{
    free(decls);

    decls = NULL;
    n_decls = 0;
    doing_indirects = false;
    not_overloadable = false;
    name = NULL;
}

void vaul_decl_set::add(nDeclaration d)
{
    // filter out hidden decls
    if(!doing_indirects) {
	if(not_overloadable)
	    return;
	if(!overloadable(d))
	    not_overloadable = true;
	for(int i = 0; i < n_decls; i++)
	    if(decls[i].state == valid && homograph(decls[i].d, d)) {
		if(name && pr)
		    pr->info("%:%n hides %n", name, decls[i].d, d); // XXX
		return;
	    }
    }

    for(int i = 0; i < n_decls; i++)
	if(decls[i].d == d)
	    return;

    decls = (item *)xrealloc(decls, (n_decls+1) * sizeof(item));
    decls[n_decls].d = d;
    decls[n_decls].state = doing_indirects? pot_valid : valid;
    decls[n_decls].cost = 0;
    n_decls++;
}

bool vaul_decl_set::finish_scope(nScope s)
{
    if(doing_indirects)
	return true;

    int i;

    // if there is more than one !overloadable pot_valid, remove all
    //	!overloadable pot_valids.
    int n_simple_pot_valids = 0;
    for(i = 0; i < n_decls; i++)
	if(decls[i].state == pot_valid && !overloadable(decls[i].d))
	    n_simple_pot_valids++;
    if(n_simple_pot_valids > 1)
	for(i = 0; i < n_decls; i++)
	    if(decls[i].state == pot_valid && !overloadable(decls[i].d))
		decls[i].state = invalid;

    // filter out pot_valids that have a homograph in immediate scope S
    for(i = 0; i < n_decls; i++)
	if(decls[i].state == valid && immediate_scope(decls[i].d, s))
	    for(int j = 0; j < n_decls; j++)
		if(decls[j].state == pot_valid
		&& homograph(decls[i].d, decls[j].d))
		    decls[j].state = invalid;

    // all pot_valids that remain become valids
    for(i = 0; i < n_decls; i++)
	if(decls[i].state == pot_valid) {
	    decls[i].state = valid;
	    if(!overloadable(decls[i].d))
		not_overloadable = true;
	}

    // if there are physical units and something else, remove the
    // physical units.
    int n_others = 0;
    for(i = 0; i < n_decls; i++)
	if(decls[i].state == valid && !decls[i].d->is(nkPhysicalUnit))
	    n_others++;
    if(n_others > 0) {
	for(i = 0; i < n_decls; i++)
	    if(decls[i].d->is(nkPhysicalUnit))
		decls[i].state = invalid;
    }

    // figure out if no more decls can possibly be accepted
    return not_overloadable;
}

void vaul_decl_set::begin_indirects()
{
    doing_indirects = true;
}

void vaul_decl_set::end_indirects()
{
    doing_indirects = false;
}

void vaul_decl_set::refresh()
{
    for(int i = 0; i < n_decls; i++) {
	if(decls[i].state == pot_invalid)
	    decls[i].state = valid;
	decls[i].cost = 0;
    }
}

void vaul_decl_set::invalidate_pot_invalids()
{
    for(int i = 0; i < n_decls; i++)
	if(decls[i].state == pot_invalid)
	    decls[i].state = invalid;
}

void vaul_decl_set::filter(int (*f)(nDeclaration, void *), void *cl)
{
    for(int i = 0; i < n_decls; i++)
	if(decls[i].state == valid) {
	    int c = f(decls[i].d, cl);
	    if(c < 0)
		decls[i].state = pot_invalid;
	    else
		decls[i].cost = c;
	}
}

void vaul_decl_set::iterate(void (*f)(nDeclaration, void *), void *cl)
{
    for(int i = 0; i < n_decls; i++)
	if(decls[i].state == valid)
	    f(decls[i].d, cl);
}


int vaul_decl_set::retain_lowcost()
{
    int i, lc = INT_MAX;

    for(i = 0; i < n_decls; i++)
	if(decls[i].state == valid && decls[i].cost < lc)
	    lc = decls[i].cost;
    for(i = 0; i < n_decls; i++)
	if(decls[i].state == valid && decls[i].cost > lc)
	    decls[i].state = pot_invalid;
    return lc == INT_MAX ? -1 : lc;
}

static char *item_state[] = {
    "invalid",
    "pot. invalid",
    "pot. valid",
    "valid"
};

nDeclaration vaul_decl_set::single_decl(bool print)
{
    // see if a single valid decl remains

    nDeclaration d = NULL;
    int lc;
    int i;
    for(i = 0; i < n_decls; i++)
	if(decls[i].state == valid) {
	    if(d)
		break;
	    d = decls[i].d;
	    lc = decls[i].cost;
	}
    if(i == n_decls && d) {
    //	if(print && name && pr && n_decls > 1)
    //	    pr->info("%:%n resolved to %n (%d)", name, name, d, lc);
	return d;
    }

    if(print && name && pr) {
	// report why not
	if(n_decls == 0)
	    pr->error("%:%n is undeclared", name, name);
	else {
	    // XXX - introduce different kinds of 'invalid'
	    //	     to disambiguate between different causes
	    pr->error("%:use of %n is ambigous", name, name);
	    show();
	}
    }
    return NULL;
}

bool vaul_decl_set::multi_decls(bool print)
{
    // see if at least one valid decl remains

    nDeclaration d = NULL;
    for(int i = 0; i < n_decls; i++)
	if(decls[i].state == valid)
	    return true;

    if(print && name && pr) {
	// report why not
	if(n_decls == 0)
	    pr->error("%:%n is undeclared", name, name);
	else {
	    // XXX - introduce different kinds of 'invalid'.
	    pr->error("%:use of %n is ambigous", name, name);
	    show();
	}
    }
    return false;
}

void vaul_decl_set::show(bool only_valids)
{
    if(pr == NULL)
	return;

    bool first = true;
    for(int j = 0; j < n_decls; j++) {
	if(!only_valids || decls[j].state == valid) {
	    pr->info("%:%s %n (%s %d)", decls[j].d,
		     first? "candidates are":
		     "              ",
		     decls[j].d, item_state[decls[j].state],
		     decls[j].cost);
	    first = false;
	}
    }
}

bool vaul_decl_set::found_none()
{
    return n_decls == 0;
}

void psr::find_decls(vaul_decl_set &ds, Id id, nScope scope, bool by_sel)
{
    if(scope->decls_in_flight.contains(id))
	return;

    for(nDeclaration d = scope->first(); d; d = d->next()) {
	if(d->is(nkIndirectDecl)) {
	    if(by_sel)
		continue;
	    if(d->id != NULL && !vaul_name_eq(d->id, id))
		continue;

	    // declarations reached thru use-clauses must be visible by
	    // selection
	    ds.begin_indirects();
	    find_decls(ds, id, nIndirectDecl(d)->ind_scope, true);
	    ds.end_indirects();
	} else if(vaul_name_eq(d->id, id))
	    ds.add(d);
    }

    if(!ds.finish_scope(scope) && !by_sel && scope->scope)
	find_decls(ds, id, scope->scope, by_sel);
}

static bool searchable_scope(nScope s)
{
    return s->is(nkPackage);
}

void psr::find_decls(vaul_decl_set &dset, nName n, nScope scope, bool by_sel)
{
    switch(n->kind()) {
    case nkSimpleName:
	find_decls(dset, nSimpleName(n)->id, scope, by_sel);
	dset.name = n;
	break;

    case nkOpSymbol:
	find_decls(dset, nOpSymbol(n)->op, scope, by_sel);
	dset.name = n;
	break;
    
    case nkSelName: {
	find_decls(dset, nSelName(n)->prefix, scope, by_sel);
	nDeclaration d = dset.single_decl(false);
	if(d == NULL)
	    return;

	char *desig = nSelName(n)->suffix;
	if(d->is(nkLibNameDecl)) {
	    dset.reset();
	    dset.name = n;
	    vaul_library *lib;
	    if(vaul_name_eq(d->id, "work")) {
		lib = libs->get_worklib();
		retain_ref(lib);
	    } else
		lib = libs->get(d->id);
	    vaul_design_unit *du = lib->get(desig, libs);
	    if(du) {
		if(du->is_error())
		    error("%:%s: %s", n, d->id, du->get_error_desc());
		else {
		    cur_du->uses(du);
		    dset.add(du->tree);
		}
	    }
	    release_ref(du);
	    release_ref(lib);
	    
	} else if(d->is(nkPackage)) {
	    dset.reset();
	    find_decls(dset, desig, nScope(d), true);
	    dset.name = n;
	}

	break;
    }

    case nkIftsName:
	find_decls(dset, nIftsName(n)->prefix, scope, by_sel);
	break;

    case nkAttributeName:
	find_decls(dset, nAttributeName(n)->prefix, scope, by_sel);
	break;

    default:
	info("XXX - can't look up a %s", n->type_id());
	dset.name = n;
	break;
    }
}

void psr::find_decls(vaul_decl_set &ds, nName n)
{
    if(selected_scope)
	find_decls(ds, n, selected_scope, true);
    find_decls(ds, n, cur_scope, false);
}

void psr::select_scope(nScope s)
{
    selected_scope = s;
}

void psr::unselect_scope()
{
    selected_scope = NULL;
}

nDeclaration psr::find_single_decl(nName name, nKind exp_k, char *kind_name)
{
    vaul_decl_set ds(this);
    find_decls(ds, name);
    nDeclaration d = ds.single_decl(kind_name != NULL);
    if(d && (!d->is(exp_k) || ds.name != name)) {
	if(kind_name)
	    error("%:%n is not a %s", name, name, kind_name);
	d = NULL;
    }
    return d;
}

nEntity psr::get_entity(char *n)
{
    nEntity entity = NULL;

    vaul_library *lib = libs->get_worklib();
    vaul_design_unit *du = lib->get(n, libs);
    if(du == NULL)
	error("unknown entity %s", n);
    else if(du->is_error())
	error("%s: %s", n, du->get_error_desc());
    else if(du->tree == NULL || !du->tree->is(nkEntity))
	error("%s is not an entity", n);
    else {
	cur_du->uses(du);
	entity = nEntity(du->tree);
    }
    release_ref(du);
    return entity;
}

nPackage psr::get_package(char *n)
{
    nPackage package = NULL;

    vaul_library *lib = libs->get_worklib();
    vaul_design_unit *du = lib->get(n, libs);
    if(du == NULL)
	error("unknown package %s", n);
    else if(du->is_error())
	error("%s: %s", n, du->get_error_desc());
    else if(du->tree == NULL || !du->tree->is(nkPackage))
	error("%s is not a package", n);
    else {
	cur_du->uses(du);
	package = nPackage(du->tree);
    }
    release_ref(du);
    return package;
}

nArchitecture psr::get_architecture(nName e, Id a)
{
    if(a == NULL) {
	info("XXX - no default architectures");
	return NULL;
    }

    assert(e != NULL);

    // XXX - memory leak
    nName ea;
    if(e->is(nkSelName))
	ea = mSelName(e->lineno, nSelName(e)->prefix,
		      libs->architecture_name(nSelName(e)->suffix, a));
    else if(e->is(nkSimpleName))
	ea = mSimpleName(e->lineno,
			 libs->architecture_name(nSimpleName(e)->id, a));
    else
	return NULL;

    return nArchitecture(find_single_decl(ea, nkArchitecture, "architecture"));
}

nConfiguration psr::get_configuration(nName c)
{
    return nConfiguration(
	    find_single_decl(c, nkConfiguration, "configuration"));
}

nAlias psr::add_Alias(int lno, Id id, nType type, nName thing)
{
    if(id == NULL || type == NULL || thing == NULL)
	return NULL;

    nExpr e = build_Expr(thing);
    overload_resolution(e, type);
    if(e == NULL)
	return NULL;

    return nAlias(add_decl(mAlias(lno, id, type, e)));
}
