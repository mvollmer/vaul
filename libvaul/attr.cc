/* attributes

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

enum attr_id {

    // type prefix
    ai_BASE, ai_IMAGE, ai_VALUE, ai_POS, ai_VAL, ai_SUCC, ai_PRED,
    ai_LEFTOF, ai_RIGHTOF,

    // type or array prefix
    ai_LEFT, ai_RIGHT, ai_HIGH, ai_LOW, ai_ASCENDING,

    // array prefix
    ai_RANGE, ai_REVERSE_RANGE, ai_LENGTH,

    // signal prefix
    ai_DELAYED, ai_STABLE, ai_QUIET, ai_TRANSACTION, ai_EVENT,
    ai_ACTIVE, ai_LAST_EVENT, ai_LAST_ACTIVE, ai_LAST_VALUE,
    ai_DRIVING, ai_DRIVING_VALUE,

    // named entity prefix
    ai_SIMPLE_NAME, ai_INSTANCE_NAME, ai_PATH_NAME,

    ai_MAX
};

#define AI_FIRST_TYPE   ai_BASE
#define AI_LAST_TYPE    ai_RIGHTOF
#define AI_FIRST_T_OR_A ai_LEFT
#define AI_LAST_T_OR_A  ai_ASCENDING
#define AI_FIRST_ARRAY  ai_RANGE
#define AI_LAST_ARRAY   ai_LENGTH
#define AI_FIRST_TA     AI_FIRST_TYPE
#define AI_LAST_TA      AI_LAST_ARRAY
#define AI_FIRST_SIGNAL ai_DELAYED
#define AI_LAST_SIGNAL  ai_DRIVING_VALUE
#define AI_FIRST_ENTITY ai_SIMPLE_NAME
#define AI_LAST_ENTITY  ai_PATH_NAME

static const char *attr_name[] = {
    "BASE", "IMAGE", "VALUE", "POS", "VAL", "SUCC", "PRED",
    "LEFTOF", "RIGHTOF",

    "LEFT", "RIGHT", "HIGH", "LOW", "ASCENDING",

    "RANGE", "REVERSE_RANGE", "LENGTH",

    "DELAYED", "STABLE", "QUIET", "TRANSACTION", "EVENT",
    "ACTIVE", "LAST_EVENT", "LAST_ACTIVE", "LAST_VALUE",
    "DRIVING", "DRIVING_VALUE",

    "SIMPLE_NAME", "INSTANCE_NAME", "PATH_NAME"
};


nNode psr::build_AttrNode(nName n, vaul_decl_set *set, nKind basic_k)
{
    if(n->is(nkAttributeName))
	return build_AttrNode(nAttributeName(n), set, basic_k);

    if(n == set->name && sNode::is_base_of(nkDeclaration, basic_k)) {
	nDeclaration d = set->single_decl();
	delete set;
	return d;
    }

    return build_Expr(n, set, basic_k);
}

nNode psr::build_AttrNode(nAttributeName n, vaul_decl_set *set, nKind basic_k)
{
    nExpr p = NULL;
    bool too_many = false;
    if(n->first_actual) {
	if(n->first_actual->next)	    
	    too_many = true;
	if(n->first_actual->formal)
	    error("%:attribute arguments do not have names", n);
	p = n->first_actual->actual;
    }

    attr_id ai = ai_MAX;
    for(int i = 0; i < ai_MAX; i++)
	if(vaul_name_eq(n->attribute, attr_name[i])) {
	    ai = attr_id(i);
	    break;
	}

    nNode res;

    if(ai >= AI_FIRST_TA && ai <= AI_LAST_TA) {
	nNode b = build_AttrNode(n->prefix, set, basic_k);
	if(b == NULL)
	    return NULL;

	nType t = NULL, base_t = NULL;
	if(b->is(nkObject)) {
	    t = nObject(b)->type;
	    if(t == NULL)
		return NULL;
	} else if(b->is(nkType))
	    t = nType(b);

	if(t)
	    base_t = t->get_base();

	if(ai >= AI_FIRST_TYPE && ai <= AI_LAST_TYPE) {
	    if(t == NULL) {
		error("%:prefix of '%s must be a type (or object)",
		      n->prefix, attr_name[ai]);
		return NULL;
	    }

	    if(ai >= ai_IMAGE && ai <= ai_VALUE) {
		if(!base_t->is(nkScalarType)) {
		    error("%:prefix of '%s must be a scalar type", n->prefix,
			  attr_name[ai]);
		    return NULL;
		}
	    } else if(ai >= ai_POS && ai <= ai_RIGHTOF) {
		if(!is_discrete_type(base_t) && !base_t->is(nkPhysicalType)) {
		    error("%:prefix of '%s must be a discrete or"
			  " physical type", n->prefix, attr_name[ai]);
		    return NULL;
		}
	    }

	    switch(ai) {
	    case ai_BASE: {
		res = base_t;
	    } break;

	    case ai_IMAGE: {
		overload_resolution(p, t);
		res = mAttr_IMAGE(n->lineno, std->predef_STRING, t, p);
		p = NULL;
	    } break;

	    case ai_VALUE: {
		overload_resolution(p, std->predef_STRING);
		res = mAttr_VALUE(n->lineno, t, t, p);
		p = NULL;
	    } break;

	    case ai_POS: {
		overload_resolution(p, t);
		res = mAttr_POS(n->lineno, std->universal_integer, t, p);
		p = NULL;
	    } break;

	    case ai_VAL: {
		overload_resolution(p, nkIntegerType);
		res = mAttr_VAL(n->lineno, t, t, p);
		p = NULL;
	    } break;

	    case ai_SUCC: {
		overload_resolution(p, t);
		res = mAttr_SUCC(n->lineno, t, t, p);
		p = NULL;
	    } break;

	    case ai_PRED: {
		overload_resolution(p, t);
		res = mAttr_PRED(n->lineno, t, t, p);
		p = NULL;
	    } break;

	    case ai_LEFTOF: {
		overload_resolution(p, t);
		res = mAttr_LEFTOF(n->lineno, t, t, p);
		p = NULL;
	    } break;

	    case ai_RIGHTOF: {
		overload_resolution(p, t);
		res = mAttr_RIGHTOF(n->lineno, t, t, p);
		p = NULL;
	    } break;

	    default: // should be complete.
		info("%:XXX - unimplemented type attribute %s",
		     n, n->attribute);
		return NULL;
	    }

	} else if(ai >= AI_FIRST_T_OR_A && ai <= AI_LAST_T_OR_A) {
	    if(t && base_t->is(nkArrayType) && t != b) {
		assert(b->is(nkObject));
		nObject o = nObject(b);
		int index;
		nType it = find_array_attr_index_type(nArrayType(base_t),
						      p, index);
		p = NULL;
		if(it == NULL)
		    return NULL;
		switch(ai) {
		case ai_LEFT:
		    res = mArrayAttr_LEFT(n->lineno, it, o, index);
		    break;
		case ai_RIGHT:
		    res = mArrayAttr_RIGHT(n->lineno, it, o, index);
		    break;
		case ai_HIGH:
		    res = mArrayAttr_HIGH(n->lineno, it, o, index);
		    break;
		case ai_LOW:
		    res = mArrayAttr_LOW(n->lineno, it, o, index);
		    break;
		case ai_ASCENDING:
		    res = mArrayAttr_ASCENDING(n->lineno,
					       std->predef_BOOLEAN, o, index);
		    break;
		default:
		    assert(false);
		}
	    } else if(t) {
		if(!base_t->is(nkScalarType)) {
		    error("%:prefix of '%s must be a scalar type",
			  n->prefix, attr_name[ai]);
		    return NULL;
		}
		switch(ai) {
		case ai_LEFT:
		    res = mAttr_LEFT(n->lineno, t, t, NULL);
		    break;
		case ai_RIGHT:
		    res = mAttr_RIGHT(n->lineno, t, t, NULL);
		    break;
		case ai_HIGH:
		    res = mAttr_HIGH(n->lineno, t, t, NULL);
		    break;
		case ai_LOW:
		    res = mAttr_LOW(n->lineno, t, t, NULL);
		    break;
		case ai_ASCENDING:
		    res = mAttr_ASCENDING(n->lineno, std->predef_BOOLEAN,
					  t, NULL);
		    break;
		default:
		    assert(false);
		}
	    } else {
		error("%:prefix of '%s must be a type or array",
		      n->prefix, attr_name[ai]);
		return NULL;
	    }

	} else {
	    assert(ai >= AI_FIRST_ARRAY && ai <= AI_LAST_ARRAY);

	    if(t == NULL || !base_t->is(nkArrayType) || t == b) {
		error("%:prefix of '%s must be an array",
		      n->prefix, attr_name[ai]);
		return NULL;
	    }
	    assert(b->is(nkObject));
	    nObject o = nObject(b);
	    int index;
	    nType it = find_array_attr_index_type(nArrayType(base_t),
						  p, index);
	    if(it == NULL)
		return NULL;
	    p = NULL;
	    switch(ai) {
	    case ai_RANGE:
		res = mArrayAttr_RANGE(n->lineno, it, o, index);
		break;
	    case ai_REVERSE_RANGE:
		res = mArrayAttr_REVERSE_RANGE(n->lineno, it, o, index);
		break;
	    case ai_LENGTH:
		res = mArrayAttr_LENGTH(n->lineno, std->universal_integer, o,
					index);
		break;
	    default:
		assert(false);
	    }
	}

    } else if(ai >= AI_FIRST_SIGNAL && ai <= AI_LAST_SIGNAL) {

	nNode b = build_Expr(n->prefix, set, basic_k);
	if(b == NULL)
	    return NULL;
	if(!b->is(nkObjectRef) || !nObjectRef(b)->is_signal()) {
	    error("%:prefix of '%s must be a signal",
		  n->prefix, attr_name[ai]);
	    return NULL;
	}
	nObjectRef or = nObjectRef(b);

	switch(ai) {
	case ai_DELAYED:
	    overload_resolution(p, std->predef_TIME);
	    res = mAttr_DELAYED(n->lineno, or->get_type(), or, p);
	    p = NULL;
	    break;

	case ai_STABLE:
	    overload_resolution(p, std->predef_TIME);
	    res = mAttr_STABLE(n->lineno, std->predef_BOOLEAN, or, p);
	    p = NULL;
	    break;

	case ai_QUIET:
	    overload_resolution(p, std->predef_TIME);
	    res = mAttr_QUIET(n->lineno, std->predef_BOOLEAN, or, p);
	    p = NULL;
	    break;

	case ai_TRANSACTION:
	    res = mAttr_TRANSACTION(n->lineno, std->predef_BIT, or);
	    break;

	case ai_EVENT:
	    res = mAttr_EVENT(n->lineno, std->predef_BOOLEAN, or);
	    break;

	case ai_ACTIVE:
	    res = mAttr_ACTIVE(n->lineno, std->predef_BOOLEAN, or);
	    break;

	case ai_LAST_EVENT:
	    res = mAttr_LAST_EVENT(n->lineno, std->predef_TIME, or);
	    break;
	    
	case ai_LAST_ACTIVE:
	    res = mAttr_LAST_ACTIVE(n->lineno, std->predef_TIME, or);
	    break;
	    
	case ai_LAST_VALUE:
	    res = mAttr_LAST_VALUE(n->lineno, or->get_type(), or);
	    break;

	case ai_DRIVING:
	    res = mAttr_DRIVING(n->lineno, std->predef_BOOLEAN, or);
	    break;
	    
	case ai_DRIVING_VALUE:
	    res = mAttr_DRIVING_VALUE(n->lineno, or->get_type(), or);
	    break;

	default:
	    // should be complete
	    info("%:XXX - unimplemented signal attribute %s", n, n->attribute);

	}

    } else if(ai >= AI_FIRST_ENTITY && ai <= AI_LAST_ENTITY) {
	info("XXX - unimplemented entity attribute %s", n, n->attribute);
	delete set;
	res = NULL;
    } else {
	// user defined attributes
	info("%:XXX - no user defined attributes", n);
	delete set;
	res = NULL;
    }

    if(p || too_many)
	error("%:too many arguments for attribute %s", n, attr_name[ai]);

    return res;
}

bool psr::evaluate_locally_static_universal_integer(nExpr e, int &n)
{
    // XXX - should be done by the backend.

    overload_resolution(e, nkUniversalInteger);
    if(e == NULL)
	return false;

    if(e->is(nkLiteralRef)) {
	n = 0;
	for(char *cp = nLiteralRef(e)->value; *cp; cp++) {
	    if(*cp == '_')
		continue;
	    if(*cp >= '0' && *cp <= '9')
		n = n*10 + *cp-'0';
	    else {
		info("XXX - unsupported abstract literal syntax");
		return false;
	    }
	}
	return true;
    } else {
	info("XXX - %s is unsupported as universal integer expression",
	     e->type_id());
	return false;
    }
}

nType psr::find_array_attr_index_type(nArrayType t, nExpr p, int &index)
{
    if(p == NULL)
	index = 1;
    else if(!evaluate_locally_static_universal_integer(p, index))
	return NULL;
    nIndexType it = nArrayType(t)->first_index;
    int dims = 0;
    while(it && index != ++dims)
	it = it->next;
    if(it == NULL) {
	error("%:%n has only %d dimensions, not %d", p, t,
	      dims, index);
	return NULL;
    }
    return it->index_type;
}

void psr::bind_attrspec(nAttributeSpec s)
{
    if(s == NULL || s->entities == NULL)
	return;

    nAttribute a = nAttribute(find_single_decl(mSimpleName(s->lineno,
							   s->attr_desig),
					       nkAttribute, "attribute"));
    if(a == NULL)
	return;

    overload_resolution(s->value, a->type);
    if(s->value == NULL)
	return;

    info("%:xxx - would bind '%n of these %ss to %n:", s, a,
	 sNode::type_id(nKind(s->entities->entity_class)), s->value);
    nEntityNameList el = s->entities->names;
    if(el->is(nkEntityNameList_Ids)) {
	for(nIdList il = nEntityNameList_Ids(el)->ids; il; il = il->link)
	    info("%:xxx -  %s", il, il->id);
    } else if(el->is(nkEntityNameList_ALL)) {
	info("%:xxx -  ALL", el);
    } else if(el->is(nkEntityNameList_OTHERS)) {
	info("%:xxx -  OTHERS", el);
    } else
	assert(false);
}
