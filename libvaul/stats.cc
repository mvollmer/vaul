/* statements

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

nLoopStat psr::push_loop(int lineno, nLabel label, nIterationScheme scheme)
{
    nLoopScope sc = nLoopScope(add_decl(cur_scope,
					mLoopScope(lineno, NULL, NULL)));
    push_scope(sc);

    // XXX - merge this with build_IndexConstraint
    if(scheme->is(nkPreForScheme)) {
	nPreForScheme pfs = nPreForScheme(scheme);
	nRange range = NULL;
	nType type = NULL;
	if(pfs->range == NULL)
	    ;
	else if(pfs->range->is(nkPreIndexRangeConstraint)) {
	    if(range = nPreIndexRangeConstraint(pfs->range)->range) {
		if(range->is(nkExplicitRange)) {
		    nExplicitRange er = nExplicitRange(range);
		    if(type = find_index_range_type(er)) {
			overload_resolution(er->first, type);
			overload_resolution(er->last, type);
		    }
		} else if(range->is(nkArrayRange))
		    type = nArrayRange(range)->type;
		else
		    assert(false);
	    }
	} else if(pfs->range->is(nkPreIndexSubtypeConstraint)) {
	    type = nPreIndexSubtypeConstraint(pfs->range)->type;
	    if(!is_discrete_type(type))
		error("%n is not a discrete type", type);
	    else if(type) {
		if(!type->is(nkSubType) ||
		   !nSubType(type)->constraint->is(nkRange)) {
		    info("XXX - no `%s' in a for-scheme", type->type_id());
		} else
		    range = nRange(nSubType(type)->constraint);
	    }
	} else
	    fatal_abort("push_loop confused.\n");

	nConstant c = mConstant(pfs->lineno, pfs->var, type, NULL);
	c = nConstant(add_decl(cur_scope, c));

	scheme = mForScheme(pfs->lineno, c, range);
    }

    nLoopStat st = mLoopStat(lineno, sc, scheme, NULL);
    sc->loop = st;
    st->label = label;
    if(label)
	label->stat = st;
    return st;
}

nLoopStat psr::pop_loop(nStatement stats, Id id)
{
    if(cur_scope && !cur_scope->is(nkLoopScope)) {
	info("confusion, expect failure");
	while(cur_scope && !cur_scope->is(nkLoopScope))
	    pop_scope(cur_scope);
    }

    if(cur_scope == NULL)
	return NULL;

    assert(cur_scope && cur_scope->is(nkLoopScope));

    nLoopScope sc = nLoopScope(cur_scope);
    pop_scope(sc);

    if(sc->loop == NULL)
	return NULL;

    if(id) {
	nLabel l = sc->loop->label;
	if(l && !vaul_name_eq(l->id, id))
	    error("'%s' does not match loop label '%s'", id, l->id);
	else if(l == NULL)
	    error("loop has no label");
    }
    sc->loop->stats = stats;

    return sc->loop;
}

void psr::add_to_signal_list(nSignalList &sigs, nObjectRef sig)
{
    nSignalList *slp;
    for(slp = &sigs; *slp; slp = &(*slp)->next)
	if((*slp)->signal == sig)
	    return;
    *slp = mSignalList(sig, *slp);
}

void psr::get_implicit_signals(nSignalList &sigs, nExpr e)
{
    if(e == NULL)
	return;

    if(e->is(nkFunctionCall)) {
	for(nAssociation a = nFunctionCall(e)->first_actual; a; a = a->next)
	    get_implicit_signals(sigs, a->actual);
    } else if(e->is(nkTypeConversion))
	get_implicit_signals(sigs, nTypeConversion(e)->expression);
    else if(e->is(nkQualifiedExpr))
	get_implicit_signals(sigs, nQualifiedExpr(e)->expression);
    else if(e->is(nkObjectRef)) {
	nObjectRef or = nObjectRef(e);
	if(or->is_signal())
	    add_to_signal_list(sigs, or);
    } else if(e->is(nkAttrSigFunc))
	add_to_signal_list(sigs, nAttrSigFunc(e)->signal);
    else
	info("xxx - %s not scanned for implicit signals", e->type_id());
}

nProcess psr::build_condal_Process(Id l, nCondalSignalAssign csa)
{
    if(csa == NULL || csa->target == NULL || csa->wave == NULL)
	return NULL;

    nSignalList sens = NULL;

    nStatement stat = NULL, *statp = &stat;
    for(nCondalWaveform cw = csa->wave; cw; cw = cw->else_wave) {
	nSignalAssignment ass = build_SignalAssignment(cw->lineno,
						       csa->target,
						       csa->transport,
						       cw->wave);
	if(ass) {
	    for(nWaveformElement wfe = ass->first_wave; wfe; wfe = wfe->next)
		get_implicit_signals(sens, wfe->value);
	}
	if(cw->condition) {
	    get_implicit_signals(sens, cw->condition);
	    *statp = mIfStat(cw->lineno, cw->condition, ass, NULL);
	    statp = &nIfStat(*statp)->else_stats;
	} else {
	    *statp = ass;
	    assert(cw->else_wave == NULL);
	}
    }
    stat->next = mWaitStat(csa->lineno, sens, NULL, NULL);

    nProcess p = mProcess(csa->lineno, l, NULL, stat);
    add_decl(p);
    return p;
}

nProcess psr::build_conc_AssertStat(int lno, Id label, nAssertStat as)
{
    if(as == NULL)
	return NULL;

    nSignalList sens = NULL;
    get_implicit_signals(sens, as->condition);
    as->next = mWaitStat(lno, sens, NULL, NULL);

    nProcess p = mProcess(lno, label, NULL, as);
    add_decl(p);
    return p;
}

nProcess psr::build_conc_ProcedureCall(int lno, Id label,
				       nProcedureCallStat pcs)
{
    if(pcs == NULL)
	return NULL;

    nSignalList sens = NULL;
    for(nAssociation assoc = pcs->first_actual; assoc; assoc = assoc->next)
	get_implicit_signals(sens, assoc->actual);
    pcs->next = mWaitStat(lno, sens, NULL, NULL);

    nProcess p = mProcess(lno, label, NULL, pcs);
    add_decl(p);
    return p;
}

nLoopControlStat psr::build_LoopControlStat(int lineno, nKind k,
					    Id loop, nExpr when)
{
    nLoopControlStat lcs = NULL;

    nLoopScope sc = NULL;
    if(loop) {
	nLabel l = nLabel(find_single_decl(mSimpleName(lineno, loop), 
					   nkLabel, "label"));
	if(l && l->stat) {
	    if(l->stat->is(nkLoopStat))
		sc = nLoopStat(l->stat)->scope;
	    else
		error("%s is not a loop statement", loop);
	}
    }

    nScope s = cur_scope;
    while(s && (!s->is(nkLoopScope) || (sc && s != sc)))
	s = s->scope;

    if(s == NULL) {
	error(loop? "%s statement is not in loop labeled `%s'"
	          : "%s statement outside of loop", 
	      (k == nkNextStat)? "next" : "exit", loop);
    } else {
	assert(s->is(nkLoopScope));
	nLoopStat l = nLoopScope(s)->loop;
	lcs = (k == nkNextStat)? mNextStat(lineno, l, when)
	                       : mExitStat(lineno, l, when);
    }

    return lcs;
}

bool psr::check_target(nExpr t, ObjectClass oc, const char *oc_label)
{
    if(t == NULL)
	return true;

    if(t->is(nkRecordAggregate)) {
	nRecordAggregate ra = nRecordAggregate(t);
	for(nRecAggrAssoc a = ra->first_assoc; a; a = a->next) {
	    if(!check_target(a->actual, oc, oc_label))
		return false;
	}
	return true;
    } else if(t->is(nkArrayAggregate)) {
	nArrayAggregate aa = nArrayAggregate(t);
	for(nArrAggrAssoc a = aa->first_assoc; a; a = a->next) {
	    if(!check_target(a->actual, oc, oc_label))
		return false;
	}
	return true;
    }

    if(!t->is(nkObjectRef) || nObjectRef(t)->get_class() != oc
       || !nObjectRef(t)->is_writeable()) {
	error("%:%n is not a %s", t, t, oc_label);
	return false;
    }

    return true;
}
	

nVarAssignment psr::build_VarAssignment(int lineno, nExpr t, nExpr v)
{
    if(t == NULL || v == NULL)
	return NULL;

    if(t->is(nkAmbgAggregate)) {
	overload_resolution(v, nkCompositeType);
	if(v == NULL)
	    return NULL;
	nType vt = expr_type(v);
	overload_resolution(t, vt);
    } else if(t->is(nkObjectRef))
	overload_resolution(v, nObjectRef(t)->get_type());
    else
	return NULL;

    if(check_target(t, ObjClass_Variable, "variable"))
	return mVarAssignment(lineno, t, v);
    else
	return NULL;
}

nSignalAssignment psr::build_SignalAssignment(int lineno,
					      nExpr t,
					      bool transport,
					      nWaveformElement wf)
{
    if(t == NULL || wf == NULL)
	return NULL;

    if(t->is(nkAmbgAggregate)) {
	bool all_composite = true;
	for(nWaveformElement we = wf; we; we = we->next) {
	    overload_resolution(we->value, nkCompositeType);
	    if(we->value == NULL)
		all_composite = false;
	}
	if(!all_composite)
	    return NULL;

	nType comp_type = NULL;
	bool ct_valid = true;
	for(nWaveformElement _we = wf; _we; _we = _we->next) {
	    nType t = expr_type(_we->value);
	    if(t && comp_type != t && comp_type != NULL)
		ct_valid = false;
	    else
		comp_type = t;
	}
	if(!ct_valid) {
	    error("the types of all waveform elements must be the same");
	    info("they are, in order:");
	    for(nWaveformElement we = wf; we; we = we->next) {
		nType t = expr_type(we->value);
		info("%: %n", t, t);
	    }
	    return NULL;
	}

	overload_resolution(t, comp_type);

    } else if(t->is(nkObjectRef)) {
	nType type = nObjectRef(t)->get_type();
	for(nWaveformElement we = wf; we; we = we->next)
	    overload_resolution(we->value, type);
    } else {
	error("%:%n can not be a target", t, t);
	return NULL;
    }

    if(check_target(t, ObjClass_Signal, "signal"))
	return mSignalAssignment(lineno, t, transport, wf);
    else
	return NULL;
}

static bool is_character_type(nType t)
{
    if(t == NULL || (t = t->get_base()) == NULL)
	return false;

    if(!t->is(nkEnumType))
	return false;

    nEnumType et = nEnumType(t);
    for(nEnumLiteral el = nEnumType(t)->first; el; el = el->next)
	if(el->id && el->id[0] == '\'')
	    return true;
    return false;
}

static bool possible_switch_expr_type(nType t)
{
    if(t == NULL)
	return false;
    nType bt = t->get_base();

    if(bt->is(nkScalarType) || bt->is(nkEnumType))
	return true;
    if(!bt->is(nkArrayType))
	return false;

    nArrayType at = nArrayType(bt);
    if(!at->first_index || at->first_index->next)
	return false;
    return is_character_type(at->element_type);
}

nCaseStat psr::build_CaseStat(int lno, nExpr swex, nCaseAlternative alts)
{
    if(swex == NULL)
	return NULL;

    nType_vector *swex_types = ambg_expr_types(swex);
    assert(swex_types);

    if(swex_types->size() == 0)
	return NULL;

    bool valid_type = true;
    nType swex_type = NULL;
    for(int i = 0; i < swex_types->size(); i++)
	if(possible_switch_expr_type((*swex_types)[i])) {
	    if(swex_type)
		valid_type = false;
	    swex_type = (*swex_types)[i];
	}

    if(!valid_type) {
	error("%:type of case expression is ambigous, it could be:", swex);
	for(int i = 0; i < swex_types->size(); i++)
	    if(possible_switch_expr_type((*swex_types)[i]))
		info("%:    %n", (*swex_types)[i], (*swex_types)[i]);
	return NULL;
    }

    if(swex_type == NULL) {
	error("%:type of case expression is invalid, it could be:", swex);
	for(int i = 0; i < swex_types->size(); i++)
	    info("%:    %n", (*swex_types)[i], (*swex_types)[i]);
	return NULL;
    }

    delete swex_types;

    overload_resolution(swex, swex_type);
    for(nCaseAlternative a = alts; a; a = a->next) {
	for(nChoice c = a->first_choice; c; c = c->next) {
	    if(c->is(nkExprChoice))
		overload_resolution(nExprChoice(c)->expr, swex_type);
	    else if(c->is(nkRangeChoice)) {
		nGenAssocElem ae = nRangeChoice(c)->range;
		if(ae->is(nkRangeAssocElem)) {
		    nRange r = nRangeAssocElem(ae)->range;
		    if(r->is(nkExplicitRange)) {
			nExplicitRange er = nExplicitRange(r);
			overload_resolution(er->first, swex_type);
			overload_resolution(er->last, swex_type);
		    } else if(r->is(nkArrayRange)) {
			nType t = nArrayRange(r)->type;
			if(t->get_base() != swex_type) {
			    error("%:%n is not a base type of %n",
				  t, swex_type);
			}
		    } else
			assert(false);
		} else if(ae->is(nkSubtypeAssocElem)) {
		    if(nSubtypeAssocElem(ae)->type->get_base() != swex_type) {
			error("%:%n is not a subtype of %n",
			      nSubtypeAssocElem(ae)->type, swex_type);
			nSubtypeAssocElem(ae)->type = NULL;
		    }
		} else
		    info("XXX - no `%s' ranges", ae->type_id());
	    } else if(c->is(nkOthersChoice))
		;
	    else
		info("XXX - no `%s' choices", c->type_id());
	}
    }

    return mCaseStat(lno, swex, alts);
}
