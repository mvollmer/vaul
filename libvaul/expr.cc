/* expressions

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
#include <vaul/tree.h>
#include <vaul/design_unit.h>
#include <vaul/library.h>

#include <stdio.h>
#include <vaulutil.h>
#include <stdlib.h>
#include <assert.h>

#define psr vaul_parser

#define NEW_NODE_PLACEMENT (pool)
#include "tree.m"

static int array_literal_conversion_cost(nAmbgArrayLitRef l, nType t, nKind k);
static int aggregate_conversion_cost(nAmbgAggregate, nType t, nKind k);

nPhysicalLiteralRef psr::build_PhysicalLiteralRef(int lineno, 
						  Literal lit, Id unit)
{
    nName u = mSimpleName(lineno, unit);
    nDeclaration d = find_single_decl(u, nkPhysicalUnit, "physical unit");
    if(d == NULL)
	return NULL;
    return mPhysicalLiteralRef(lineno, lit, nPhysicalUnit(d));
}

nExpr psr::build_QualifiedExpr(nName m, nExpr e)
{
    nType t = get_type(m);
    if(t == NULL)
	return NULL;
    overload_resolution(e, t);
    return mQualifiedExpr(m->lineno, t, e);
}

nExpr psr::build_TypeConversion(int lineno, nType t, nExpr e)
{
    overload_resolution(e, nkType);
    // XXX - check for closely related types and stuff
    // info("xxx - unchecked conversion from %n to %n", expr_type(e), t);
    return mTypeConversion(lineno, t, e);
}

nExpr psr::build_bcall(int lineno, nExpr l, Id id, nExpr r)
{
    nName n = mIftsName(lineno,
			mSimpleName(lineno, aprintf("\"%s\"", id)),
			mNamedAssocElem(lineno,
					r? mNamedAssocElem(lineno,
							   NULL,
							   NULL,
							   r) : NULL,
					NULL, l));
    // XXX - optimize memory use
    return build_Expr(n);
}

static void iterate_for_kind(nDeclaration d, void *cl)
{
    nKind &basic_k = *(nKind *)cl;
    nKind this_k = d->kind();

    if(basic_k == nkInvalid)
	basic_k = this_k;
    else if(basic_k != this_k && !sNode::is_base_of(basic_k, this_k)) {
	while(!sNode::is_base_of(this_k, basic_k))
	    this_k = sNode::base_kind(this_k);
	basic_k = this_k;
    }
}

nNode psr::build_Expr_or_Attr(nName n)
{
    // A OpSymbol on its own is really a string literal.
    //
    if(n->is(nkOpSymbol))
	return mAmbgArrayLitRef(n->lineno, nOpSymbol(n)->op);

    // see what declarations a prefix of the name matches
    //
    vaul_decl_set *set = new vaul_decl_set(this);
    find_decls(*set, n);

    if(!set->multi_decls(false)) {
	delete set;
	return mUnresolvedName(n->lineno, n);
    }

    // recursivly resolve the rest of the name.
    //
    return build_Expr_or_Attr(n, set, nkInvalid);
}

nExpr psr::validate_Expr(nNode ea)
{
    if(ea == NULL)
	return NULL;
    if(ea->is(nkExpr))
	return nExpr(ea);
    if(ea->is(nkArrayRange))
	error("%:%n can not be used in an expression", ea, ea);
    else
	error("%:%n can only be used as the prefix of another attribute",
	      ea, ea);
    return NULL;
}

nExpr psr::build_Expr(nName n)
{
    nNode ea = build_Expr_or_Attr(n);
    return validate_Expr(ea);
}

nExpr psr::make_appropriate(nExpr e)
{
    if(e && try_overload_resolution(e, NULL, nkAccessType)) {
	overload_resolution(e, nkAccessType);
	nType at = expr_type(e);
	if(at == NULL)
	    return NULL;
	assert(at->is(nkAccessType));
	e = mAccessObjectRef(e->lineno, e, nAccessType(at)->designated);
    }
    return e;
}

nNode psr::build_Expr_or_Attr(nName n, vaul_decl_set *set, nKind basic_k)
{
    if(basic_k == nkInvalid) {
	set->iterate(iterate_for_kind, &basic_k);
	assert(basic_k != nkDeclaration);
    }

    if(n->is(nkIftsName) 
       || (set->name == n && sNode::is_base_of(nkSubprogram, basic_k))) {

	// An array subscription, a function call (maybe without
	// arguments), a type conversion, or an array slicing.

	nName pfx = n;
	nGenAssocElem a = NULL;

	if(n->is(nkIftsName)) {
	    pfx = nIftsName(n)->prefix;
	    a = nIftsName(n)->assoc;
	}

	// Now PFX is the function to be called, the array to be
	// subscripted or sliced, or the target type of the conversion.
	// A is the list of arguments, or NULL if there are none.

	if(a && (a->is(nkRangeAssocElem) || a->is(nkSubtypeAssocElem))) {
	    // An array slice.

	    nExpr prefix = make_appropriate(build_Expr(pfx, set, basic_k));
	    overload_resolution(prefix, nkArrayType);
	    return build_SliceObjectRef(prefix, a);
	} else
	
	if(set->name == pfx && sNode::is_base_of(nkSubprogram, basic_k)) {
	    // A function call.

	    if(prepare_named_assocs(a)) {
		nAmbgCall e = mAmbgCall(pfx->lineno, nNamedAssocElem(a));
		e->set = set;
		int c = constrain(e, NULL, nkType, true);
		if(c < 0) {
		    report_mismatched_subprog(pfx, set, nNamedAssocElem(a));
		    delete set;
		    return NULL;
		}
		e->set->invalidate_pot_invalids();
		// info("%:at construction time:", pfx);
		// e->set->show(true);
		return e;
	    }
	} else
	
	if(set->name == pfx && sNode::is_base_of(nkType, basic_k)) {
	    // A type conversion.

	    if(a == NULL || a->next != NULL)
		error("%:type conversions must have exactly one argument",
		      pfx);
	    else if(!a->is(nkNamedAssocElem) || nNamedAssocElem(a)->formal)
		error("%:argument of type conversion must be a simple "
		      "expression", pfx);
	    else {
		nDeclaration d = set->single_decl();
		delete set;
		assert(d && d->is(nkType) && a->is(nkNamedAssocElem));
		nExpr e = nNamedAssocElem(a)->actual;
		return build_TypeConversion(pfx->lineno, nType(d), e);
	    }
	} else {
	    assert(n->is(nkIftsName));
	    // An array subscription.

	    nExpr prefix = make_appropriate(build_Expr(pfx, set, basic_k));
	    overload_resolution(prefix, nkArrayType);
	    return build_ArrayObjectRef(prefix, a);
	}

    } else if(n->is(nkAttributeName)) {
	return build_AttrNode(nAttributeName(n), set, basic_k);
	
    } else if(n->is(nkSelName)) {
	Id suffix = nSelName(n)->suffix;
	nName pfx = nSelName(n)->prefix;
	nExpr orig_prefix = build_Expr(pfx, set, basic_k);
	nExpr prefix = make_appropriate(orig_prefix);
	if(prefix != orig_prefix && vaul_name_eq(suffix, "all"))
	    return prefix;
	overload_resolution(prefix, nkRecordType);
	if(prefix == NULL)
	    return NULL;
	nType t = expr_type(prefix);
	if(t == NULL || !t->is(nkRecordType))
	    return NULL;
	nRecordType rt = nRecordType(t);
	for(nRecordElement re = rt->first_element; re; re = re->next) {
	    if(vaul_name_eq(re->id, suffix))
		return mRecordObjectRef(n->lineno, prefix, rt, re);
	}
	error("%:%n has no element named %s, it has", n, rt, suffix);
	for(nRecordElement _re = rt->first_element; _re; _re = _re->next)
	    info("%: %s: %n", _re, _re->id, _re->type);
	return NULL;

    } else if(set->name == n) {

	if(sNode::is_base_of(nkEnumLiteral, basic_k)) {
	    nAmbgEnumLitRef e = mAmbgEnumLitRef(n->lineno);
	    e->set = set;
	    return e;
	} else if(sNode::is_base_of(nkAlias, basic_k)) {
	    nDeclaration d = set->single_decl();
	    delete set;
	    if(d == NULL)
		return NULL;
	    assert(d->is(nkAlias));
	    return nAlias(d)->initial_value;
	} else if(!sNode::is_base_of(nkObject, basic_k) 
		  && !sNode::is_base_of(nkPhysicalUnit, basic_k)) {
	    delete set;
	    return mUnresolvedName(n->lineno, n);
	}
	
	nDeclaration d = set->single_decl();
	delete set;
	if(d == NULL)
	    return NULL;

	if(d->is(nkObject))
	    return mSimpleObjectRef(n->lineno, nObject(d), n);
	else if(d->is(nkPhysicalUnit))
	    return mPhysicalLiteralRef(n->lineno, NULL, nPhysicalUnit(d));
	else
	    assert(false);

    } else
	info("%:XXX - unimplemented expr name %n", n, n);

    delete set;
    return NULL;
}

nExpr psr::build_Expr(nName n, vaul_decl_set *set, nKind basic_k)
{
    nNode ea = build_Expr_or_Attr(n, set, basic_k);
    return validate_Expr(ea);
}

void psr::report_mismatched_subprog(nName pfx, vaul_decl_set *set,
				    nNamedAssocElem assocs)
{
    bool obvious = true;
    my_dynparray<nType_vector> assoc_types;

    for(nNamedAssocElem na = assocs; na; na = nNamedAssocElem(na->next)) {
	assert(na->is(nkNamedAssocElem));
	nType_vector *tv = ambg_expr_types(na->actual);
	assert(tv);
	assoc_types.add(tv);
	if(tv->size() > 5)
	    obvious = false;
    }
    if(obvious || verbose) {
	error("%:no declaration matches use as %n(%~", pfx, pfx);
	int i = 0;
	for(nNamedAssocElem na = assocs; na; na = nNamedAssocElem(na->next)) {
	    assert(na->is(nkNamedAssocElem));
	    if(na->formal)
		info("%~%n => %~", na->formal);
	    nType_vector *tv = assoc_types[i++];
	    if(tv->size() == 0)
		info("%~?%~");
	    if(tv->size() > 1)
		info("%~{ %~");
	    for(int j = 0; j < tv->size(); j++) {
		info("%~%n%~", (*tv)[j]);
		if(j < tv->size()-1)
		    info("%~ | %~");
	    }
	    if(tv->size() > 1)
		info("%~ }%~");
	    if(na->next)
		info("%~, %~");
	}
	info("%~)");
    } else
	error("%:no declaration of %n matches this unobvious use", pfx, pfx);
    for(int i = 0; i < assoc_types.size(); i++)
	delete assoc_types[i];
    set->show();
}

nArrayObjectRef psr::build_ArrayObjectRef(nExpr prefix, nGenAssocElem a)
{
    if(prefix == NULL || a == NULL)
	return NULL;

    nType t = expr_type(prefix);
    if(t == NULL)
	return NULL;

    if(!t->get_base()->is(nkArrayType)) {
	error("%:can't subscribe %n (not an array)", a, prefix);
	return NULL;
    }
    
    nArrayType at = nArrayType(t->get_base());
    if(at->first_index == NULL)
	return NULL;

    nArrayObjectRef aor = mArrayObjectRef(a->lineno, prefix, at, NULL);
    nIndexValue *ivp = &aor->first_index;
    for(nIndexType it = at->first_index; it; it = it->next) {
	if(a == NULL) {
	    error("%:too few subscripts for array %n", prefix, prefix);
	    return NULL;
	}
	if(!a->is(nkNamedAssocElem) || nNamedAssocElem(a)->formal) {
	    error("%:array subscripts must be simple expressions", a);
	    return NULL;
	}
	nNamedAssocElem na = nNamedAssocElem(a);
	overload_resolution(na->actual, it->index_type);
	*ivp = mIndexValue(a->lineno, NULL, na->actual);
	ivp = &(*ivp)->next;
	a = a->next;
    }
    if(a) {
	error("%:too many subscripts for array %n", a, prefix);
	return NULL;
    }
    
    return aor;
}

nSliceObjectRef psr::build_SliceObjectRef(nExpr prefix, nGenAssocElem slice)
{
    if(prefix == NULL || slice == NULL)
	return NULL;

    nType st = NULL;
    if(slice->is(nkRangeAssocElem)) {
	nRange r = nRangeAssocElem(slice)->range;
	nType rt = NULL;
	if(r->is(nkExplicitRange)) {
	    nExplicitRange er = nExplicitRange(r);
	    rt = find_index_range_type(er);
	    if(rt) {
		overload_resolution(er->first, rt);
		overload_resolution(er->last, rt);
	    } else
		r = NULL;
	} else
	    rt = nArrayRange(r)->type;
	st = mSubType(slice->lineno, NULL, rt, r, NULL);
    } else if(slice->is(nkSubtypeAssocElem))
	st = nSubtypeAssocElem(slice)->type;
    else
	assert(false);

    assert(slice->next == NULL);

    nType t = expr_type(prefix);
    if(t == NULL)
	return NULL;

    if(!t->get_base()->is(nkArrayType)) {
	error("%:can't take slice of %n (not an array)", slice, prefix);
	return NULL;
    }

    nArrayType at = nArrayType(t->get_base());
    if(at->first_index && at->first_index->next)
	error("%:sliced arrays must be one-dimensional", slice);

    if(st && at->first_index && at->first_index->index_type &&
       st->get_base() != at->first_index->index_type->get_base()) {
	error("%:type of slice bounds does not match array index type");
	st = NULL;
    }

    return mSliceObjectRef(slice->lineno, prefix, at, st);
}

int psr::try_one_association(nNamedAssocElem a, nInterface f)
{
    return constrain(a->actual, f->type, nkInvalid);
}

static nInterface find_interface_by_id(nInterface first, Id id)
{
    for(nInterface f = first; f; f = f->next_element)
	if(vaul_name_eq(f->id, id))
	    return f;
    return NULL;
}

static nSimpleName get_interface_simple_name(nName n)
{
    if(n->is(nkSimpleName))
	return nSimpleName(n);
    else if(n->is(nkSelName))
	return get_interface_simple_name(nSelName(n)->prefix);
    else if(n->is(nkIftsName))
	return get_interface_simple_name(nIftsName(n)->prefix);
    else
	return NULL;
}

nDeclaration psr::grab_formal_conversion(nNamedAssocElem assoc,
					 nInterface formals,
					 int *formal_cost,
					 nInterface *converted_formal)
{
    if(assoc->ifts_decls == NULL)
	return NULL;

    // We have either function(ifc) or type(ifc) with ifc being
    // the only argument.
    
    assert(assoc->formal->is(nkIftsName));
    nIftsName ifts = nIftsName(assoc->formal);
    assert(ifts->assoc && ifts->assoc->is(nkNamedAssocElem));
    nNamedAssocElem arg = nNamedAssocElem(ifts->assoc);
    assert(arg->next == NULL);
    nKind kind = assoc->ifts_kind;
    nSimpleName sn = assoc->ifts_arg_name;
    assert(sn);

    if(nInterface f = find_interface_by_id(formals, sn->id)) {
	nDeclaration conversion = NULL;
	int fcost;
	if(sNode::is_base_of(nkFunction, kind)) {
	    // See, if we can resolve the function.

	    nSimpleObjectRef or = mSimpleObjectRef(assoc->lineno,
						   f, sn);
	    nNamedAssocElem na = mNamedAssocElem(assoc->lineno,
						 NULL,
						 arg->formal,
						 or);
	    nAmbgCall ac = mAmbgCall(assoc->lineno, na);
	    ac->set = assoc->ifts_decls;
	    // XXX - should this be based on the actual type?
	    fcost = constrain(ac, NULL, nkType);
	    if(fcost >= 0) {
		conversion = assoc->ifts_decls->single_decl(false);
		assert(conversion && conversion->is(nkFunction));
	    }
	} else if(sNode::is_base_of(nkType, kind)) {
	    conversion = assoc->ifts_decls->single_decl(false);
	    assert(conversion && conversion->is(nkType));
	    fcost = 0;
	} else
	    assert(false);

	info("+++ - %n converted by %n (cost %d)", f, conversion, fcost);

	if(formal_cost)
	    *formal_cost = fcost;
	if(converted_formal)
	    *converted_formal = f;
	return conversion;
    }

    return NULL;
}

static nType get_partial_formal_type(nName name, nInterface formal,
				     nSubarrayType subarray_storage = NULL)
{
    if(name->is(nkSimpleName))
	return formal->type;
    else if(name->is(nkSelName)) {
	nType ptype = get_partial_formal_type(nSelName(name)->prefix, formal);
	if(!ptype || !ptype->is(nkRecordType))
	    return NULL;
	for(nRecordElement re = nRecordType(ptype)->first_element; re;
	    re = re->next)
	    if(vaul_name_eq(re->id, nSelName(name)->suffix))
		return re->type;
	return NULL;
    } else if(name->is(nkIftsName)) {
	nType ptype = get_partial_formal_type(nIftsName(name)->prefix, formal);
	if(!ptype)
	    return NULL;
	ptype = ptype->get_base();
	if(!ptype->is(nkArrayType))
	    return NULL;

	nGenAssocElem arg = nIftsName(name)->assoc;
	if(arg == NULL)
	    return NULL;
	if(arg->is(nkNamedAssocElem)) {
	    nArrayType at = nArrayType(ptype);
	    nIndexType it = at->first_index;
	    while(it && arg) {
		assert(arg->is(nkNamedAssocElem));
		arg = arg->next;
		it = it->next;
	    }
	    if(arg)
		return NULL;
	    if(it) {
		if(subarray_storage) {
		    subarray_storage->lineno = at->lineno;
		    subarray_storage->id = NULL;
		    subarray_storage->first_index = it;
		    subarray_storage->element_type = at->element_type;
		    subarray_storage->complete_type = at;
		    return subarray_storage;
		} else
		    return NULL;
	    } else
		ptype = nArrayType(ptype)->element_type;
	} else {
	    // slices must be one-dimensional, this has already been checked
	    assert(arg->next == NULL);
	}
	return ptype;
    } else
	assert(false);
}

int psr::try_association(nNamedAssocElem actuals, nInterface formals)
{
    int cost = 0;
    nInterface formal;
    nNamedAssocElem a;
    my_dynarray<nInterface> associated_formals;

    // quick check to avoid unecessary recursion for the most common cases

    int n_formals = 0, n_opt_formals = 0;
    for(nInterface f = formals; f; f = f->next_element) {
	n_formals++;
	if(f->initial_value)
	    n_opt_formals++;
    }

    int n_actuals = 0;
    bool valid = true;
    for(nNamedAssocElem ae = actuals; ae; ae = nNamedAssocElem(ae->next)) {
	assert(ae->is(nkNamedAssocElem));
	if(ae->formal && !ae->formal->is(nkSimpleName)) {
	    valid = false;
	    break;
	}
	n_actuals++;
	if(n_actuals > n_formals)
	    break;
    }

    if(valid && 
       (n_actuals > n_formals || n_actuals < n_formals - n_opt_formals)) {
	// info("+++ - shortcut");
	return -1;
    }

    // first, the unnamed ones

    formal = formals;
    a = actuals;
    while(formal && a && a->formal == NULL) {
	assert(a->is(nkNamedAssocElem));

	int c = try_one_association(a, formal);
	if(c < 0)
	    return c;
	cost += c;
	associated_formals.add(formal);
	a = nNamedAssocElem(a->next);
	formal = formal->next_element;
    }

    if(formal == NULL && a != NULL)
	return -1;

    // and now the named ones

    while(a) {
	assert(a->is(nkNamedAssocElem));
	if(a->formal == NULL)
	    break;

	// There are several cases here for the formal:
	//
	// interface_name
	// function(interface_name)
	// type(interface_name)
	//
	// interface_name.record_item
	// interface_name(array_index, ...)
	//   and combinations
	//
	// we try all cases and if more than one is valid, it's an error

	int formal_cost = -1;
	nInterface converted_formal = NULL;

	nDeclaration conv = grab_formal_conversion(a, formals,
						   &formal_cost,
						   &converted_formal);
	if(conv) {
	    nType actual_type;
	    if(conv->is(nkFunction))
		actual_type = nFunction(conv)->return_type;
	    else if(conv->is(nkType))
		actual_type = nType(conv);
	    else
		assert(false);
	    formal_cost += constrain(a->actual, actual_type, nkInvalid);
	    associated_formals.add(converted_formal);
	}

	nSimpleName sn = get_interface_simple_name(a->formal);
	assert(sn);
	if(nInterface f = find_interface_by_id(formals, sn->id)) {
	    if(converted_formal) {
#if 0
		error("%:%n is ambigous as a formal name",
		      a->formal, a->formal);
		info("%: it could apply to both %n and %n",
		     a->formal, converted_formal, f);
#endif
		return -1;
	    }

	    assert(formal_cost < 0);

	    sSubarrayType sat;
	    if(nType atype = get_partial_formal_type(a->formal, f, &sat)) {
		// info("+++ - type of %n is %n", a->formal, atype);
		formal_cost = constrain(a->actual, atype, nkInvalid);
		associated_formals.add(f);
	    } else
		return -1;
	}

	if(formal_cost < 0)
	    return formal_cost;
	cost += formal_cost;
	a = nNamedAssocElem(a->next);
    }

    for(nInterface _f = formals; _f; _f = _f->next_element) {
	if(!_f->initial_value && !associated_formals.contains(_f))
	    return -1;
    }

    return cost;
}

int psr::try_array_subscription(nArrayType at, nGenAssocElem a)
{
    int cost = 0;
    for(nIndexType it = at->first_index; it; it = it->next) {
	if(a == NULL || !a->is(nkNamedAssocElem) || nNamedAssocElem(a)->formal)
	    return -1;
	cost += constrain(nNamedAssocElem(a)->actual,
			  it->index_type, nkInvalid);
	a = a->next;
    }
    if(a)
	return -1;
    return cost;
}

bool psr::associate_one(nAssociation &tail,
			nInterface f, nDeclaration fconv,
			nExpr a, nDeclaration aconv)
{
    for(nAssociation as = tail; as; as = as->next)
	if(as->formal == f) {
	    error("%:multiple actuals for formal %n", a, f);
	    return false;
	}

    nType actual_type;
    if(fconv) {
	if(fconv->is(nkFunction))
	    actual_type = nFunction(fconv)->return_type;
	else if(fconv->is(nkType))
	    actual_type = nType(fconv);
	else
	    assert(false);
    } else
	actual_type = f->type;

    overload_resolution(a, actual_type);
    if(a == NULL)
	return false;

    // XXX - check modes and classes

#if 0
    if(f->object_class == ObjClass_Variable) {
	if(!e->is(nkObjectRef) || !nObjectRef(e)->is_variable()) {
	    error("%:%n is not a variable", e, e);
	    return NULL;
	}
    } else if(f->object_class == ObjClass_Signal) {
	if(!e->is(nkObjectRef) || !nObjectRef(e)->is_signal()) {
	    error("%:%n is not a signal", e, e);
	    return NULL;
	}
    } else if(f->object_class == ObjClass_File) {
	info("%:XXX - can't associate to file", a);
	return NULL;
    }
#endif

    nAssociation assoc = mAssociation(NULL, f, fconv, a, aconv);
    if(assoc) {
	assoc->next = tail;
	tail = assoc;
    }
    return true;
}

nExpr psr::add_partial_choice(nExpr &pactual, nName formal, nExpr actual)
{
    int lno = formal->lineno;
    nExpr *ep = NULL;

    if(formal->is(nkSimpleName))
	ep = &pactual;

    else if(formal->is(nkSelName)) {
	nExpr px = add_partial_choice(pactual, nSelName(formal)->prefix, NULL);
	if(px == NULL)
	    return NULL;
	assert(px->is(nkArtificialAmbgAggregate));
	nAmbgAggregate aggr = nAmbgAggregate(px);
	Id sfx = nSelName(formal)->suffix;
	nElemAssoc ea;
	for(ea = aggr->first_assoc; ea; ea = ea->next) {
	    if(ea->first_choice && ea->first_choice->next == NULL 
	       && ea->first_choice->is(nkNamedChoice)) {
		nNamedChoice nc = nNamedChoice(ea->first_choice);
		if(nc->name && nc->name->is(nkSimpleName)
		   && vaul_name_eq(nSimpleName(nc->name)->id, sfx))
		    break;
	    }
	}
	if(ea == NULL) {
	    nChoice c = mNamedChoice(lno, NULL, mSimpleName(lno, sfx));
	    ea = mElemAssoc(lno, aggr->first_assoc, c, NULL);
	    aggr->first_assoc = ea;
	}
	ep = &ea->actual;

    } else if(formal->is(nkIftsName)) {
	nGenAssocElem arg = nIftsName(formal)->assoc;
	if(arg == NULL)
	    return NULL;

	nExpr px = add_partial_choice(pactual,
				      nIftsName(formal)->prefix, NULL);
	if(px == NULL)
	    return NULL;
	assert(px->is(nkArtificialAmbgAggregate));
	nAmbgAggregate aggr = nAmbgAggregate(px);

	while(arg) {
	    nChoice c;
	    if(arg->is(nkNamedAssocElem))
		c = mExprChoice(lno, NULL, nNamedAssocElem(arg)->actual);
	    else {
		c = mRangeChoice(lno, NULL, arg);
		nRangeChoice(c)->actual_is_slice = true;
	    }
	    nElemAssoc ea = mElemAssoc(lno, aggr->first_assoc, c, NULL);
	    aggr->first_assoc = ea;
	    ep = &ea->actual;

	    arg = arg->next;
	    if(arg) {
		aggr = mArtificialAmbgAggregate(lno, NULL);
		*ep = aggr;
	    }
	}

    } else
	assert(false);

    if(*ep) {
	if(actual || !(*ep)->is(nkArtificialAmbgAggregate)) {
	    error("%:multiple actuals for %n", formal, formal);
	    return NULL;
	}
    } else
	*ep = actual? actual : mArtificialAmbgAggregate(lno, NULL);

    return *ep;
}

nAssociation psr::associate(nNamedAssocElem actuals, nInterface formals,
			    bool complete)
{
    nAssociation tail = NULL;
    nNamedAssocElem a;
    nInterface formal;

    formal = formals;
    a = actuals;
    while(formal && a && a->formal == NULL) {
	assert(a->is(nkNamedAssocElem));

	if(!associate_one(tail, formal, NULL, a->actual, NULL))
	    return NULL;
	a = nNamedAssocElem(a->next);
	formal = formal->next_element;
    }

    if(formal == NULL && a != NULL) {
	error("%:too many actuals", a);
	return NULL;
    }

    nInterface pformal = NULL;
    nExpr pactual = NULL;

    while(a) {
	assert(a->is(nkNamedAssocElem) && a->formal);

	nInterface f;
	nSimpleName sn = get_interface_simple_name(a->formal);
	nDeclaration fconv = grab_formal_conversion(a, formals, NULL, &f);
	if(fconv) {
	    if(!associate_one(tail, f, fconv, a->actual, NULL))
		return NULL;
	} else
	    f = find_interface_by_id(formals, sn->id);

	if(f == NULL) {
	    error("%:no formal with name %n", a, sn);
	    return NULL;
	}

	if(pformal && (f != pformal || fconv)) {
	    if(!associate_one(tail, pformal, NULL, pactual, NULL))
		return NULL;
	    pformal = NULL;
	    pactual = NULL;
	}

	if(!fconv) {
	    pformal = f;
	    if(!add_partial_choice(pactual, a->formal, a->actual))
		return NULL;
	    // info("+++ - added %n, resulting in %n", a->formal, pactual);
	}

	a = nNamedAssocElem(a->next);
    }

    if(pformal) {
	if(!associate_one(tail, pformal, NULL, pactual, NULL))
	    return NULL;
    }

    if(complete) {
	bool success = true;
	for(nInterface f = formals; f; f = f->next_element) {
	    if(f->initial_value)
		continue;
	    nAssociation as;
	    for(as = tail; as; as = as->next)
		if(as->formal == f)
		    break;
	    if(as == NULL) {
		error("%:formal %n has not been associated", actuals, f);
		success = false;
	    }
	}
	if(!success)
	    return NULL;
    }

    return reverse(tail);
}

struct psr::filter_return_closure {
    vaul_parser *self;
    nType t;
    nKind k;
    nNamedAssocElem a;
};

int psr::filter_return_stub(nDeclaration d, void *cl)
{
    filter_return_closure *rc = (filter_return_closure *)cl;
    return rc->self->filter_return(d, rc);
}

nType psr::expr_type(nExpr e)
{
    if(e == NULL)
	return NULL;

    if(e->is(nkFunctionCall))
	return nFunctionCall(e)->func? nFunctionCall(e)->func->return_type
				     : NULL;
    else if(e->is(nkObjectRef))
	return nObjectRef(e)->get_type();
    else if(e->is(nkLiteralRef)) {
	char *val = nLiteralRef(e)->value;
	if(e->is(nkPhysicalLiteralRef))
	    return nPhysicalLiteralRef(e)->unit?
		nPhysicalLiteralRef(e)->unit->type : NULL;
	else if(is_integer_literal(val))
	    return std->universal_integer;
	else if(is_real_literal(val))
	    return std->universal_real;
	else {
	    info("XXX - no %s yet", nLiteralRef(e)->value);
	    return NULL;    // XXX
	}
    } else if(e->is(nkArrayLiteralRef))
	return nArrayLiteralRef(e)->type;
    else if(e->is(nkEnumLitRef))
	return nEnumLitRef(e)->value ? nEnumLitRef(e)->value->type : NULL;
    else if(e->is(nkAttrFunctionCall))
	return nAttrFunctionCall(e)->attr_type;
    else if(e->is(nkArrayAttr))
	return nArrayAttr(e)->attr_type;
    else if(e->is(nkTypeConversion))
	return nTypeConversion(e)->target_type;
    else if(e->is(nkQualifiedExpr))
	return nQualifiedExpr(e)->type;
    else if(e->is(nkNullExpr))
	return nNullExpr(e)->type;
    else if(e->is(nkNewExpr))
	return nNewExpr(e)->type;
    else {
	info("%:XXX - %n (%s) has no type", e, e, e->type_id());
	return NULL;
    }
}

static void get_ambg_types(nDeclaration d, void *cl)
{
    nType_vector *vc = (nType_vector *)cl;
    if(d->is(nkFunction) && nFunction(d)->return_type)
	vc->add_uniq(nFunction(d)->return_type);
    else if(d->is(nkEnumLiteral) && nEnumLiteral(d)->type)
	vc->add_uniq(nEnumLiteral(d)->type);
    else
	fprintf(stderr,  "XXX - %n (%s) has no ambg_type\n", d, d->type_id());
}

struct cat_closure {
    nType_vector *types;
    nExpr expr;
};

static bool is_interesting_array_type(nDeclaration d)
{
    return (d->is(nkArrayType) && nArrayType(d)->id != NULL)
	|| (d->is(nkSubType) && nSubType(d)->base->is(nkArrayType)
	    && nSubType(d)->base->id == NULL);
}

static void collect_ambg_types(nDeclaration d, void *_cl)
{
    cat_closure *cl = (cat_closure *)_cl;

    if(cl->expr->is(nkAmbgArrayLitRef)) {
	nAmbgArrayLitRef alit = nAmbgArrayLitRef(cl->expr);

	if(is_interesting_array_type(d)
	&& array_literal_conversion_cost(alit, nType(d), nkInvalid) >= 0)
	    cl->types->add_uniq(nType(d));
    } else if(cl->expr->is(nkAmbgAggregate)) {
	nAmbgAggregate aggr = nAmbgAggregate(cl->expr);

	if((is_interesting_array_type(d) || d->is(nkRecordType))
	   && aggregate_conversion_cost(aggr, nType(d), nkInvalid) >= 0)
	    cl->types->add_uniq(nType(d));
    } else if(cl->expr->is(nkAmbgNullExpr)) {
	if(d->is(nkAccessType))
	    cl->types->add_uniq(nType(d));
    } else
	assert(false);
}

nType_vector *psr::ambg_expr_types(nExpr e)
{
    nType_vector *types = new nType_vector;

    if(e->is(nkAmbgCall) || e->is(nkAmbgEnumLitRef)) {
	vaul_decl_set *d = e->is(nkAmbgCall)? nAmbgCall(e)->set :
	                                      nAmbgEnumLitRef(e)->set;
	d->refresh();
	d->iterate(get_ambg_types, types);
    } else if(e->is(nkAmbgArrayLitRef) || e->is(nkAmbgAggregate)
	      || e->is(nkAmbgNullExpr)) {
	// brute force, what else?
	cat_closure cl = { types, e };
	visit_decls(collect_ambg_types, &cl);
	info("+++ - collected %d types", types->size());
    } else if(e->is(nkUnresolvedName))
	overload_resolution(e, nkType); // just for the error message
    else if(nType t = expr_type(e))
	types->add(t);
    
    return types;
}

int psr::conversion_cost(nNode target, nType should_t, nKind should_k)
{
    if(target == NULL)
	return 0;

    if(should_t) {
	should_t = should_t->get_base();
	should_k = should_t->kind();
    }

    bool impl_conv = false;
    nType is_t = NULL;

    if(target->is(nkFunction)) {
	nFunction f = nFunction(target);
	is_t = f->return_type;
	if(f->is(nkPredefOp) && vaul_name_eq("\"/\"", f->id)
	&& f->first_formal
	&& f->first_formal->type->get_base()->is(nkPhysicalType)
	&& f->first_formal->next_element
	&& f->first_formal->next_element->type->get_base()->is(nkPhysicalType))
	    impl_conv = true;
    } else if(target->is(nkLiteralRef)) {
	is_t = expr_type(nLiteralRef(target));
	if(is_t == std->universal_integer || is_t == std->universal_real)
	    impl_conv = true;
    } else if(target->is(nkAmbgArrayLitRef)) {
	return array_literal_conversion_cost(nAmbgArrayLitRef(target),
					     should_t, should_k);
    } else if(target->is(nkArrayAttr_LENGTH)) {
	is_t = nArrayAttr_LENGTH(target)->attr_type;
	impl_conv = true;
    } else if(target->is(nkAttr_POS)) {
	is_t = nAttr_POS(target)->attr_type;
	impl_conv = true;
    } else if(target->is(nkAmbgNullExpr)) {
	return sNode::is_base_of(should_k, nkAccessType)? 0 : -1;
    } else if(target->is(nkNewExpr) && should_t && should_t->is(nkAccessType)) {
	nType new_type = nNewExpr(target)->type;
	assert(new_type->is(nkAccessType));
	return conversion_cost(nAccessType(new_type)->designated,
			       nAccessType(should_t)->designated, nkInvalid);
    } else if(target->is(nkExpr)) {
	is_t = expr_type(nExpr(target));
    } else {
	assert(target->is(nkType));
	is_t = nType(target);
    }
	
    if(is_t == NULL)
	return 0;

    is_t = is_t->get_base();

    // exact match
    if(should_t? is_t == should_t : is_t->is(should_k))
	return 0;

    // implicit conversion from universal types
    if(impl_conv) {
	if(should_t)
	    should_k = should_t->kind();
	if((is_t->is(nkUniversalInteger)
	    && sNode::is_base_of(nkIntegerType, should_k))
	|| (is_t->is(nkUniversalReal)
	    && sNode::is_base_of(nkFloatingType, should_k)))
	    return 1;
    }

#if 0
    // implicit conversion from access types
    if(is_t->is(nkAccessType)) {
	// info("+++ - converting %n to %n", is_t,
	//      nAccessType(is_t)->designated);
	return conversion_cost(nAccessType(is_t)->designated,
			       should_t, should_k);
    }
#endif

    // no match
    return -1;
}

static int array_literal_conversion_cost(nAmbgArrayLitRef l, nType t, nKind k)
{
    if(t == 0)
	return sNode::is_base_of(nkArrayType, k)? 0 : -1;

    if(!t->is(nkArrayType))
	return -1;
    nArrayType at = nArrayType(t);
    if(at->first_index && at->first_index->next)
	return -1;
    nType et = at->element_type;
    if(et == NULL)
	return -1;
    et = et->get_base();
    if(!et->is(nkEnumType))
	return -1;
    for(const char *cp = l->value+1; cp[1]; cp++) {
	nEnumLiteral el;
	for(el = nEnumType(et)->first; el; el = el->next) {
	    if(el->id && el->id[0] == '\'' && el->id[1] == *cp)
		break;
	}
	if(el == NULL)
	    return -1;
    }
    return 0;
}

static nSimpleName choice_simple_name(nChoice c)
{
    nName n = NULL;
    if(c->is(nkNamedChoice))
	n = nNamedChoice(c)->name;
    else if(c->is(nkExprChoice)) {
	if(nExpr e = nExprChoice(c)->expr) {
	    if(e->is(nkUnresolvedName))
		n = nUnresolvedName(e)->name;
	    else if(e->is(nkSimpleObjectRef))
		n = nSimpleObjectRef(e)->name;
	    else
		return NULL;
	}
    }
    if(n == NULL || !n->is(nkSimpleName))
	return NULL;
    return nSimpleName(n);
}

static nRecordElement choice_elem(nRecordType t, nChoice c)
{
    nSimpleName n = choice_simple_name(c);
    if(n == NULL)
	return NULL;
    for(nRecordElement re = t->first_element; re; re = re->next)
	if(vaul_name_eq(re->id, n->id))
	    return re;
    return NULL;
}

int psr::choice_conversion_cost(nChoice c, nExpr actual, nType t, nKind k)
{
    if(sNode::is_base_of(nkArrayType, k)) {
	int c1 = 0, c2 = 0;
	nArrayType at = NULL;
	if(t) {
	    assert(t->is(nkArrayType));
	    nArrayType at = nArrayType(t);
	    c1 = constrain(actual, at->element_type, nkInvalid);
	    if(c1 < 0)
		return c1;
	}
	if(c != NULL) {
	    if(!c->is(nkExprChoice))
		return -1;
	    nExpr e = nExprChoice(c)->expr;
	    if(e == NULL)
		return c1;
	    if(at) {
		if(at->first_index == NULL)
		    return -1;
		c2 = constrain(e, at->first_index->index_type, nkInvalid);
		if(c2 < 0)
		    return c2;
		return c1 + c2;
	    } else
		return e->is(nkUnresolvedName)? -1 : c1;
	}
    } else if(sNode::is_base_of(nkRecordType, k)) {
	nSimpleName n = choice_simple_name(c);
	if(n == NULL)
	    return -1;
	if(t) {
	    assert(t->is(nkRecordType));
	    nRecordType rt = nRecordType(t);
	    for(nRecordElement re = rt->first_element; re; re = re->next)
		if(vaul_name_eq(re->id, n->id))
		    return constrain(actual, re->type, nkInvalid);
	    return -1;
	} else
	    return 0;
    } else
	return -1;
}

static int aggregate_conversion_cost(nAmbgAggregate a, nType t, nKind k)
{
    if(t) {
	t = t->get_base();
	k = t->kind();
    }

#if 1
    return sNode::is_base_of(nkCompositeType, k)? 0 : -1;
#else // no need to be this clever.
    int cost = 0;

    for(nElemAssoc ea = a->first_assoc; ea; ea = ea->next)
	for(nChoice c = ea->first_choice; c; c = c->next) {
	    int cc = choice_conversion_cost(c, ea->actual, t, k);
	    if(cc < 0)
		return cc;
	    cost += cc;
	}
    return cost;
#endif
}

nAggregate psr::build_Aggregate(nAmbgAggregate aggr, nType t)
{
    if(aggr == NULL || t == NULL)
	return NULL;

    nType bt = t->get_base();

    if(bt->is(nkRecordType)) {
	assert(bt == t);
	nRecordType rt = nRecordType(t);
	nRecordAggregate ra = mRecordAggregate(aggr->lineno, t, NULL);
	nRecAggrAssoc *raa_tail = &ra->first_assoc;
	
	// associations without choice
	//
	nElemAssoc ea = aggr->first_assoc;
	nRecordElement re = rt->first_element;
	while(ea && ea->first_choice == NULL && re) {
	    *raa_tail = mRecAggrAssoc(ea->lineno, NULL, re, ea->actual);
	    overload_resolution((*raa_tail)->actual, re->type);
	    raa_tail = &(*raa_tail)->next;
	    re = re->next;
	    ea = ea->next;
	}
	if(re == NULL && ea != NULL) {
	    error("%:too many elements in record aggregate of type %n",
		  ea, rt);
	    return ra;
	}

	// associations with choices
	//
	while(ea && ea->first_choice) {
	    // find the type of all choosen elements
	    nType elem_type = NULL;
	    for(nChoice c = ea->first_choice; c; c = c->next) {
		nRecordElement re = choice_elem(rt, c);
		if(re) {
		    for(nRecAggrAssoc a = ra->first_assoc; a; a = a->next)
			if(a->elem == re) {
			    error("%:%n::%s has already been associated",
				  c, rt, re->id);
			    if(a->lineno != c->lineno)
				info("%: here", a);
			    continue;
			}
		    if(elem_type && elem_type != re->type)
			error("%:all elements of an association must be of"
			      " the same type", c);
		    elem_type = re->type;
		} else {
		    error("%:%n has no element named %n, it has:", c, rt, c);
		    for(nRecordElement _re = rt->first_element; _re; 
			_re = _re->next)
			info("%: %s: %n", _re, _re->id, _re->type);
		}

	    }
	    if(elem_type) {
		overload_resolution(ea->actual, elem_type);
		for(nChoice c = ea->first_choice; c; c = c->next) {
		    nRecordElement re = choice_elem(rt, c);
		    *raa_tail = mRecAggrAssoc(c->lineno, NULL, re, ea->actual);
		    raa_tail = &(*raa_tail)->next;
		}
	    }
	    ea = ea->next;
	}

	if(ea)
	    error("%:associations without choices must precede all the others",
		  ea);

	// check, wether all elements have been choosen
	//
	for(nRecordElement _re = rt->first_element; _re; _re = _re->next) {
	    bool found = false;
	    for(nRecAggrAssoc raa = ra->first_assoc; raa; raa = raa->next)
		if(raa->elem == _re) {
		    found = true;
		    break;
		}
	    if(!found)
		error("%:%n::%s has not been associated", aggr, rt, _re->id);
	}
	    
	return ra;

    } else if(bt->is(nkArrayType)) {
	nArrayType at = nArrayType(bt);

	if(at->first_index == NULL)
	    return NULL;

	nArrayAggregate aa = mArrayAggregate(aggr->lineno, t, NULL);
	nArrAggrAssoc *aaa = &aa->first_assoc;

	nType actual_type;
	if(at->first_index->next == NULL)
	    actual_type = at->element_type;
	else {
	    actual_type = mSubarrayType(at->lineno, NULL,
					at->first_index->next,
					at->element_type,
					(at->is(nkSubarrayType)?
					 nSubarrayType(at)->complete_type :
					 at));
	}

	nElemAssoc ea = aggr->first_assoc;
	while(ea && ea->first_choice == NULL) {
	    overload_resolution(ea->actual, actual_type);
	    *aaa = mSingleArrAggrAssoc(ea->lineno, NULL, ea->actual, NULL);
	    aaa = &(*aaa)->next;
	    ea = ea->next;
	}

	while(ea && ea->first_choice) {
	    nChoice c = ea->first_choice;
	    nType atype;
	    if(c->is(nkRangeChoice) && nRangeChoice(c)->actual_is_slice)
		atype = at;
	    else
		atype = actual_type;
	    overload_resolution(ea->actual, atype);
	    for(; c; c = c->next) {
		if(c->is(nkExprChoice)) {
		    assert(atype == actual_type);
		    nExpr e = nExprChoice(c)->expr;
		    overload_resolution(e, at->first_index->index_type);
		    *aaa = mSingleArrAggrAssoc(ea->lineno, NULL, ea->actual,
					       e);
		    aaa = &(*aaa)->next;
		} else if(c->is(nkRangeChoice)) {
		    assert(atype == (nRangeChoice(c)->actual_is_slice?
				     at : actual_type));
		    nGenAssocElem gae = nRangeChoice(c)->range;
		    nRange range = NULL;
		    if(gae->is(nkRangeAssocElem)) {
			nRange r = nRangeAssocElem(gae)->range;
			if(r->is(nkExplicitRange)) {
			    nExplicitRange er = nExplicitRange(r);
			    nType rt = find_index_range_type(er);
			    if(rt) {
				overload_resolution(er->first, rt);
				overload_resolution(er->last, rt);
				range = r;
			    }
			} else
			    range = r;
		    } else
			error("%:%n invalid as array aggregate choice",
			      gae, gae);

		    if(nRangeChoice(c)->actual_is_slice)
			*aaa = mSliceArrAggrAssoc(ea->lineno, NULL,
						  ea->actual, range);
		    else
			*aaa = mRangeArrAggrAssoc(ea->lineno, NULL,
						  ea->actual, range);

		    aaa = &(*aaa)->next;
		} else if(c->is(nkOthersChoice))
		    *aaa = mOthersArrAggrAssoc(ea->lineno, NULL, ea->actual);
		else
		    error("%:%n invalid as array aggregate choice", c, c);
	    }
	    ea = ea->next;
	}
	if(ea)
	    error("%:associations without choices must precede all the others",
		  ea);

	return aa;

    } else
	assert(false);
}
    
static int is_array_func(nFunction f, nNamedAssocElem a)
{
    return f->return_type && f->return_type->is(nkArrayType)
	&& f->first_formal == NULL && a != NULL;
}

int psr::filter_return(nDeclaration d, filter_return_closure *rc)
{
    if(d->is(nkFunction)) {
	nFunction f = nFunction(d);
	int c1, c2;
	if(is_array_func(f, rc->a)) {
	    c1 = conversion_cost(nArrayType(f->return_type)->element_type,
				 rc->t, rc->k);
	    if(c1 >= 0)
		c2 = try_array_subscription(nArrayType(f->return_type),
					    rc->a);
	} else {
	    c1 = conversion_cost(f, rc->t, rc->k);
	    if(c1 >= 0)
		c2 = try_association(rc->a, f->first_formal);
	}
	if(c1 < 0 || c2 < 0)
	    return -1;
	return c1+c2;
    } else if(d->is(nkProcedure)) 
	return try_association(rc->a, nProcedure(d)->first_formal);
    else if(d->is(nkEnumLiteral))
	return conversion_cost(nEnumLiteral(d)->type, rc->t, rc->k);
    else
	return -1;
}

static nName expr_name(nExpr e)
{
    if(e->is(nkUnresolvedName))
	return nUnresolvedName(e)->name;
    else if(e->is(nkSimpleObjectRef))
	return nSimpleObjectRef(e)->name;
    else if(e->is(nkAmbgCall)) {
	vaul_decl_set *set = nAmbgCall(e)->set;
	return set? set->name : NULL;
    } else
	return NULL;
}

bool psr::prepare_named_assocs(nGenAssocElem gen)
{
    bool named = false;
    bool success = true;

    for(nGenAssocElem a = gen; a; a = a->next) {
	if(a->is(nkNamedAssocElem)) {
	    nNamedAssocElem na = nNamedAssocElem(a);

	    if(na->formal)
		named = true;
	    else if(named) {
		error("%:unnamed associations must preced the named ones", na);
		return false;
	    }

	    if(na->formal) {
		nName f = na->formal;

		na->ifts_decls = NULL;
		if(f->is(nkIftsName)) {
		    nIftsName in = nIftsName(f);
		    info("%:+++ - function/type/array element", f);

		    na->ifts_arg_name = NULL;
		    if(in->assoc && in->assoc->next == NULL
		       && in->assoc->is(nkNamedAssocElem)) {
			nExpr a = nNamedAssocElem(in->assoc)->actual;
			if(a) {
			    nName an = expr_name(a);
			    if(an->is(nkSimpleName))
				na->ifts_arg_name = nSimpleName(an);
			}
		    }

		    if(na->ifts_arg_name) {
			na->ifts_decls = new vaul_decl_set(this);
			find_decls(*(na->ifts_decls), f);

			if(!na->ifts_decls->multi_decls(false)) {
			    info("%:      but no matches (which is fine)", f);
			    delete na->ifts_decls;
			    na->ifts_decls = NULL;
			} else {
			    na->ifts_kind = nkDeclaration;
			    na->ifts_decls->iterate(iterate_for_kind,
						    &na->ifts_kind);
			    if(!sNode::is_base_of(nkFunction, na->ifts_kind)
			       && !sNode::is_base_of(nkType, na->ifts_kind)) {
				info("%:      but not a function/type (%s)",
				     f, sNode::type_id(na->ifts_kind));
				delete na->ifts_decls;
				na->ifts_decls = NULL;
			    }
			}
		    } else
			info("%:      but not exactly one simple argument", f);
		}

		if(get_interface_simple_name(f) == NULL) {
		    error("%:%n does not contain an interface name",
			  na->formal, na->formal);
		    success = false;
		}
	    }

	} else {
	    error("%:%n can not be used in an association", a, a);
	    success = false;
	}
    }

    return success;
}

bool psr::validate_gen_assocs(nGenAssocElem assocs)
{
    for(nGenAssocElem a = assocs; a; a = a->next) {
	if(!a->is(nkNamedAssocElem)) {
	    if(a != assocs || a->next != NULL) {
		error("%:slices must be one-dimensional", a);
		return false;
	    }
	}
    }
    return true;
}

int psr::constrain(nExpr e, nType t, nKind k, bool pre)
{
    if(e == NULL || (t == NULL && k == nkInvalid))
	return 0;

    if(e->is(nkAmbgCall)) {
	vaul_decl_set *s = nAmbgCall(e)->set;
	s->refresh();
	filter_return_closure rc = { this, t, k, nAmbgCall(e)->first_actual };
	s->filter(filter_return_stub, &rc);
	if(!pre)
	    return s->retain_lowcost();
	else {
	    s->invalidate_pot_invalids();
	    return s->multi_decls(false)? 0 : -1;
	}
    } else if(e->is(nkAmbgEnumLitRef)) {
	vaul_decl_set *s = nAmbgEnumLitRef(e)->set;
	s->refresh();
	filter_return_closure rc = { this, t, k, NULL };
	s->filter(filter_return_stub, &rc);
	return s->retain_lowcost();
    } else if(e->is(nkAmbgAggregate))
	return aggregate_conversion_cost(nAmbgAggregate(e), t, k);
    else if(e->is(nkUnresolvedName))
	return 0;
    else 
	return conversion_cost(e, t, k);
}

bool psr::try_overload_resolution(nExpr e, nType t, nKind k)
{
    return constrain(e, t, k) >= 0;
}

static char *type_string(nKind k)
{
    switch(k) {
    case nkIntegerType:
	return "integer";
    case nkFloatingType:
	return "floating point";
    case nkPhysicalType:
	return "physical";
    case nkArrayType:
	return "array";
    case nkRecordType:
	return "record";
    case nkCompositeType:
	return "composite";
    case nkAccessType:
	return "access";
    case nkType:
	return "any";
    default:
	return "unspeakable";
    }
}

nExpr psr::disambiguate_expr(nExpr e, nType t, bool procs, bool)
{
    if(e->is(nkAmbgCall)) {

	for(nNamedAssocElem ne = nAmbgCall(e)->first_actual; ne;
	    ne = nNamedAssocElem(ne->next)) {

	    assert(ne->is(nkNamedAssocElem));
	    if(ne->actual == NULL) {
		info("%:+++ - found NULL actual in %n", e, e);
		return NULL;
	    }
	}
	
	nAmbgCall(e)->set->invalidate_pot_invalids();
	nDeclaration d = nAmbgCall(e)->set->single_decl();
	if(d == NULL) {
	    // give more info about `e'
	    info("+++ - assocs:");
	    for(nGenAssocElem ge = nAmbgCall(e)->first_actual; ge;
		ge = ge->next) {
		info("+++ -  %n", ge);
		if(ge->is(nkNamedAssocElem)) {
		    nExpr e = nNamedAssocElem(ge)->actual;
		    nType_vector *types = ambg_expr_types(e);
		    for(int i = 0; i < types->size(); i++)
			if(try_overload_resolution(e, (*types)[i], nkInvalid))
			    info("+++ --  %n", (*types)[i]);
		}
	    }
	}
	if(d && d->is(nkFunction)) {
	    nAmbgCall ac = nAmbgCall(e);
	    if(is_array_func(nFunction(d), ac->first_actual)) {
		e = mFunctionCall(ac->lineno, nFunction(d), NULL);
		return build_ArrayObjectRef(e, ac->first_actual);
	    } else {
		return mFunctionCall(e->lineno,
				     nFunction(d),
				     associate(nAmbgCall(e)->first_actual,
					       nFunction(d)->first_formal));
	    }
	} else if(d && d->is(nkProcedure)) {
	    if(!procs) {
		error("%:%n is a procedure, not a function", e, d);
		return NULL;
	    } else
		return mProcedureCall(e->lineno, nProcedure(d),
				      associate(nAmbgCall(e)->first_actual,
						nProcedure(d)->first_formal));
	}

    } else if(e->is(nkAmbgEnumLitRef)) {
	nAmbgEnumLitRef(e)->set->invalidate_pot_invalids();
	nDeclaration d = nAmbgEnumLitRef(e)->set->single_decl();
	if(d && d->is(nkEnumLiteral))
	    return mEnumLitRef(e->lineno, nEnumLiteral(d));
    } else if(e->is(nkAmbgArrayLitRef)) {
	if(t == NULL)
	    error("%:can't determine string literal type", e);
	else
	    return mArrayLiteralRef(e->lineno, nAmbgArrayLitRef(e)->value, t);
    } else if(e->is(nkAmbgAggregate)) {
	if(t == NULL)
	    error("%:can't determine aggregate type", e);
	else
	    return build_Aggregate(nAmbgAggregate(e), t);
    } else if(e->is(nkAmbgNullExpr)) {
	if(t == NULL)
	    error("%:can't determine null constant type", e);
	else
	    return mNullExpr(e->lineno, t);
    } else if(e->is(nkNewExpr)) {
	if(t == NULL)
	    error("%:can't determine type of new object", e);
	else {
	    nNewExpr(e)->type = t;
	    return e;
	}
    } else
	return e;

    return NULL;
}

void psr::overload_resolution(nExpr &e, nType t, nKind k, bool procs)
{
    if(e == NULL)
	return;

    if(e->is(nkUnresolvedName)) {
	nUnresolvedName un = nUnresolvedName(e);
	nDeclaration d = find_single_decl(un->name, nkDeclaration, "");
	if(d)
	    error("%:%n can not be used in an expression", un->name, d);
	e = NULL;
	return;
    }

    bool valid = try_overload_resolution(e, t, k);

    if(!valid) {
	nType_vector *types = ambg_expr_types(e);
	if(t)
	    error("%:%n does not match required type %n, its type could be:",
		  e, e, t);
	else
	    error("%:type of %n is not a %s type, its type could be:", e, e,
		  type_string(k));
	for(int i = 0; i < types->size(); i++) {
	    nType t = (*types)[i];
	    if(try_overload_resolution(e, t, nkInvalid))
		info("%:   %n", (*types)[i], (*types)[i]);
	}
	delete types;
	e = NULL;
    } else
	e = disambiguate_expr(e, t, procs, false);

#if 0
    bool write = (m != Mode_In);
    bool read = (m == Mode_In || m == Mode_InOut || m == Mode_Linkage);
    valid = true;

    if(e && e->is(nkObjectRef)) {
	if(write && !nObjectRef(e)->is_writeable()) {
	    error("%:%n can not be updated", e, e);
	    valid = false;
	}
	if(read && !nObjectRef(e)->is_readable()) {
	    error("%:%n can not be read", e, e);
	    valid = false;
	}
	if(nObjectRef(e)->get_mode() == Mode_Linkage && m != Mode_Linkage) {
	    error("%:%n can only be passed to formals with mode 'linkage'",
		    e, e);
	    valid = false;
	}
    } else if(e && write) {
	error("%:%n is not an object", e, e);
	valid = false;
    }

    if(!valid)
	e = NULL;
#endif
}

ObjectClass sObjectRef::get_class()
{
    if(this->is(nkRecordObjectRef)) {
	nRecordObjectRef ror = nRecordObjectRef(this);
	if(ror->record && ror->record->is(nkObjectRef))
	    return nObjectRef(ror->record)->get_class();
	else
	    return ObjClass_None;
    } else if(this->is(nkGenericArrayObjectRef)) {
	nGenericArrayObjectRef aor = nGenericArrayObjectRef(this);
	if(aor->array && aor->array->is(nkObjectRef))
	    return nObjectRef(aor->array)->get_class();
	else
	    return ObjClass_None;
    } else if(this->is(nkAttrSignalRef))
	return ObjClass_Signal;
    else if(this->is(nkSimpleObjectRef)) {
	nSimpleObjectRef so = nSimpleObjectRef(this);
	if(so->object == NULL)
	    return ObjClass_None;
	if(so->object->is(nkInterface))
	    return nInterface(so->object)->object_class;
	return so->object->is(nkVariable)? ObjClass_Variable :
	       so->object->is(nkConstant)? ObjClass_Constant :
	       so->object->is(nkSignal)?   ObjClass_Signal :
	    // so->object->is(nkFile)?	   ObjClass_File :
					   ObjClass_None;
    } else if(this->is(nkAccessObjectRef)) {
	return ObjClass_Variable;
    } else
	assert(false);
}

Mode sObjectRef::get_mode()
{
    if(this->is(nkRecordObjectRef)) {
	nRecordObjectRef ror = nRecordObjectRef(this);
	if(ror->record && ror->record->is(nkObjectRef))
	    return nObjectRef(ror->record)->get_mode();
	else
	    return Mode_In;
    } else if(this->is(nkGenericArrayObjectRef)) {
	nGenericArrayObjectRef aor = nGenericArrayObjectRef(this);
	if(aor->array && aor->array->is(nkObjectRef))
	    return nObjectRef(aor->array)->get_mode();
	else
	    return Mode_In;
    } else if(this->is(nkAttrSignalRef))
	return Mode_In;
    else if(this->is(nkSimpleObjectRef)) {
	nSimpleObjectRef so = nSimpleObjectRef(this);
	if(so->object == NULL)
	    return Mode_None;
	if(so->object->is(nkInterface))
	    return nInterface(so->object)->mode;
	return so->object->is(nkConstant)? Mode_In :
	    // so->object->is(nkFile)?	   Mode_??? :
					   Mode_InOut;
    } else if(this->is(nkAccessObjectRef)) {
	return Mode_InOut; // XXX - guessed
    } else
	assert(false);
}

nType sObjectRef::get_type()
{
    if(this->is(nkRecordObjectRef)) {
	if(nRecordObjectRef(this)->element)
	    return nRecordObjectRef(this)->element->type;
    } else if(this->is(nkArrayObjectRef)) {
	if(nArrayObjectRef(this)->array_type)
	    return nArrayObjectRef(this)->array_type->element_type;
    } else if(this->is(nkSliceObjectRef)) {
	return nSliceObjectRef(this)->array_type;
    } else if(this->is(nkAttrSignalRef)) {
	return nAttrSignalRef(this)->attr_type;
    } else if(this->is(nkSimpleObjectRef)) {
	if(nSimpleObjectRef(this)->object)
	    return nSimpleObjectRef(this)->object->type;
    } else if(this->is(nkAccessObjectRef)) {
	return nAccessObjectRef(this)->accessed_type;
    } else
	assert(false);
    return NULL;
}
