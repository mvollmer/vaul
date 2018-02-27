/* types and subtypes

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

nType psr::build_SubType(nName res, nName mark, nConstraint cons)
{
    nType base = get_type(mark);
    if(base == NULL)
	return NULL;

    if(res) {	// XXX - filter for resolution function
	info("xxx - no resolution functions");
	vaul_decl_set ds(this);
	find_decls(ds, res);
	ds.single_decl();
    }

    if(cons) {
	if(cons->is(nkExplicitRange)) {
	    nExplicitRange r = nExplicitRange(cons);
	    overload_resolution(r->first, base);
	    overload_resolution(r->last, base);
	} else if(cons->is(nkArrayRange)) {
	    nType t = nArrayRange(cons)->type;
	    if(t->get_base() != base)
		error("%:%n is not a base type of %n", cons, t, base);
	} else if(cons->is(nkPreIndexConstraint))
	    cons = build_IndexConstraint(nPreIndexConstraint(cons), base);
	else
	    assert(false);
    }

    if(cons == NULL && res == NULL
       && (!base->is(nkSubType) || nSubType(base)->resol_func == NULL))
	return base;
    return mSubType(0, NULL, base, cons, res);
}

nIndexConstraint psr::build_IndexConstraint(nPreIndexConstraint pre,
					    nType base)
{
    if(!base->is(nkArrayType)) {
	if(base->get_base()->is(nkArrayType))
	    error("%:a constrained array can't be further constrained", pre);
	else
	    error("%:only array types can have index constraints", pre);
	return NULL;
    }

    nIndexType index = nArrayType(base)->first_index;
    nPreIndexConstraint pcons = pre;
    nIndexConstraint cons = NULL, *ctail = &cons;

    while(pcons && index) {
	*ctail = mIndexConstraint(pcons->lineno, *ctail, NULL);
	if(pcons->is(nkPreIndexRangeConstraint)) {
	    if(nRange r = nPreIndexRangeConstraint(pre)->range) {
		if(r->is(nkExplicitRange)) {
		    nExplicitRange er = nExplicitRange(r);
		    if(nType itype = find_index_range_type(er)) {
			overload_resolution(er->first, itype);
			overload_resolution(er->last, itype);
		    }
		}
		(*ctail)->type = mSubType(pcons->lineno, NULL, 
					  index->index_type, r, NULL);
	    }
	} else if(pcons->is(nkPreIndexSubtypeConstraint))
	    (*ctail)->type = nPreIndexSubtypeConstraint(pcons)->type;
	else
	    fatal_abort("build_IndexConstraint confused.\n");

	if((*ctail)->type && index->index_type 
	   && (*ctail)->type->get_base() != index->index_type->get_base())
	    error("%:constraint type (%n) does not match index type (%n)", pre,
		  (*ctail)->type->get_base(), index->index_type->get_base());

	ctail = &(*ctail)->next;
	index = index->next;
	pcons = pcons->next;
    }

    if(pcons)
	error("%:too many index constraints for %n", pre, base);
    else if(index)
	error("%:too few index constraints for %n", pre, base);
    return cons;
}

nType psr::find_index_range_type(nExplicitRange r)
{
    nType_vector &first_types = *ambg_expr_types(r->first);
    nType_vector &last_types = *ambg_expr_types(r->last);

    if(first_types.size() == 0 || last_types.size() == 0)
	return NULL;

    nType_vector types;

    for(int i = 0; i < first_types.size(); i++) {
	assert(first_types[i]);
	nType t = first_types[i];
	if(!is_discrete_type(t->get_base()))
	    continue;
	for(int j = 0; j < last_types.size(); j++) {
	    assert(last_types[j]);
	    nType tt = last_types[j];
	    if(!is_discrete_type(tt->get_base()))
		continue;
	    if(t == std->universal_integer)
		t = tt;
	    else if(tt == std->universal_integer)
		tt = t;
	    if(t->get_base() == tt->get_base()) {
		if(t == std->universal_integer)
		    t = std->predef_INTEGER;
		if(try_overload_resolution(r->first, t, nkInvalid)
		   && try_overload_resolution(r->last, t, nkInvalid)) {
		    bool already_inserted = false;
		    for(int k = 0; k < types.size(); k++)
			if(types[k]->get_base() == t->get_base()) {
			    // info("+++ - found %n twice", t);
			    already_inserted = true;
			    break;
			}
		    if(!already_inserted)
			types.add(t);
		}
	    }
	}
    }

    if(types.size() == 0) {
	error("%:index bounds must be discrete and of the same type", r);
	if(first_types.size() > 0) {
	    info("left bound could be:"); 
	    for(int i = 0; i < first_types.size(); i++)
		info("%:   %n", first_types[i], first_types[i]);
	} else
	    info("no left types");
	if(last_types.size() > 0) {
	    info("right bound could be:"); 
	    for(int i = 0; i < last_types.size(); i++)
		info("%:   %n", last_types[i], last_types[i]);
	} else
	    info("no right types");
    } else if(types.size() != 1) {
	error("%:type of index bounds is ambigous, it could be:", r);
	for(int i = 0; i < types.size(); i++)
	    info("%:   %n (%s)", types[i], types[i], types[i]->type_id());
    }

    delete &first_types;
    delete &last_types;

    return types.size() == 1? types[0] : NULL;
}


nSubType psr::build_SubType_def(int lineno, nRange r, nType base)
{
    if(base == NULL) {
	if(r->is(nkExplicitRange)) {
	    nExplicitRange er = nExplicitRange(r);
	    if(try_overload_resolution(er->first, NULL, nkIntegerType)
	       && try_overload_resolution(er->last, NULL, nkIntegerType))
		base = mIntegerType(lineno, NULL);
	    else if(try_overload_resolution(er->first, NULL, nkFloatingType)
		    && try_overload_resolution(er->last, NULL, nkFloatingType))
		base = mFloatingType(lineno, NULL);
	    else {
		error("%!range bounds must be both either integer"
		      " or real values", lex, lineno);
		return NULL;
	    }
	} else if(r->is(nkArrayRange)) {
	    info("XXX - no array ranges in type definition");
	    return NULL;
	} else
	    assert(false);
    }

    nKind base_k = base->kind();
    if(base_k == nkPhysicalType)
	base_k = nkIntegerType;
    assert(r->is(nkExplicitRange)); // for now...
    overload_resolution(nExplicitRange(r)->first, base_k);
    overload_resolution(nExplicitRange(r)->last, base_k);

    base = nType(add_decl(base));
    return mSubType(0, NULL, base, r, NULL);
}

nType psr::get_type(nName mark)
{
    nDeclaration d = find_single_decl(mark, nkType, "type");
    if(d) {
	if(d->is(nkIncompleteType))
	    error("%:type %n is incomplete", mark, mark);
	else
	    return nType(d);
    }
    return NULL;
}

void psr::add_PredefOp(int lineno, nType ret, Id sym, nType left, nType right)
{
    nInterface interf = 
	mInterface(lineno, NULL, left, NULL, NULL, ObjClass_Constant,
		   Mode_In, false);
    if(right)
	interf->next_element =
	    mInterface(lineno, NULL, right, NULL, NULL, ObjClass_Constant,
		       Mode_In, false);
    
    add_decl(mPredefOp(lineno, sym, interf, true, ret));
}

nType psr::is_one_dim_array(nType t)
{
    if(!t->is(nkArrayType))
	return NULL;
    nArrayType at = nArrayType(t);
    if(!at->first_index || at->first_index->next)
	return NULL;
    return at->element_type;
}

bool psr::is_one_dim_logical_array(nType t)
{
    nType et = is_one_dim_array(t);
    return et && et == std->predef_BIT || et == std->predef_BOOLEAN;
}

bool psr::is_one_dim_discrete_array(nType t)
{
    nType et = is_one_dim_array(t);
    return is_discrete_type(et);
}

bool psr::is_discrete_type(nType t)
{
    if(t == NULL)
	return false;
    t = t->get_base();
    return t && (t->is(nkIntegerType) || t->is(nkEnumType));
}

void psr::add_predefined_ops(nType t)
{
    if(t == NULL)
	return;

    nType bt = t;
    if(t->is(nkSubType))
	bt = nSubType(t)->base;
    if(bt->is(nkIncompleteType) || bt->is(nkSubType))
	return;


#   define add(r, op, t1, t2)  add_PredefOp(t->lineno, r, #op, t1, t2)

    // logical operators
    //
    if((t == std->predef_BIT || t == std->predef_BOOLEAN) 
       || is_one_dim_logical_array(t)) {
	add(t, "and", t, t);
	add(t, "or", t, t);
	add(t, "nand", t, t);
	add(t, "nor", t, t);
	add(t, "xor", t, t);
	add(t, "xnor", t, t);
	add(t, "not", t, NULL);
	if(bt == std->predef_BOOLEAN) {
	    add_predefined_ops(nType(
		add_decl(mUniversalInteger(t->lineno, "universal-integer"))
	    ));
	    add_predefined_ops(nType(
		add_decl(mUniversalReal(t->lineno, "universal-real"))
	    ));
	}
    }

    // relational operators
    //
    nType b = std->predef_BOOLEAN;
    if(b == NULL) {
	info("%:can't predefine relational operators for %n", t, t);
	info("%:since type BOOLEAN is undefined", t);
    } else {
	add(b, "=", t, t);
	add(b, "/=", t, t);
	if(bt->is(nkScalarType) || is_one_dim_discrete_array(bt)) {
	    add(b, "<", t, t);
	    add(b, ">", t, t);
	    add(b, "<=", t, t);
	    add(b, ">=", t, t);
	}
    }

    // shift operators
    //
    if(is_one_dim_logical_array(bt)) {
	if(nType r = std->predef_INTEGER) {
	    add(t, "sll", t, r);
	    add(t, "srl", t, r);
	    add(t, "sla", t, r);
	    add(t, "sra", t, r);
	    add(t, "rol", t, r);
	    add(t, "ror", t, r);
	} else {
	    info("%:can't predefine shift operators for %n", t, t);
	    info("%:since type INTEGER is undefined", t);
	}
	
    }

    // Adding, Sign and Miscellanous operators
    //
    if(bt->is(nkNumericType)) {
	add(t, "+", t, t);
	add(t, "-", t, t);
	add(t, "abs", t, NULL);
	add(t, "+", t, NULL);
	add(t, "-", t, NULL);
    }

    // concatenation operator
    //
    if(nType et = is_one_dim_array(bt)) {
	add(t, "&", t, t);
	add(t, "&", t, et);
	add(t, "&", et, t);
	add(t, "&", et, et);
    }

    // Multiplying operators
    //
    if(bt->is(nkIntegerType) || bt->is(nkFloatingType)) {
	add(t, "*", t, t);
	add(t, "/", t, t);
	if(bt->is(nkIntegerType)) {
	    add(t, "mod", t, t);
	    add(t, "rem", t, t);
	}
	if(std->predef_INTEGER)
	    add(t, "**", t, std->predef_INTEGER);
	else if(bt != std->universal_integer && bt != std->universal_real) {
	    info("%:can't predefine \"**\" operator for %n", t, t);
	    info("%:since type INTEGER is undefined", t);
	}
    }

    if(bt->is(nkPhysicalType)) {
	if(std->predef_INTEGER && std->predef_REAL) {
	    add(t, "*", t, std->predef_INTEGER);
	    add(t, "*", std->predef_INTEGER, t);
	    add(t, "/", t, std->predef_INTEGER);
	    add(t, "*", t, std->predef_REAL);
	    add(t, "*", std->predef_REAL, t);
	    add(t, "/", t, std->predef_REAL);
	} else {
	    info("%:can't predefine multiplying operators for %n", t, t);
	    info("%:since types INTEGER and REAL are undefined", t);
	}
	if(std->universal_integer)
	    add(std->universal_integer, "/", t, t);
    }

    if(t == std->predef_INTEGER) {
	add(std->universal_integer, "**", std->universal_integer, t);
	add(std->universal_real, "**", std->universal_real, t);
    }

    if(t->is(nkAccessType)) {
	nInterface parm = mInterface(t->lineno, NULL, t, NULL, NULL,
				     ObjClass_Variable, Mode_InOut, false);
	nProcedure dealloc = mProcedure(t->lineno, "deallocate", parm);
	add_decl(dealloc);
    }
    
    if(t->is(nkFileType)) {
	nInterface parm = mInterface(t->lineno, NULL, t, NULL, NULL,
				     ObjClass_File, Mode_None, false);
	nFunction endfile = mFunction(t->lineno, "endfile", parm, false,
				       std->predef_BOOLEAN);
	add_decl(endfile);
    }
#   undef add
}

void sIncompleteType::add_use(nType &ref)
{
    assert(ref == this);
    use *u = new use;
    u->next = uses;
    uses = u;
    u->ref = &ref;
}

void sIncompleteType::complete(nType t)
{
    for(use *u = uses; u; u = u->next) {
	assert(*u->ref == this);
	*u->ref = t;
    }
}

nPreIndexConstraint psr::build_PreIndexConstraint(nGenAssocElem a)
{
    nPreIndexConstraint ic = NULL, *ict = &ic;

    while(a) {
	if(a->is(nkNamedAssocElem)) {
	    nNamedAssocElem nae = nNamedAssocElem(a);
	    if(nae->formal)
		error("%:index constraints can't use named association", nae);
	    if(nae->actual && nae->actual->is(nkUnresolvedName)) {
		nName n = nUnresolvedName(nae->actual)->name;
		nType st = nType(find_single_decl(n, nkType, "type"));
		if(is_discrete_type(st))
		    *ict = mPreIndexSubtypeConstraint(a->lineno, NULL, st);
		else if(st)
		    error("%: %s is not a discrete type", n, st);
	    }
	} else if(a->is(nkRangeAssocElem)) {
	    *ict = mPreIndexRangeConstraint(a->lineno, NULL,
					    nRangeAssocElem(a)->range);
	} else if(a->is(nkSubtypeAssocElem)) {
	    *ict = mPreIndexSubtypeConstraint(a->lineno,
					      NULL, nSubtypeAssocElem(a)->type);
	}

	a = a->next;
	if(*ict)
	    ict = &(*ict)->next;
    }

    return ic;
}

nType psr::adapt_object_type(ObjectClass c, nType t, nExpr init)
{
    if(t == NULL)
	return NULL;

    if(c == ObjClass_Variable || c == ObjClass_Signal) {
	if(t->is(nkArrayType))
	    error("array objects must have a constrained type");
	return t;
    }

    if(c == ObjClass_Constant) {
	if(init == NULL || !t->is(nkArrayType))
	    return t;

	info("xxx - dreaming up index bounds of array constant");
	nArrayType at = nArrayType(t);
	nIndexConstraint cons = NULL, *ctail = &cons;
	for(nIndexType it = at->first_index; it; it = it->next) {
	    int l = init->lineno;
	    nExplicitRange er = mExplicitRange(l, DirectionUp,
					       mLiteralRef(l, "0"),
					       mLiteralRef(l, "10"));
	    nSubType st = mSubType(l, NULL, it->index_type, er, NULL);
	    *ctail = mIndexConstraint(l, NULL, st);
	    ctail = &(*ctail)->next;
	}
	return mSubType(init->lineno, NULL, t, cons, NULL);
    }

    info("xxx - unchecked object type");
    return t;
}

nSubType psr::build_constrained_array_type(nPreIndexConstraint pre, nType elt)
{
    nIndexType itypes = NULL, *itail = &itypes;
    for(nPreIndexConstraint p = pre; p; p = p->next) {
	nType it;
	if(p->is(nkPreIndexSubtypeConstraint))
	    it = nPreIndexSubtypeConstraint(p)->type;
	else if(p->is(nkPreIndexRangeConstraint)) {
	    nRange r = nPreIndexRangeConstraint(p)->range;
	    if(r->is(nkExplicitRange))
		it = find_index_range_type(nExplicitRange(r));
	    else if(r->is(nkArrayRange))
		it = nArrayRange(r)->type;
	    else
		assert(false);
	} else
	    assert(false);
	*itail = mIndexType(p->lineno, NULL, it);
	itail = &(*itail)->next;
    }

    nArrayType base = mArrayType(pre? pre->lineno:-1, NULL, itypes, elt);
    return mSubType(base->lineno, NULL, base, build_IndexConstraint(pre, base),
		    NULL);
}

nFile psr::add_File(int lno, Id id, nType file_type, nExpr mode, nExpr name)
{
    if(!file_type->is(nkFileType)) {
	error("%!%n is not a file type", lex, lno, file_type);
	return NULL;
    }
    return nFile(add_decl(mFile(lno, id, file_type, mode, name)));
}

