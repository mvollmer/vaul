/* blocks, components and bindings

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

nBindingIndic psr::build_BindingIndic(nIncrementalBindingIndic ibi)
{
    return build_BindingIndic(ibi->unit, ibi->generic_assoc, ibi->port_assoc);
}

nBindingIndic psr::build_BindingIndic(nComponent u, nNamedAssocElem gm,
				      nNamedAssocElem pm)
{
    if(u == NULL)
	return NULL;

    nAssociation g = associate(gm, u->first_generic, false);
    nAssociation p = associate(pm, u->first_port, false);
    return mBindingIndic(u, g, p);
}

void sBlock::add_spec(nConfigSpec cs)
{
    nConfigSpec *csp;
    for(csp = &specs; *csp; csp = &(*csp)->next)
	;
    *csp = cs;
}

void psr::bind_specs(nBlock b)
{
    for(nConfigSpec cs = b->specs; cs; cs = cs->next) {
	if(cs->comps == NULL)
	    continue;
	for(nCompInstList cil = list_comps(cs->comps, b); cil; cil = cil->link)
	    if(cil->inst->config) {
		error("%:%n is already configured by..", cs, cil->inst);
		info("%:..this configuration specification",
		     cil->inst->config);
	    } else
		cil->inst->config = cs;
    }
	
}

nArchitecture psr::get_architecture(nEntity e, nSimpleName an)
{
    if(e == NULL || an == NULL)
	return NULL;

    vaul_library *l = e->owner? e->owner->lib : NULL;
    if(l == NULL)
	l = libs->get_worklib();

    nArchitecture a = NULL;

    char *lid = libs->architecture_name(e->id, an->id);
    vaul_design_unit *du = l->get(lid, libs);
    if(du == NULL)
	error("%:%n is undeclared", an, an);
    else if(du->is_error())
	error("%s: %s", lid, du->get_error_desc());
    else if(du->tree && du->tree->is(nkArchitecture)) {
	a = nArchitecture(du->tree);
	cur_du->uses(du);
    } else
	error("%:%n is not an architecture", an, an);

    release_ref(du);
    return a;
}

static void add(nBlockConfig b, nBaseConfig c)
{
    nBaseConfig *cp;
    for(cp = &b->configs; *cp; cp = &(*cp)->next_config)
	;
    *cp = c;
}

nBlockConfig psr::start_BlockConfig(nName n)
{
    assert(cur_scope);

    nBlock block = NULL;

    if(!n->is(nkSimpleName))
	error("%:block specification must be a simple name", n);
    else if(cur_scope->is(nkConfiguration)) 
	block = get_architecture(nConfiguration(cur_scope)->entity,
				 nSimpleName(n));
    else if(cur_scope->is(nkBlockConfig))
	block = nBlockStat(find_single_decl(n, nkBlockStat,
					    "block statement"));
    else if(cur_scope->is(nkCompConfig))
	info("XXX - no component config");

    nBlockConfig bc = mBlockConfig(n->lineno, NULL, block);
    bc->continued = block;

    if(cur_scope->is(nkBlockConfig))
	add(nBlockConfig(cur_scope), bc);

    add_decl(bc);
    push_scope(bc);
    return bc;
}

static nCompConfig get_CompConfig(nBlockConfig bc, nComponentInst ci)
{
    for(nBaseConfig c = bc->configs; c; c = c->next_config) {
	if(!c->is(nkCompConfig))
	    continue;
	nCompConfig cc = nCompConfig(c);
	for(nCompInstList cil = cc->comps; cil; cil = cil->link)
	    if(cil->inst == ci)
		return cc;
    }
    return NULL;
}
   
static bool bound_by_name(nComponentInst ci, nBlockConfig bc)
{
    nComponentSpec s = NULL;
    if(bc) {
	nCompConfig cc = get_CompConfig(bc, ci);
	s = cc? cc->comp_spec : NULL;
    } else
	s = ci->config? ci->config->comps : NULL;

    return s && s->ids && s->ids->is(nkInstList_Ids);
}

nCompInstList psr::list_comps(nComponentSpec c, nScope s)
{
    if(c == NULL || c->ids == NULL || s == NULL)
	return NULL;

    nCompInstList l = NULL;

    if(c->ids->is(nkInstList_Ids)) {
	for(nIdList idl = nInstList_Ids(c->ids)->ids; idl; idl = idl->link) {
	    sSimpleName sn(idl->lineno, idl->id);
	    nComponentInst ci = nComponentInst(
		find_single_decl(&sn, nkComponentInst,
				 "component instantiation"));
	    if(ci && ci->binding && ci->binding->unit && c->comp 
	       && ci->binding->unit != c->comp) {
		error("%:configured unit %n does not match"
		      " instantiated unit %n", idl, c->comp,
		      ci->binding->unit);
	    } else
		l = mCompInstList(l, ci);
	}
    } else if(c->ids->is(nkInstList_ALL) || c->ids->is(nkInstList_OTHERS)) {
	nBlock b;
	nBlockConfig bc;
	if(s->is(nkBlockConfig)) {
	    bc = nBlockConfig(s);
	    b = bc->block;
	    if(b == NULL)
		return NULL;
	} else if(s->is(nkBlock)) {
	    bc = NULL;
	    b = nBlock(s);
	} else
	    return NULL;

	for(nConcurrentStatement cs = b->stats; cs; cs = cs->next_stat) {
	    if(!cs->is(nkComponentInst))
		continue;
	    nComponentInst ci = nComponentInst(cs);
	    if(ci->binding && ci->binding->unit == c->comp
	       && !(c->ids->is(nkInstList_OTHERS) && bound_by_name(ci, bc)))
		l = mCompInstList(l, ci);
	}
    }
    
    return l;
}

static inline nComponent get_bound_unit(nComponentInst ci)
{
    if(ci && ci->config && ci->config->binding)
	return ci->config->binding->unit;
    return NULL;
}

nCompConfig psr::start_CompConfig(int ln, nComponentSpec c,
				  nIncrementalBindingIndic ib)
{
    // XXX - do lots of checking

    if(c == NULL)
	return NULL;

    nCompConfig cc = mCompConfig(ln, NULL, c, list_comps(c, cur_scope), NULL);

    if(ib && ib->unit == NULL) {
	for(nCompInstList cil = cc->comps; cil; cil = cil->link) {
	    nComponent u = get_bound_unit(cil->inst);
	    if(u) {
		if(ib->unit && ib->unit != u)
		    info("XXX - incrementally bound component instantiations"
			 " must all be bound to the same unit");
		else
		    ib->unit = u;
	    }
	}
    }

    cc->binding = ib? build_BindingIndic(ib) : NULL;

    if(cur_scope->is(nkBlockConfig))
	add(nBlockConfig(cur_scope), cc);

    add_decl(cc);
    push_scope(cc);
    return cc;
}

void psr::check_BlockConfig(nBlockConfig bc)
{
    // see if any component instantiation is bound more than once
    //
    for(nBaseConfig c = bc->configs; c; c = c->next_config) {
	if(!c->is(nkCompConfig))
	    continue;
	nCompConfig cc = nCompConfig(c);
	for(nCompInstList cil = cc->comps; cil; cil = cil->link) {
	    nCompConfig cc2 = get_CompConfig(bc, cil->inst);
	    if(cc2 != cc) {
		error("%:%n is already configured by..", cc, cil->inst);
		info("%:..this component configuration", cc2);
	    }
	}
    }
}
