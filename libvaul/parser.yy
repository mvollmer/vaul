/* bison grammar.

 This files is based on `vhdl.yacc' by Thomas Dettmer. 'vhdl.yacc'
 carries the following notice:

 ************** Syntax for VHDL in YACC format ******************
 *
 * Version 0.2 Wed Aug 11, 1993
 * 
 * The original was a VHDL parser description to be used with GMD Compiler
 * Tool Box
 * written from:
 * Jan den Ouden, Jaap Hofstede
 * Department of Computer Science (INF/SPA)
 * University of Twente
 * PB 217, 7500 AE ENSCHEDE, The Netherlands
 * Tel: +31 53 893788
 * Email: hofstede@cs.utwente.nl
 * avail at: ftp.cs.utwente.nl in pub/src/VHDL/Grammar
 *
 * author of YACC transformation:
 * Thomas Dettmer
 * Dortmund University
 * Dept. of Computer Scienc, LS1
 * PB 500 500
 * D-44221 Dortmund (Germany)
 * Phone: +49-231-755-6464
 * e-mail: dettmer@ls1.informatik.uni-dortmund.de
 *
 * Please report discovered bugs, critics, suggestions
 * and changes you make to dettmer@ls1.informatik.uni-dortmund.de
 * 
 * at the end of this file there is a list of users, who discovered bugs,
 * and suggested solutions. additionally there's a list of known problems
 *
 ****************************************************************
 * 
 * This file is intended not to be used for commercial purposes
 * without permission of the University of Twente and permission
 * of the University of Dortmund
 *
 * NOTE THAT THERE IS NO WARRANTY FOR CORRECTNES, COMPLETENESS, SUPPORT
 * OR ANYTHING ELSE.
 *
 ****************************************************************
 *
 * Note that the context-free syntax of VHDL in [LRM] is ambiguous 
 * (and so is not LR(1)) and that the language VHDL makes it impossible to
 * construct a non-ambiguous context-free syntax. Therefore a deterministic
 * parser based on a non-ambiguous context-free syntax has to accept a too
 * large class of inputs. Inputs which are accepted by the parser but are not
 * correct VHDL must be rejected by static semantical checks.
 *******************************************************

 Modifications and actions written by mvo@nt.e-technik.uni-dortmund.de

 NOTE: The original version of the grammar was free of conflicts.
 They have all been introduced by my careless tampering.

*/

%{

#include <vaul/tree.h>
#include <vaul/lexer.h>
#include <vaul/parser.h>
#include <vaul/design_unit.h>
#include <vaul/library.h>
#include <vaul/list.h>

#include <stdio.h>
#include <stdarg.h>
#include <vaulutil.h>
#include <assert.h>

#define yylex(sem, loc) lex->lex(sem, loc)
#define yyparse		vaul_parser::bison_parse
#define yyerror		BisonError

#define YYSTYPE vaul_yystype
#define YYLTYPE vaul_yyltype

#if 1
// Erweiterte Fehlermeldungen waeren ja ganz schön, aber die
// Namen fuer die Token sind nicht sehr elegant.
#define YYDEBUG 1
#define YYERROR_VERBOSE
#endif

#define NEW_NODE_PLACEMENT (pool)
#include "tree.m"

// XXX - tidy up, used only for RANGE attribute resolution

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

%}

%pure_parser

%token
t_ACCESS
t_AFTER
t_ALIAS
t_ALL
t_AND
t_ARCHITECTURE
t_ARRAY
t_ASSERT
t_ATTRIBUTE
t_BEGIN
t_BLOCK
t_BODY
t_BUFFER
t_BUS
t_CASE
t_COMPONENT
t_CONFIGURATION
t_CONSTANT
t_DISCONNECT
t_DOWNTO
t_ELSE
t_ELSIF
t_END
t_ENTITY
t_EXIT
t_FILE
t_FOR
t_FUNCTION
t_GENERATE
t_GENERIC
t_GUARDED
t_IF
t_IN
t_INOUT
t_IS
t_LABEL
t_LIBRARY
t_LINKAGE
t_LOOP
t_MAP
t_NAND
t_NEW
t_NEXT
t_NOR
t_NULL
t_OF
t_ON
t_OPEN
t_OR
t_OTHERS
t_OUT
t_PACKAGE
t_PORT
t_PROCEDURE
t_PROCESS
t_RANGE
t_RECORD
t_REGISTER
t_REPORT
t_RETURN
t_SELECT
t_SEVERITY
t_SIGNAL
t_SUBTYPE
t_THEN
t_TO
t_TRANSPORT
t_TYPE
t_UNITS
t_UNTIL
t_USE
t_VARIABLE
t_WAIT
t_WHEN
t_WHILE
t_WITH
t_XOR

%nonassoc t_EQSym t_NESym t_LTSym t_LESym t_GTSym t_GESym
%left t_Plus t_Minus t_Ampersand
%left MED_PRECEDENCE
%left t_Star t_Slash t_MOD t_REM
%nonassoc t_DoubleStar t_ABS t_NOT MAX_PRECEDENCE

%token
t_Apostrophe
t_LeftParen
t_RightParen
t_Comma
t_VarAsgn
t_Colon
t_Semicolon

t_Arrow
t_Box
t_Bar
t_Dot

%token <text>
    t_AbstractLit t_CharacterLit t_StringLit t_Identifier



%type <id>
    designator opt_entity_end opt_package_end
    opt_package_body_end opt_architecture_end
    opt_configuration_end opt_t_Identifier
    suffix decl_Identifier enumeration_literal
    opt_designator opt_proc_label opt_arch_id

%type <text>
    opt_t_AbstractLit

%type <tree.IdList>
    idf_list lib_clause

%type <tree.Name>
    name name2 mark ifts_name formal_part simple_sel_name

%type <tree.AttributeName>
    attribute_name attribute_name_with_param

%type <tree.SelName>
    sel_name

%type <tree.SelNameList>
    sel_list use_clause

%type <tree.Type>
    opt_type_def type_definition enumeration_type_definition
    subtype_indic subtype_indic1 physical_type_definition
    unconstrained_array_definition constrained_array_definition
    file_type_definition

%type <tree.PhysicalUnit>
    primary_unit_decl secondary_unit_decl secondary_unit_decls

%type <tree.PhysicalLiteralRef>
    physical_literal_no_default physical_literal    

%type <tree.Range>
    range_constraint range_spec

%type <tree.PreIndexConstraint>
    opt_index_constraint index_constraint opt_index_association_list

%type <tree.EnumLiteral>
    enumeration_literal_decls opt_more_enumeration_literals

%type <dir>
    direction

%type <tree.Expr>
    expr opt_var_init and_relation or_relation xor_relation
    relation primary opt_expr qualified_expr
    condition literal opt_when opt_wait_until opt_wait_for
    opt_wavefrm_after block_guard target opt_assertion_severity
    opt_assertion_report allocator opt_open_mode opt_file_name

%type <tree.Node>
    expr_or_attr relation_or_attr primary_or_attr actual_part

%type <tree.Subprogram>
    subprog_spec proc_or_func_spec

%type <tree.Interface>
    opt_interf_list interf_list opt_more_interf_elements interf_element
    port_interf_list generic_interf_list

%type <tree.Statement>
    seq_stats seq_stat return_stat variable_assign_stat if_stat if_stat_1
    if_stat_2 loop_stat rev_seq_stats next_stat exit_stat null_stat
    wait_stat signal_assign_stat assertion_stat case_stat

%type <tree.ProcedureCallStat>
    procedure_call_stat

%type <tree.CaseAlternative>
    case_stat_alternative more_case_stat_alternatives

%type <tree.Label>
    opt_label

%type <tree.GenAssocElem>
    gen_association_list gen_association_list_1 gen_association_element
    association_element association_list association_list_1 discrete_range
    discrete_range1 opt_more_discrete_ranges

%type <tree.NamedAssocElem>
    opt_port_map opt_generic_map block_generic_stuff block_port_stuff
    port_map generic_map named_association_list opt_attribute_param

%type <tree.SignalList>
    sensitivity_list reverse_opt_more_sensitivities signal_name
    opt_wait_on opt_proc_sensitivity_list

%type <tree.IterationScheme>
    while_scheme for_scheme opt_iteration_scheme iteration_scheme
    generation_scheme

%type <tree.WaveformElement>
    wavefrm reverse_more_wavefrm wavefrm_element

%type <obj_class>
    object_class opt_object_class

%type <signal_kind>
    signal_kind opt_signal_kind

%type <mode>
    mode opt_mode

%type <boolean>
    opt_t_BUFFER opt_t_TRANSPORT

%type <tree.ConcurrentStatement>
    concurrent_stats reverse_concurrent_stats concurrent_stat
    proc_stat block_stat comp_inst_stat opt_entity_stats
    concurrent_signal_assign_stat concurrent_assertion_stat
    concurrent_procedure_call

%type <tree.CondalSignalAssign>
    condal_signal_assign

%type <tree.CondalWaveform>
    condal_wavefrms condal_wavefrms_1

%type <tree.Component>
    comp_mark comp_inst_unit entity_aspect

%type <tree.ComponentSpec>
    comp_spec

%type <tree.BindingIndic>
    binding_indic 

%type <tree.IncrementalBindingIndic>
    opt_comp_binding_indic incremental_binding_indic
    pre_binding_indic

%type <tree.InstList>
    inst_list

%type <tree.IndexType>
    index_subtype_definition index_subtype_defs opt_more_index_subtype_defs

%type <tree.RecordType>
    record_type_definition

%type <tree.RecordElement>
    element_decl opt_more_element_decls

%type <tree.AmbgAggregate>
    aggregate

%type <tree.ElemAssoc>
    rev_element_association_list2 element_association

%type <tree.Choice>
    choices opt_more_choices choice

%type <tree.AccessType>
    access_type_definition

%type <tree.BaseConfig>
    config_item comp_config

%type <tree.BlockConfig>
    block_config opt_block_config

%type <tree.EntitySpec>
    entity_spec

%type <tree.IdList>
    opt_more_entity_name_list

%type <tree.EntityNameList>
    entity_name_list

%type <kind>
    entity_class

%%

start:
	opt_design_unit { YYACCEPT; }	

opt_design_unit:    
	/* nothing */  { eof = true; }
    |	design_unit    { eof = false; }
    ;

designator:
	t_Identifier	{ $$ = $1; }
    |	t_StringLit	{ $$ = $1; }
    ;

literal:
	t_AbstractLit		    { $$ = mLiteralRef(@1, $1); }
    |	t_CharacterLit		    { $$ = build_Expr(mSimpleName(@1, $1)); }
    |	physical_literal_no_default { $$ = $1; }
    |	t_NULL			    { $$ = mAmbgNullExpr(@1); }
    ;

enumeration_literal:
	t_CharacterLit	{ $$ = $1; }
    |	t_Identifier	{ $$ = $1; }
    ;

physical_literal:
	opt_t_AbstractLit t_Identifier
	 { $$ = build_PhysicalLiteralRef(@1, $1, $2); }
    ;

opt_t_AbstractLit:  
	/* nothing */	{ $$ = NULL; }
    |	t_AbstractLit
    ;

physical_literal_no_default:
	t_AbstractLit t_Identifier
	 { $$ = build_PhysicalLiteralRef(@1, $1, $2); }
    ;

idf_list:
	t_Identifier			{ $$ = mIdList(@1, $1, NULL); }
    |	idf_list t_Comma t_Identifier	{ $$ = mIdList(@3, $3, $1); }
    ;

/*------------------------------------------
--  Desing Unit
--------------------------------------------*/

design_unit:
	context_list lib_unit
    ;

context_list:
	/* nothing */  
    |	context_list context_item
    ;

lib_unit:
	entity_declaration
    |	configuration_declaration		
    |	package_declaration
    |	architecture_body 
    |	package_body
    ;

context_item:
	lib_clause	{ add_libs($1); }
    |	use_clause	{ use($1); }
    ;

lib_clause:
	t_LIBRARY idf_list t_Semicolon	{ $$ = reverse($2); }
    ;

use_clause:
	t_USE sel_list t_Semicolon	{ $$ = reverse($2); }
    ;

sel_list:
	sel_name			{ $$ = mSelNameList(@1, $1, NULL); }
    |	sel_list t_Comma sel_name	{ $$ = mSelNameList(@3, $3, $1); }
    ;

/*------------------------------------------
--  Library Units
--------------------------------------------*/

entity_declaration:
    t_ENTITY t_Identifier t_IS
    {
	nEntity e = mEntity(@2, $2);
	start(e);
	$<tree.Block>$ = e;
    }
	opt_generic_clause
	opt_port_clause
	entity_decl_part
	opt_entity_stats
    t_END opt_entity_end t_Semicolon
    {
	$<tree.Block>4->stats = $8;
	if($10 && !vaul_name_eq($10, $2))
	    error("%s does not match entity name %s", $10, $2);
	pop_scope($<tree.Block>4);
    };

opt_generic_clause:
	/* nothing */
    |	generic_clause
    ;

generic_clause:
	t_GENERIC generic_interf_list t_Semicolon
	{
	    if(cur_scope->is(nkComponent))
		nComponent(cur_scope)->first_generic = $2;
	}
    ;

opt_port_clause:
	/* nothing */
    |	port_clause
    ;

port_clause:	    
	t_PORT port_interf_list t_Semicolon
	{
	    if(cur_scope->is(nkComponent))
		nComponent(cur_scope)->first_port = $2;
	}
    ;

entity_decl_part:
	/* nothing */
    |	entity_decl_part entity_decltve_item
    ;

opt_entity_stats:
	/* nothing */		 { $$ = NULL; }
    |	t_BEGIN concurrent_stats { $$ = $2; }
    ;

opt_entity_end:			
	t_ENTITY opt_t_Identifier   { $$ = $2; }
    |	opt_t_Identifier	    { $$ = $1; }
    ;

opt_t_Identifier:
	/* nothing */		    { $$ = NULL; }
    |	t_Identifier		    { $$ = $1; }
    ;

architecture_body:
    t_ARCHITECTURE t_Identifier t_OF t_Identifier t_IS
    {
	nArchitecture a = mArchitecture(@2, $2);
	if(a->continued = get_entity($4)) {
	    a->first_generic = nComponent(a->continued)->first_generic;
	    a->first_port = nComponent(a->continued)->first_port;
	}
	start(a);
	$<tree.Block>$ = a;
    }
	architecture_decl_part
    t_BEGIN
	concurrent_stats
    t_END opt_architecture_end t_Semicolon
    {
	$<tree.Block>6->stats = $9;
	if($11 && !vaul_name_eq($11, $2))
	    error("%s does not match architecture name %s", $11, $2);
	pop_scope($<tree.Block>6);
    };

architecture_decl_part:
    |	architecture_decl_part block_decltve_item
    ;

opt_architecture_end:		    
	t_ARCHITECTURE opt_t_Identifier { $$ = $2; }
    |	opt_t_Identifier		{ $$ = $1; }
    ;


configuration_declaration:
    t_CONFIGURATION t_Identifier t_OF t_Identifier t_IS
    {
    	nConfiguration c = mConfiguration(@2, $2, get_entity($4));
	start(c);
	if(c->entity) {
    	    // XXX - check where this is really usefull
    	    // c->first_generic = c->entity->first_generic;
	    // c->first_port = c->entity->first_port;
	}
    }
	configuration_decl_part
	block_config
    t_END opt_configuration_end t_Semicolon
    {
	if($10 && !vaul_name_eq($10, $2))
	    error("%s does not match configuration name %s", $10, $2);
    	if(cur_du->tree->is(nkConfiguration))
    	    nConfiguration(cur_du->tree)->config = $8;
	pop_scope(cur_du->tree);
    };

configuration_decl_part:
    | configuration_decl_part config_decltve_item
    ;

opt_configuration_end:
	t_CONFIGURATION opt_t_Identifier    { $$ = $2; }
    |	opt_t_Identifier		    { $$ = $1; }
    ;
    

package_declaration:
    t_PACKAGE t_Identifier t_IS
    {	if(vaul_name_eq($2, "standard"))
	    start(mStandardPackage(@2, $2));
	else
	    start(mPackage(@2, $2));
    }
	package_decl_part
    t_END opt_package_end t_Semicolon
    {
	if($7 && !vaul_name_eq($7, $2))
	    error("%s does not match package name %s", $7, $2);
	pop_scope(cur_du->tree);
    };

package_decl_part:
    |	package_decl_part package_decltve_item
    ;

opt_package_end:
	t_PACKAGE opt_t_Identifier	{ $$ = $2; }
    |	opt_t_Identifier		{ $$ = $1; }
    ;


package_body:
    t_PACKAGE t_BODY t_Identifier t_IS
    {
	nPackageBody pb = mPackageBody(@3, $3);
        pb->continued = get_package($3);
	start(pb);
    }
	package_body_decl_part
    t_END opt_package_body_end t_Semicolon
    {
	if($8 && !vaul_name_eq($8, $3))
	    error("%s does not match package name %s", $8, $3);
	pop_scope(cur_du->tree);
    };

package_body_decl_part:
    |	package_body_decl_part package_body_decltve_item
    ;

opt_package_body_end:
	t_PACKAGE t_BODY opt_t_Identifier   { $$ = $3; }
    |	opt_t_Identifier		    { $$ = $1; }
    ;

/*------------------------------------------
--  Declarative Item
--------------------------------------------*/

common_decltve_item:
	type_decl
    |	subtype_decl  
    |	constant_decl
    |	file_decl
    |	alias_decl
    |	subprog_decl
    |	use_clause	{ use($1); }
    ;

entity_decltve_item:
	common_decltve_item
    |	subprog_body
    |	attribute_decl
    |	attribute_spec
    |	disconnection_spec
    |	signal_decl
    ;

block_decltve_item:
	common_decltve_item
    |	subprog_body
    |	comp_decl
    |	attribute_decl
    |	attribute_spec
    |	config_spec
    |	disconnection_spec
    |	signal_decl
    ;

package_decltve_item:
	common_decltve_item
    |	comp_decl
    |	attribute_decl
    |	attribute_spec
    |	disconnection_spec
    |	signal_decl
    ;

package_body_decltve_item:
	common_decltve_item
    |	subprog_body
    ;

subprog_decltve_item:
	common_decltve_item
    |	subprog_body
    |	attribute_decl
    |	attribute_spec
    |	variable_decl
    ;

proc_decltve_item:
	common_decltve_item
    |	subprog_body
    |	attribute_decl
    |	attribute_spec
    |	variable_decl
    ;

config_decltve_item:
	attribute_spec
    |	use_clause	{ use($1); }
    ;

/*------------------------------------------
--  Subprograms
--------------------------------------------*/

subprog_decl:
	subprog_spec t_Semicolon
	{
	    add_decl($1);
	}
    ;

subprog_spec:
	proc_or_func_spec
	{
	    for(nInterface i = $1->first_formal; i; i = i->next_element) {
		validate_interface($1, i);
		add_decl($1, i, 0, NULL);
	    }
	    $$ = $1;
	}
    ;

proc_or_func_spec:
	t_PROCEDURE t_Identifier opt_interf_list
	 { $$ = mProcedure(@2, $2, $3); }
    |	t_FUNCTION designator opt_interf_list t_RETURN mark
	 { $$ = mFunction(@2, $2, $3, true, get_type($5)); }
    ;

opt_interf_list:
	/* nothing */	{ $$ = NULL; }
    |	interf_list
    ;

subprog_body:
	subprog_spec t_IS
	 {
	    nSubprogram s = nSubprogram(add_decl($1));
	    assert(s->is(nkSubprogram));
	    cur_body = mSubprogramBody(@2, NULL);
	    cur_body->continued = s;
	    nDeclaration prev;
	    for(prev = cur_scope->first_decl; prev; prev = prev->next_decl)
		if(prev->is(nkSubprogramBody)
		&& nSubprogramBody(prev)->continued == s) {
		    error("redefinition of %n", s);
		    info("%: here is the previous definition", prev);
		    break;
		}
	    add_decl(cur_body);
	    push_scope(cur_body);
	 }
	    subprog_body_decl_part
	t_BEGIN
	    seq_stats
	t_END opt_designator t_Semicolon
	 {
	    if($8 && cur_body->continued
	    && !vaul_name_eq($8, cur_body->continued->id))
		error("%s does not match subprogram designator %s", $8,
		 cur_body->continued->id);
	    cur_body->stats = $6;
	    pop_scope(cur_body);
	 }
    ;

opt_designator:
	/* nothing */	{ $$ = NULL; }
    |	designator
    ;

subprog_body_decl_part:
	/* nothing */
    |	subprog_body_decl_part subprog_decltve_item
    ;


/*--------------------------------------------------
--  Interface Lists and Associaton Lists
----------------------------------------------------*/

port_interf_list:
	interf_list
	{
	    for(nInterface i = $1; i; i = i->next_element) {
		validate_port(i);
		add_decl(i);
	    }
	    $$ = $1;
	}
    ;

generic_interf_list:
	interf_list
	{
	    for(nInterface i = $1; i; i = i->next_element) {
		validate_generic(i);
		add_decl(i);
	    }
	    $$ = $1;
	}
    ;

interf_list:
	t_LeftParen interf_element opt_more_interf_elements t_RightParen
	{
	    nInterface i;
	    for(i = $2; i && i->next_element; i = i->next_element)
		;
	    if(i) {
		i->next_element = $3;
		$$ = $2;
	    } else
		$$ = $3;
	 }
    ;

opt_more_interf_elements:
	/* nothing */	    { $$ = NULL; }
    |	opt_more_interf_elements t_Semicolon interf_element
	{
	    nInterface i;
	    for(i = $1; i && i->next_element; i = i->next_element)
		;
	    if(i) {
		i->next_element = $3;
		$$ = $1;
	    } else
		$$ = $3;
	}
    ;

interf_element:
	opt_object_class idf_list t_Colon opt_mode subtype_indic
	opt_t_BUFFER opt_var_init
    {	$$ = NULL;
	overload_resolution($7, $5);
	for(nIdList idl = $2; idl; idl = idl->link)
	    $$ = mInterface(@2, idl->id, $5, $7, $$, $1, $4, $6);
    }
    ;

opt_t_BUFFER:
	/* nothing */	{ $$ = false; }
    |	t_BUFFER	{ $$ = true; }
    ;

opt_mode:
	/* nothing */	{ $$ = Mode_None; }
    |	mode
    ;
  
opt_object_class:
	/* nothing */	{ $$ = ObjClass_None; }
    |	object_class
    ;
  
mode:
	t_IN	    { $$ = Mode_In; }
    |	t_OUT	    { $$ = Mode_Out; }
    |	t_INOUT	    { $$ = Mode_InOut; }
    |	t_BUFFER    { $$ = Mode_Buffer; }
    |	t_LINKAGE   { $$ = Mode_Linkage; }
    ;

association_list:
	t_LeftParen association_element association_list_1 t_RightParen
	 { $$ = $2; $2->next = reverse($3); } 
    ;

named_association_list:
	association_list
	{   
	    $$ = prepare_named_assocs($1)? nNamedAssocElem($1) : NULL;
	}
    ;

association_list_1:
	/* nothing */	{ $$ = NULL; }
    |	association_list_1 t_Comma association_element
	 { $3->next = $1; $$ = $3; }
    ;


gen_association_list:
	t_LeftParen gen_association_element gen_association_list_1 t_RightParen
	{
	    $$ = reverse($3);
	    if($2) {
		$2->next = $$;
		$$ = $2;
	    }
	} 
    ;

gen_association_list_1:
	/* nothing */	{ $$ = NULL; }
    |	gen_association_list_1 t_Comma gen_association_element
	{
	    $$ = $1;
	    if($3) {
		$3->next = $$;
		$$ = $3;
	    }
	}
    ;

association_element:
	formal_part t_Arrow actual_part
	{
	    $$ = mNamedAssocElem(@2, NULL, $1, validate_Expr($3));
	}
    |	actual_part
	{
	    if($1 && $1->is(nkRange))
		$$ = mRangeAssocElem(@1, NULL, nRange($1));
	    else 
		$$ = mNamedAssocElem(@1, NULL, NULL, validate_Expr($1));
	}
    ;

gen_association_element:
    	association_element
    |  	discrete_range1
    ;

formal_part:
	name
    ;

actual_part:
	expr_or_attr
    |	t_OPEN	{ $$ = mOpenExpr(@1); }
    ;

/*--------------------------------------------------
--  Names and Expressions
----------------------------------------------------*/

mark: 
	t_Identifier	{ $$ = mSimpleName(@1, $1); }
    |	sel_name	{ $$ = $1; }
    ;

expr:
	expr_or_attr
	{
	    $$ = validate_Expr($1);
	}
    ;

expr_or_attr:
	and_relation	    { $$ = $1; }
    |	or_relation	    { $$ = $1; }
    |	xor_relation	    { $$ = $1; }
    |	relation_or_attr    { $$ = $1; }
    |	relation t_NAND relation
	 { $$ = build_bcall(@2, $1, "nand", $3); }
    |	relation t_NOR relation
	 { $$ = build_bcall(@2, $1, "nor", $3); }
    ;

relation:
	relation_or_attr
	{
	    $$ = validate_Expr($1);
	}
    ;

and_relation:
	relation t_AND relation
	 { $$ = build_bcall(@2, $1, "and", $3); }
    |	and_relation t_AND relation
	 { $$ = build_bcall(@2, $1, "and", $3); }
    ;

or_relation:
	relation t_OR relation
	 { $$ = build_bcall(@2, $1, "or", $3); }
    |	or_relation t_OR relation
	 { $$ = build_bcall(@2, $1, "or", $3); }
    ;

xor_relation:
	relation t_XOR relation
	 { $$ = build_bcall(@2, $1, "xor", $3); }
    |	xor_relation t_XOR relation
	 { $$ = build_bcall(@2, $1, "xor", $3); }
    ;

/* ;relation   : unary_operator primary	  */
relation_or_attr:
	primary_or_attr
    |	t_Plus primary	%prec MED_PRECEDENCE
	 { $$ = build_bcall(@2, $2, "+", NULL); }
    |	t_Minus primary %prec MED_PRECEDENCE
	 { $$ = build_bcall(@2, $2, "-", NULL); }
    |	t_ABS primary
	 { $$ = build_bcall(@2, $2, "abs", NULL); }
    |	t_NOT primary
	 { $$ = build_bcall(@2, $2, "not", NULL); }
    |	primary t_DoubleStar primary
	 { $$ = build_bcall(@2, $1, "**", $3); }
    ;

/* ;	relation : relation binary_operator primary */
relation_or_attr:
	relation t_MOD relation
	 { $$ = build_bcall(@2, $1, "mod", $3); }
    |	relation t_REM relation
	 { $$ = build_bcall(@2, $1, "rem", $3); }
    |	relation t_Ampersand relation
	 { $$ = build_bcall(@2, $1, "&", $3); }
    |	relation t_Star relation
	 { $$ = build_bcall(@2, $1, "*", $3); }
    |	relation t_Plus relation
	 { $$ = build_bcall(@2, $1, "+", $3); }
    |	relation t_Minus relation
	 { $$ = build_bcall(@2, $1, "-", $3); }
    |	relation t_LESym relation
	 { $$ = build_bcall(@2, $1, "<=", $3); }
    |	relation t_GESym relation
	 { $$ = build_bcall(@2, $1, ">=", $3); }
    |	relation t_LTSym relation
	 { $$ = build_bcall(@2, $1, "<", $3); }
    |	relation t_GTSym relation
	 { $$ = build_bcall(@2, $1, ">", $3); }
    |	relation t_EQSym relation
	 { $$ = build_bcall(@2, $1, "=", $3); }
    |	relation t_NESym relation
	 { $$ = build_bcall(@2, $1, "/=", $3); }
    |	relation t_Slash relation
	 { $$ = build_bcall(@2, $1, "/", $3); }
    ;

primary_or_attr:
	name		{ $$ = build_Expr_or_Attr($1); }
    |	literal		{ $$ = $1; }
    |	aggregate	{ $$ = $1; }
    |	qualified_expr	{ $$ = $1; }
    |	allocator	{ $$ = $1; }
    |	t_LeftParen expr t_RightParen { $$ = $2 }
    ;

primary:
	primary_or_attr
	{
	    $$ = validate_Expr($1);
	}
    ;

name:
	mark
    |	name2
    ;

name2:
	t_StringLit	    { $$ = mOpSymbol(@1, $1); }
    |	attribute_name	    { $$ = $1; }
    |	ifts_name
    ;  

sel_name:
	name t_Dot suffix   { $$ = mSelName(@1, $1, $3); }
    ;

simple_sel_name:
	simple_sel_name t_Dot t_Identifier  { $$ = mSelName(@1, $1, $3); }
    |	t_Identifier			    { $$ = mSimpleName(@1, $1); }
    ;

suffix:
	designator	    { $$ = $1; }
    |	t_CharacterLit	    { $$ = $1; }
    |	t_ALL		    { $$ = "all"; }

ifts_name:
	mark gen_association_list   
	{
	    nName n = $1; // gcc bug?
	    if(n->is(nkAttributeName)) {
		nAttributeName(n)->first_actual =
		    prepare_named_assocs($2)? nNamedAssocElem($2) : NULL;
		$$ = n;
	    } else {
		if(validate_gen_assocs($2))
		    $$ = mIftsName(@1, n, $2);
		else
		    $$ = NULL;
	    }
	}
    |	name2 gen_association_list
	{
	    nName n = $1; // gcc bug?
	    if(n->is(nkAttributeName)) {
		nAttributeName(n)->first_actual =
		    prepare_named_assocs($2)? nNamedAssocElem($2) : NULL;
		$$ = n;
	    } else {
		if(validate_gen_assocs($2))
		    $$ = mIftsName(@1, n, $2);
		else
		    $$ = NULL;
	    }
	}
    ;

attribute_name:
	mark t_Apostrophe t_Identifier
	 { $$ = mAttributeName(@1, $1, $3, NULL); }
    |	name2 t_Apostrophe t_Identifier
	 { $$ = mAttributeName(@1, $1, $3, NULL); }
    |	mark t_Apostrophe t_RANGE
	 { $$ = mAttributeName(@1, $1, "RANGE", NULL); }
    |	name2 t_Apostrophe t_RANGE
	 { $$ = mAttributeName(@1, $1, "RANGE", NULL); }
    ;

attribute_name_with_param:
	attribute_name opt_attribute_param
	{
	    nAttributeName n = $$ = $1;
	    if(n)
		n->first_actual = $2;
	}
    ;

opt_attribute_param:
	/* nothing */  { $$ = NULL; }
    |	t_LeftParen expr t_RightParen
	 { $$ = mNamedAssocElem(@1, NULL, NULL, $2); }
    ;

aggregate:
	rev_element_association_list2 t_RightParen
	{
	    $$ = mAmbgAggregate(@1, reverse($1));
	}
    |	t_LeftParen choices t_Arrow expr t_RightParen
	{
	    $$ = mAmbgAggregate(@1, mElemAssoc(@3, NULL, $2, $4));
	}
    ;

rev_element_association_list2:
	t_LeftParen element_association t_Comma element_association
	{
	    if($4) {
		$4->next = $2;
		$$ = $4;
	    } else
		$$ = $2;
	}
    |	rev_element_association_list2 t_Comma element_association
	{
	    if($3) {
		$3->next = $1;
		$$ = $3;
	    } else
		$$ = $1;
	}
    ;

qualified_expr:
	mark t_Apostrophe t_LeftParen expr t_RightParen
	 { $$ = build_QualifiedExpr($1, $4); }
    |	mark t_Apostrophe aggregate
	 { $$ = build_QualifiedExpr($1, $3); }
    ;


allocator:
	t_NEW mark mark opt_index_association_list
	{
	    nType t = mAccessType(@1, NULL, build_SubType($2, $3, $4));
	    $$ = mNewExpr(@1, t, NULL);
	}
    |	t_NEW mark opt_index_association_list
	{
	    nType t = mAccessType(@1, NULL, build_SubType(NULL, $2, $3));
	    $$ = mNewExpr(@1, t, NULL);
	}
    |	t_NEW qualified_expr
	{
	    nExpr e = $2;
	    overload_resolution(e, nkType);
	    if(e) {
		assert(e->is(nkQualifiedExpr));
		nQualifiedExpr qe = nQualifiedExpr(e);
		$$ = mNewExpr(@1, mAccessType(@1, NULL, qe->type),
			      qe->expression);
	    } else
		$$ = NULL;
	}
    ;

opt_index_association_list:
	/* nothing */		{ $$ = NULL; }
    |	gen_association_list	{ $$ = build_PreIndexConstraint($1); }
    ;

/*--------------------------------------------------
--  Element Association and Choices
----------------------------------------------------*/

element_association:
	choices t_Arrow expr
	    { $$ = mElemAssoc(@2, NULL, $1, $3); }
    |	expr
	    { $$ = mElemAssoc(@1, NULL, NULL, $1); }
    ;

choices:
	choice opt_more_choices
	{
	    if($1) {
		$1->next = $2;
		$$ = $1;
	    } else
		$$ = $2;
	}
    ;

opt_more_choices:
	/* nothing */	{ $$ = NULL; }
    |	opt_more_choices t_Bar choice
	{
	    if($1) {
		nChoice c;
		for(c = $1; c->next; c = c->next)
		    ;
		c->next = $3;
		$$ = $1;
	    } else
		$$ = $3;
	}		    
    ;

choice:
	expr_or_attr
	{
	    nNode e = $1;
	    if(e->is(nkArrayRange))
		$$ = mRangeChoice(@1, NULL,
				  mRangeAssocElem(@1, NULL, nArrayRange(e)));
	    else
		$$ = mExprChoice(@1, NULL, validate_Expr(e));
	}
    |	discrete_range1
	{
	    $$ = mRangeChoice(@1, NULL, $1);
	}
    |	t_OTHERS
	{
	    $$ = mOthersChoice(@1, NULL);
	}
    ;

/*--------------------------------------------------
--  Type Declarations
----------------------------------------------------*/

decl_Identifier:
	t_Identifier { start_decl($1); $$ = $1; }
    ;

type_decl:
	t_TYPE decl_Identifier opt_type_def t_Semicolon
	{ 
	    nType t = nType(add_decl(cur_scope, $3, @2, $2));
	    add_predefined_ops(t);
	}
    ;

opt_type_def:
	/* nothing */		{ $$ = mIncompleteType(0, NULL); }
    |	t_IS type_definition	{ $$ = $2; }
    ;

type_definition:
	enumeration_type_definition     { $$ = $1; }
    |	range_constraint		{ $$ = build_SubType_def(@1, $1); }
    |	physical_type_definition        { $$ = $1; }
    |	unconstrained_array_definition  { $$ = $1; }
    |	constrained_array_definition    { $$ = $1; }
    |	record_type_definition          { $$ = $1; }
    |	access_type_definition          { $$ = $1; }	
    |	file_type_definition            { $$ = $1; }
    ;

enumeration_type_definition:
	t_LeftParen enumeration_literal_decls t_RightParen
	{
	    $$ = mEnumType(0, NULL, $2);
	    for(nEnumLiteral el = $2; el; el = el->next)
		el->type = nEnumType($$);
	}
    ;

enumeration_literal_decls:
	enumeration_literal opt_more_enumeration_literals
	 { $$ = nEnumLiteral(add_decl(mEnumLiteral(@1, $1, NULL, $2))); }

opt_more_enumeration_literals:
	/* nothing */			    { $$ = NULL; }
    |	t_Comma enumeration_literal_decls   { $$ = $2; }
    ;

physical_type_definition:
	range_constraint t_UNITS
	    primary_unit_decl
	    secondary_unit_decls
	t_END t_UNITS
	{
	    nPhysicalType base = mPhysicalType(0, NULL, $3, NULL);
	    if($3) {
		$3->next = reverse($4);
		for(nPhysicalUnit pu = $3; pu; pu = pu->next) {
		    if(pu->value && pu->value->unit
		    && pu->value->unit->type != base)
			error("%:value of unit %n must be from its"
			      " physical type", pu, pu);
		    pu->type = base;
		}
	    }
	    $$ = base->declaring_subtype = build_SubType_def(@1, $1, base);
	}
    ;

secondary_unit_decls:
	/* nothing */	{ $$ = NULL; }
    |	secondary_unit_decls secondary_unit_decl
	{
	    if($2) {
		$2->next = $1;
		$$ = $2;
	    } else
		$$ = $1;
	}
    ;

primary_unit_decl:
	t_Identifier t_Semicolon
	{
	    $$ = nPhysicalUnit(
		    add_decl(mPhysicalUnit(@1, $1, NULL, NULL, NULL))
		 );
	}
    ;

secondary_unit_decl:
	t_Identifier t_EQSym physical_literal t_Semicolon
	{
	    if($3 && !is_integer_literal($3->value))
		error("secondary units must be integer multiples");
	    $$ = nPhysicalUnit(
		    add_decl(mPhysicalUnit(@1, $1, NULL, $3, NULL))
		 );
	}
    ;

unconstrained_array_definition:
	t_ARRAY t_LeftParen index_subtype_defs t_RightParen t_OF subtype_indic
	{
	    $$ = mArrayType(@1, NULL, $3, $6);
	}
    ;

index_subtype_defs:
	index_subtype_definition opt_more_index_subtype_defs
	{
	    if($1) {
		$1->next = $2;
		$$ = $1;
	    } else
		$$ = $2;
	}
    ;

opt_more_index_subtype_defs:
	/* nothing */
	{
	    $$ = NULL;
	}
    |	t_Comma index_subtype_definition opt_more_index_subtype_defs
	{
	    if($2) {
		$2->next = $3;
		$$ = $2;
	    } else
		$$ = $3;
	}
    ;

index_subtype_definition:
	mark t_RANGE t_Box
	{
	    $$ = mIndexType(@1, NULL, get_type($1));
	}
    ;

constrained_array_definition:
	t_ARRAY index_constraint t_OF subtype_indic
	{
	    $$ = build_constrained_array_type($2, $4);
	}
    ;

record_type_definition:
	t_RECORD element_decl opt_more_element_decls t_END t_RECORD
	{
	    $$ = mRecordType(@1, NULL, concat($2, $3));
	}
    ;

opt_more_element_decls:
	/* nothing */
	{
	    $$ = NULL;
	}
    |	opt_more_element_decls element_decl
	{
	    $$ = concat($1, $2);
	}
    ;

element_decl:
	idf_list t_Colon subtype_indic t_Semicolon
	{
	    $$ = NULL;
	    for(nIdList idl = reverse($1); idl; idl = idl->link)
		$$ = mRecordElement(idl->lineno, $$, idl->id, $3);
	    $$ = reverse($$);
	}
    ;

access_type_definition:
	t_ACCESS subtype_indic
	{
	    if($2 && $2->is(nkFileType)) {
		error("%:can't access files", $2);
		$2 = NULL;
	    }
	    $$ = mAccessType(@1, NULL, $2);
	}
    ;

file_type_definition:
	t_FILE t_OF mark
	{
	    nType ct = get_type($3);
	    if(ct)
		info("xxx - unchecked file type: %n", ct);
	    $$ = mFileType(@1, NULL, ct);
	}
    ;

/*--------------------------------------------------
--  Subtypes and Constraints
----------------------------------------------------*/

subtype_decl:
	t_SUBTYPE decl_Identifier t_IS subtype_indic t_Semicolon
	{
	    if($4 && $4->scope)
		add_decl(mSubType(@2, $2, $4, NULL, NULL));
	    else
		add_decl(cur_scope, $4, @2, $2);
	}
    ;

subtype_indic:
	mark opt_index_constraint	{ $$ = build_SubType(NULL, $1, $2); }
    |	subtype_indic1
    ;

subtype_indic1:
	mark mark range_constraint	{ $$ = build_SubType($1, $2, $3); }
    |	mark range_constraint		{ $$ = build_SubType(NULL, $1, $2); }
    |	mark mark opt_index_constraint	{ $$ = build_SubType($1, $2, $3); }
    ;

opt_index_constraint:
	/* nothing */		{ $$ = NULL; }
    |	gen_association_list	{ $$ = build_PreIndexConstraint($1); }
    ;

range_constraint:
	t_RANGE range_spec  { $$ = $2; }
    ;

index_constraint:
	t_LeftParen discrete_range opt_more_discrete_ranges t_RightParen
	{
	    if($2) {
		$2->next = reverse($3);
		$$ = build_PreIndexConstraint($2);
	    } else
		$$ = build_PreIndexConstraint($3);
	}
    ;
  
opt_more_discrete_ranges:
	/* nothing */ 
	{
	    $$ = NULL;
	}
    |	opt_more_discrete_ranges t_Comma discrete_range
	{
	    if($3) {
		$3->next = $1;
		$$ = $3;
	    } else
		$$ = $1;
	}
    ;

discrete_range:
	subtype_indic
	{
	    if(is_discrete_type($1))
		$$ = mSubtypeAssocElem(@1, NULL, $1);
	    else {
		if($1)
		    error("%!%n is not a discrete type", lex, int(@1), $1);
		$$ = NULL;
	    }
	}
    |	range_spec
	{
	    $$ = mRangeAssocElem(@1, NULL, $1);
	}
    ;

discrete_range1:
	subtype_indic1
	{
	    if(is_discrete_type($1))
		$$ = mSubtypeAssocElem(@1, NULL, $1);
	    else {
		if($1)
		    error("%!%n is not a discrete type", lex, int(@1), $1);
		$$ = NULL;
	    }
	}
    |	expr direction expr	
	{
	    $$ = mRangeAssocElem(@1, NULL, mExplicitRange(@2, $2, $1, $3));
	}
    ;

range_spec:
	attribute_name_with_param 
	{
	    $$ = NULL;
	    if($1) {
		if(!vaul_name_eq($1->attribute, "range"))
		    error("'%s not valid here, only 'RANGE", $1->attribute);
		else {
		    // XXX - tidy up
		    vaul_decl_set *set = new vaul_decl_set(this);
		    find_decls(*set, $1);

		    if(set->multi_decls()) {
			nKind basic_k = nkInvalid;
			set->iterate(iterate_for_kind, &basic_k);
			assert(basic_k != nkDeclaration);
			nNode rc = build_AttrNode($1, set, basic_k);
			assert(!rc || rc->is(nkRange));
			$$ = nRange(rc);
		    }
		}
	    }
	}
    |	expr direction expr 
	{
	    $$ = mExplicitRange(@2, $2, $1, $3);
	}
    ;

direction:
	t_TO	    { $$ = DirectionUp; }
    |	t_DOWNTO    { $$ = DirectionDown; }
    ;

/*--------------------------------------------------
--  Objects, Aliases, Files, Disconnections
----------------------------------------------------*/

constant_decl:
	t_CONSTANT idf_list t_Colon subtype_indic opt_var_init t_Semicolon
	{
	    if($5 == NULL && !cur_scope->is(nkPackage))
		error("constants must be initialized");
	    nType t = adapt_object_type(ObjClass_Constant, $4, $5);
	    overload_resolution($5, t);
	    for(nIdList idl = $2; idl; idl = idl->link)
		add_decl(mConstant(@2, idl->id, t, $5));
	}
    ;
  

signal_decl:
	t_SIGNAL idf_list t_Colon subtype_indic
	 opt_signal_kind opt_var_init t_Semicolon
	{
	    nType t = adapt_object_type(ObjClass_Signal, $4, $6);
	    overload_resolution($6, t);
	    for(nIdList idl = $2; idl; idl = idl->link)
		add_decl(mSignal(@2, idl->id, t, $6, $5));
	}
    ;

opt_signal_kind:
	/* nothing */	{ $$ = SigKind_None; }
    |	signal_kind
    ;

variable_decl:
	t_VARIABLE idf_list t_Colon subtype_indic opt_var_init t_Semicolon
	{
	    nType t = adapt_object_type(ObjClass_Variable, $4, $5);
	    overload_resolution($5, t);
	    for(nIdList idl = $2; idl; idl = idl->link)
		add_decl(mVariable(@2, idl->id, t, $5, false));
	}
    ;

opt_var_init:  
	/* nothing */	    { $$ = NULL; }
    |	t_VarAsgn expr	    { $$ = $2; }
    ;

object_class:
	t_CONSTANT  { $$ = ObjClass_Constant; }
    |	t_SIGNAL    { $$ = ObjClass_Signal; }
    |	t_VARIABLE  { $$ = ObjClass_Variable; }
    |	t_FILE	    { $$ = ObjClass_File; }
    ;

signal_kind:
	t_BUS	    { $$ = SigKind_Bus; }
    |	t_REGISTER  { $$ = SigKind_Register; }
    ;

alias_decl:
	t_ALIAS t_Identifier t_Colon subtype_indic t_IS name t_Semicolon
	{
	    add_Alias(@1, $2, $4, $6);
	}
    ;  

file_decl:
	t_FILE t_Identifier t_Colon subtype_indic opt_open_mode opt_file_name
	  t_Semicolon
	{
	    add_File(@1, $2, $4, $6, $5);
	}
    ;

opt_open_mode:
	/* nothing */
	{
	    $$ = NULL; 
	}
    |	t_OPEN expr
	{
	    nExpr e = $2;
	    overload_resolution(e, std->predef_FILE_OPEN_KIND);
	    $$ = e;
	}
    ;

opt_file_name:
	/* nothing */
	{
	    $$ = NULL;
	}
    |	t_IS expr
	{
	    nExpr e = $2;
	    overload_resolution(e, std->predef_STRING);
	    $$ = e;
	}
    ;

disconnection_spec:
	t_DISCONNECT signal_list t_Colon mark t_AFTER expr t_Semicolon
	{
	    info("XXX - no disconnection specifications");
	}
    ;

signal_list:
	name opt_more_signal_list { }
    |	t_OTHERS                  { }
    |	t_ALL                     { }
    ;

opt_more_signal_list:
	/* nothing */
    |	opt_more_signal_list t_Comma name
    ;

/*--------------------------------------------------
--  Attribute Declarations and Specifications
----------------------------------------------------*/

attribute_decl:
	t_ATTRIBUTE t_Identifier t_Colon mark t_Semicolon
	{
	    nType t = get_type($4);
	    if(t) {
		nType bt = t->get_base();
		if(bt && bt->is(nkAccessType))
		    error("%!attributes can't have access types",
			     lex, int(@1));
		else if(bt && bt->is(nkFileType))
		    error("%!attributes can't have file types",
			     lex, int(@1));
		else
		    add_decl(mAttribute(@1, $2, t));
	    }
	}
    ;

attribute_spec:
	t_ATTRIBUTE t_Identifier t_OF entity_spec t_IS expr t_Semicolon
	{
	    bind_attrspec(mAttributeSpec(@1, $2, $4, $6));
	}
    ;

entity_spec:
	entity_name_list t_Colon entity_class
	{
	    $$ = mEntitySpec(@1, $1, $3);
	}
    ;

entity_name_list:
	designator opt_more_entity_name_list
	{
	    $$ = mEntityNameList_Ids(@1, mIdList(@1, $1, reverse($2)));
	}
    |	t_OTHERS
	{
	    $$ = mEntityNameList_OTHERS(@1);
	}
    |	t_ALL
	{
	    $$ = mEntityNameList_ALL(@1);
	}
    ;

opt_more_entity_name_list:
	/* nothing */	{ $$ = NULL; }
    |	opt_more_entity_name_list t_Comma designator
	{
	    $$ = mIdList(@3, $3, $1);
	}
    ;

entity_class:
	t_ENTITY	{ $$ = nkEntity; }
    |	t_ARCHITECTURE  { $$ = nkArchitecture; }
    |	t_PACKAGE       { $$ = nkPackage; }
    |	t_CONFIGURATION { $$ = nkConfiguration; }
    |	t_COMPONENT     { $$ = nkComponent; }
    |	t_LABEL         { $$ = nkLabel; }
    |	t_TYPE          { $$ = nkType; }
    |	t_SUBTYPE       { $$ = nkSubType; }
    |	t_PROCEDURE     { $$ = nkProcedure; }
    |	t_FUNCTION      { $$ = nkFunction; }
    |	t_SIGNAL        { $$ = nkSignal; }
    |	t_VARIABLE      { $$ = nkVariable; }
    |	t_CONSTANT      { $$ = nkConstant; }
    ;

/*--------------------------------------------------
--  Schemes
----------------------------------------------------*/

generation_scheme:
	if_scheme    { $$ = NULL; /* XXX */ }
    |	for_scheme   { $$ = $1; }
    ;

iteration_scheme:
	for_scheme   { $$ = $1; }
    |	while_scheme { $$ = $1; }
    ;

if_scheme:
	t_IF expr
	{
	    info("XXX - no if schemes");
	}
    ;

for_scheme:
	t_FOR t_Identifier t_IN discrete_range
	 { $$ = mPreForScheme(@1, $2, build_PreIndexConstraint($4)); }
    ;

while_scheme:
	t_WHILE condition { $$ = mWhileScheme(@1, $2); }
    ;

/*--------------------------------------------------
--  Concurrent Statements
----------------------------------------------------*/

concurrent_stats:
	reverse_concurrent_stats    { $$ = reverse($1); }
    ;

reverse_concurrent_stats:
	/* nothing */ { $$ = NULL; }
    |	reverse_concurrent_stats concurrent_stat
	{
	    if($2) {
		$2->next_stat = $1;
		$$ = $2;
	    } else
		$$ = $1;
	}
    ;
  
concurrent_stat:
	block_stat
    |	concurrent_assertion_stat  
    |	concurrent_procedure_call  
    |	concurrent_signal_assign_stat  
    |	comp_inst_stat	
    |	generate_stat  
	{
	    info("XXX - no generate statements");
	    $$ = mConcurrentStatement(@1, NULL);
	}
    |	proc_stat  
    ;

block_stat:
	t_Identifier t_Colon t_BLOCK block_guard opt_t_IS
	{
	    nBlockStat b = mBlockStat(@2, $1);
	    add_decl(b);
	    push_scope(b);
	    if($4)
		add_decl(mGuardSignal(@4, "GUARD", std->predef_BOOLEAN, $4,
				      SigKind_None));
	    $<tree.BlockStat>$ = b;
	}
	    block_generic_stuff
	    block_port_stuff
	    block_decl_part
	t_BEGIN
	    concurrent_stats
	t_END t_BLOCK opt_t_Identifier t_Semicolon
	{
	    nBlockStat b = $<tree.BlockStat>6;
	    b->stats = $11;
	    b->binding = build_BindingIndic(b, $7, $8);
	    if($14 && !vaul_name_eq($14, $1))
		error("'%s' does not match block label '%s'", $14, $1);
	    pop_scope(b);
	    $$ = b;
	}
    ;

block_decl_part:
	/* nothing */
    |	block_decl_part block_decltve_item
    ;

block_port_stuff:
	/* nothing */		    { $$ = NULL; }
    |	port_clause opt_port_map    { $$ = $2; }
    ;

block_generic_stuff:
	/* nothing */			{ $$ = NULL; }
    |	generic_clause opt_generic_map	{ $$ = $2; }
    ;

block_guard:
	/* nothing */			    { $$ = NULL; }
    |	t_LeftParen condition t_RightParen  { $$ = $2; }
    ;

comp_inst_stat:
	t_Identifier t_Colon comp_inst_unit
	    opt_generic_map
	    opt_port_map
	t_Semicolon
	{
	    $$ = mComponentInst(@3, $1, build_BindingIndic($3, $4, $5));
	    add_decl($$);
	}
    ;

comp_inst_stat:
	t_Identifier t_Colon comp_mark
	    t_GENERIC t_MAP named_association_list
	    opt_port_map
	t_Semicolon
	{
	    $$ = mComponentInst(@3, $1, build_BindingIndic($3, $6, $7));
	    add_decl($$);
	}

    |	t_Identifier t_Colon comp_mark
	    t_PORT t_MAP named_association_list
	t_Semicolon
	{
	    $$ = mComponentInst(@3, $1, build_BindingIndic($3, NULL, $6));
	    add_decl($$);
	}
    ;

/* NOTE: component instantiation statements without a keyword look
**	 like concurrent procedure calls
*/

comp_inst_unit:
	t_COMPONENT mark
	{
	    $$ = nComponent(find_single_decl($2, nkComponent, "component"));
	}
    |	entity_aspect
    ;

opt_generic_map:
	/* nothing */	{ $$ = NULL; }
    |	generic_map
    ;

generic_map:
	t_GENERIC t_MAP named_association_list    { $$ = $3; }
    ;

opt_port_map:
	/* nothing */			{ $$ = NULL; }
    |	port_map
    ;

port_map:
    	t_PORT t_MAP named_association_list	{ $$ = $3; }
    ;

concurrent_assertion_stat:
	t_Identifier t_Colon assertion_stat
	{
	    $$ = build_conc_AssertStat(@1, $1, nAssertStat($3));
	}
    |	assertion_stat
	{
	    $$ = build_conc_AssertStat(@1, NULL, nAssertStat($1));
	}
    ;

/* XXX - handle comp_inst_stat without keywords
 */
concurrent_procedure_call:
	t_Identifier t_Colon procedure_call_stat
	{
	    $$ = build_conc_ProcedureCall(@1, $1, $3);
	}
    |	procedure_call_stat
	{
	    $$ = build_conc_ProcedureCall(@1, NULL, $1);
	}

concurrent_signal_assign_stat:
    	t_Identifier t_Colon condal_signal_assign
       	{
	    $$ = build_condal_Process($1, $3);
	}

    |	condal_signal_assign
    	{
	    $$ = build_condal_Process(NULL, $1);
	}

    |	t_Identifier t_Colon sel_signal_assign
    	{
	    info("XXX - no selected signal assignment");
	    $$ = mConcurrentStatement(@1, NULL);
	}

    |	sel_signal_assign
    	{
	    info("XXX - no selected signal assignment");
	    $$ = mConcurrentStatement(@1, NULL);
	}
    ;

condal_signal_assign:
    	target t_LESym opts condal_wavefrms t_Semicolon
    	{
	    $$ = mCondalSignalAssign(@2, $1, false, $4);
	}
    ;

condal_wavefrms:
    	condal_wavefrms_1 wavefrm
    	{
    	    nCondalWaveform w = mCondalWaveform(@2, $2, NULL);
    	    if($$ = $1) {
            	while($1->else_wave)
    	    	    $1 = $1->else_wave;
    	    	$1->else_wave = w;
    	    } else
    	    	$$ = w;
    	}
    ;

condal_wavefrms_1:
    	/* nothing */ { $$ = NULL; }
    |	condal_wavefrms_1 wavefrm t_WHEN condition t_ELSE
    	{
    	    nCondalWaveform w = mCondalWaveform(@2, $2, $4);
    	    if($$ = $1) {
            	while($1->else_wave)
    	    	    $1 = $1->else_wave;
    	    	$1->else_wave = w;
    	    } else
    	    	$$ = w;
    	}
    ;

wavefrm:
	wavefrm_element reverse_more_wavefrm
	{
	    $2 = reverse($2);
	    if($1) {
		$1->next = $2;
		$$ = $1;
	    } else
		$$ = $2;
	}
    ;

reverse_more_wavefrm:
	/* nothing */	{ $$ = NULL; }
    |	reverse_more_wavefrm t_Comma wavefrm_element
	{
	    if($3) {
		$3->next = $1;
		$$ = $3;
	    } else
		$$ = $1;
	}
    ;

wavefrm_element:
	expr opt_wavefrm_after
	{
	    $$ = mWaveformElement($1, $2, NULL);
	}
    ;

opt_wavefrm_after:
	/* nothing */	{ $$ = NULL; }
    |	t_AFTER expr	{ $$ = $2; overload_resolution($$, std->predef_TIME); }
    ;

target:
	name	    { $$ = build_Expr($1); }
    |	aggregate   { $$ = $1; }
    ;

opts:
        opts_1 opts_2
    ;

opts_2:
	/* nothing */
    |	t_TRANSPORT	{ info("XXX - no transport"); }
    ;

opts_1:
	/* nothing */
    |	t_GUARDED	{ info("XXX - not guarded"); }
    ;

sel_signal_assign:
	t_WITH expr t_SELECT target t_LESym opts sel_wavefrms t_Semicolon
    ;

sel_wavefrms:
	sel_wavefrms_1 wavefrm t_WHEN choices
    ;

sel_wavefrms_1:
	/* nothing */
    |	sel_wavefrms_1 wavefrm t_WHEN choices t_Comma
    ;

generate_stat:
	t_Identifier t_Colon generation_scheme t_GENERATE
	  concurrent_stats
	t_END t_GENERATE generate_stat_1 t_Semicolon { }
    ;

generate_stat_1:
	/* nothing */
    |	t_Identifier { }
    ;

proc_stat:
	opt_proc_label t_PROCESS opt_proc_sensitivity_list opt_t_IS
	{
	    nProcess p = mProcess(@2, $1, $3, NULL);
	    add_decl(p);
	    push_scope(p);
	    $<tree.Process>$ = p;
	}
	    proc_decl_part
	t_BEGIN
	    seq_stats
	t_END t_PROCESS opt_t_Identifier t_Semicolon
	{
	    $<tree.Process>5->stats = $8;
	    if($11 && !vaul_name_eq($11, $1)) {
		if($1)
		    error("'%s' does not match process label '%s'", $11, $1);
		else
		    error("process has no label");
	    }
	    $$ = $<tree.Process>5;
	    pop_scope($$);
	}
    ;

opt_proc_label:
	/* nothing */		{ $$ = NULL; }
    |	t_Identifier t_Colon	{ $$ = $1; }
    ;

opt_t_IS:
	/* nothing */
    |	t_IS
    ;

proc_decl_part:
	/* nothing */
    |	proc_decl_part proc_decltve_item
    ;

opt_proc_sensitivity_list:
	/* nothing */				    { $$ = NULL; }
    |	t_LeftParen sensitivity_list t_RightParen   { $$ = $2; }
    ; 

sensitivity_list:
	signal_name reverse_opt_more_sensitivities
	{
	    $1->next = reverse($2);
	    $$ = $1;
	}
    ;

reverse_opt_more_sensitivities:	 
	/* nothing */	{ $$ = NULL; }
    |	reverse_opt_more_sensitivities t_Comma signal_name
	{
	    $3->next = $1;
	    $$ = $3;
	}	
    ;

signal_name:
	name
	{
	    nExpr e = build_Expr($1);
	    overload_resolution(e, nkType);
	    if(e && (!e->is(nkObjectRef) || !nObjectRef(e)->is_signal())) {
		error("%:%n is not a signal", $1, $1);
		e = NULL;
	    }
	    $$ = mSignalList(nObjectRef(e), NULL);
	}
    ;

/*--------------------------------------------------
--  Sequential Statements
----------------------------------------------------*/

seq_stats:
	rev_seq_stats	{ $$ = reverse($1); }
    ;

rev_seq_stats:
	/* nothing */		{ $$ = NULL; }
    |	rev_seq_stats seq_stat	
	{
	    if($2) {
		$2->next = $1;
		$$ = $2;
	    } else
		$$ = $1;
	}
    ;

seq_stat:
	assertion_stat        { $$ = $1; }
    |	case_stat             { $$ = $1; }
    |	exit_stat             { $$ = $1; }
    |	if_stat               { $$ = $1; }
    |	loop_stat             { $$ = $1; }
    |	next_stat             { $$ = $1; }
    |	null_stat             { $$ = $1; }
    |	procedure_call_stat   { $$ = $1; }
    |	return_stat           { $$ = $1; }
    |	signal_assign_stat    { $$ = $1; }
    |	variable_assign_stat  { $$ = $1; }
    |	wait_stat             { $$ = $1; }
    ;

assertion_stat:
	t_ASSERT condition opt_assertion_report opt_assertion_severity
	 t_Semicolon
	{
	    $$ = mAssertStat(@1, $2, $3, $4);
	}
    ;

opt_assertion_severity:
	/* nothing */
	{
	    $$ = NULL;
	}
    |	t_SEVERITY expr
	{
	    $$ = $2;
	    overload_resolution($$, std->predef_SEVERITY_LEVEL);
	}
    ;

opt_assertion_report:
	/* nothing */
	{
	    $$ = NULL;
	}
    |	t_REPORT expr
	{
	    $$ = $2;
	    overload_resolution($$, std->predef_STRING);
	}
    ;

case_stat:
	t_CASE expr t_IS 
	    case_stat_alternative
	    more_case_stat_alternatives 
	t_END t_CASE t_Semicolon
	{
	    nCaseAlternative ca = reverse($5);
	    if($4) {
		$4->next = ca;
		ca = $4;
	    }
	    $$ = build_CaseStat(@1, $2, ca);
	}
    ;

case_stat_alternative:
	t_WHEN choices t_Arrow seq_stats
	{
	    $$ = mCaseAlternative(@1, NULL, $2, $4);
	}
    ;

more_case_stat_alternatives:
	/* nothing */
	{
	    $$ = NULL;
	}
    |	more_case_stat_alternatives case_stat_alternative
	{
	    if($2) {
		$2->next = $1;
		$$ = $2;
	    } else
		$$ = $1;
	}
    ;

if_stat:
	t_IF condition t_THEN seq_stats if_stat_1 if_stat_2 t_END t_IF
	t_Semicolon
	{
	    $$ = mIfStat(@1, $2, $4, $5);
	    if($6) {
		nStatement *s;
		for(s = &(nIfStat($$)->else_stats); *s && (*s)->is(nkIfStat);
		    s = &(nIfStat(*s)->else_stats))
		    ;
		assert(*s == NULL);
		*s = $6;
	    }
	}
    ;

if_stat_2:
	/* nothing */	    { $$ = NULL; }
    |	t_ELSE seq_stats    { $$ = $2; }
    ;

if_stat_1:
	/* nothing */	    { $$ = NULL; }
    |	if_stat_1  t_ELSIF condition t_THEN seq_stats
	{
	    $$ = mIfStat(@2, $3, $5, NULL);
	    if($1) {
		nStatement *s;
		for(s = &(nIfStat($1)->else_stats); *s && (*s)->is(nkIfStat);
		    s = &(nIfStat(*s)->else_stats))
		    ;
		assert(*s == NULL);
		*s = $$;
		$$ = $1;
	    }
	}   
    ;

condition:
	expr { overload_resolution($1, std->predef_BOOLEAN); $$ = $1; }
    ;

loop_stat:
	opt_label opt_iteration_scheme t_LOOP
	{ 
	    nLoopStat l = push_loop(@3, $1, $2);
	}
	    seq_stats
	t_END t_LOOP opt_t_Identifier t_Semicolon
	{
	    $$ = pop_loop($5, $8);
	}
    ;

opt_iteration_scheme:
	/* nothing */	    { $$ = NULL; }
    |	iteration_scheme    { $$ = $1; }
    ;

opt_label:
	/* nothing */	     { $$ = NULL; }
    |	t_Identifier t_Colon { $$ = nLabel(add_decl(mLabel(@1, $1, NULL))); }
    ;

next_stat:
	t_NEXT opt_t_Identifier opt_when t_Semicolon
	{   
	    $$ = build_LoopControlStat(@1, nkNextStat, $2, $3);
	}
    ;

exit_stat:
	t_EXIT opt_t_Identifier opt_when t_Semicolon
	{   
	    $$ = build_LoopControlStat(@1, nkExitStat, $2, $3);
	}
    ;

opt_when:
	/* nothing */	    { $$ = NULL; }
    |	t_WHEN condition    { $$ = $2; }
    ;

null_stat:
	t_NULL t_Semicolon  { $$ = mNullStat(@1); }
    ;

procedure_call_stat:
	name t_Semicolon
	{
	    nExpr e = build_Expr($1);
	    overload_resolution(e, NULL, nkType, true);
	    if(e && e->is(nkProcedureCall))
		$$ = mProcedureCallStat(@1, nProcedureCall(e)->proc,
					nProcedureCall(e)->first_actual);
	    else {
		if(e)
		    error("%:%n is not a procedure call", $1, $1);
		$$ = NULL;
	    }
	}
    ;

return_stat:
	t_RETURN opt_expr t_Semicolon
	{
	    if(cur_body == NULL || cur_body->continued == NULL || $2 == NULL)
		$$ = NULL;
	    else if(cur_body->continued->is(nkFunction)) {
		if($2->is(nkOpenExpr)) {
		    error("return without a value");
		    $$ = NULL;
		} else {
		    overload_resolution($2,
		     nFunction(cur_body->continued)->return_type);
		    $$ = mReturnStat(@1, $2);
		}
	    } else if(cur_body->continued->is(nkProcedure)) {
		if(!$2->is(nkOpenExpr)) {
		    error("return with a value");
		    $$ = NULL;
		} else
		    $$ = mReturnStat(@1, NULL);
	    } else {
		error("can only return from subprograms");
		$$ = NULL;
	    }
	}
    ;

opt_expr:
	/* nothing */	{ $$ = mOpenExpr(); }
    |	expr
    ;

signal_assign_stat:
	target t_LESym opt_t_TRANSPORT wavefrm t_Semicolon
	{
	    $$ = build_SignalAssignment(@2, $1, $3, $4);
	}
    ;

opt_t_TRANSPORT:
	/* nothing */	{ $$ = false; }
    |	t_TRANSPORT	{ $$ = true; }
    ;

variable_assign_stat:
	target t_VarAsgn expr t_Semicolon
	{
	    $$ = build_VarAssignment(@2, $1, $3);
	}
    ;

wait_stat:
	t_WAIT opt_wait_on opt_wait_until opt_wait_for t_Semicolon
	{
	    $$ = mWaitStat(@1, $2, $3, $4);
	}
    ;

opt_wait_for:
	/* nothing */	{ $$ = NULL; }
    |	t_FOR expr	{ $$ = $2; overload_resolution($$, std->predef_TIME); }
    ;

opt_wait_until:
	/* nothing */	    { $$ = NULL; }
    |	t_UNTIL condition   { $$ = $2; }
    ;

opt_wait_on:
	/* nothing */		{ $$ = NULL; }
    |	t_ON sensitivity_list	{ $$ = $2; }
    ;

/*--------------------------------------------------
--  Components and Configurations
----------------------------------------------------*/

comp_decl:
	t_COMPONENT t_Identifier opt_t_IS
	{
	    nComponent c = mComponent(@1, $2);
	    add_decl(c);
	    push_scope(c);
	    $<tree.Component>$ = c;
	}
	    opt_generic_clause
	    opt_port_clause
	t_END t_COMPONENT opt_t_Identifier t_Semicolon
	{
	    if($9 && !vaul_name_eq($9, $2))
		error("'%s' does not match component name '%s'", $9, $2);
	    pop_scope($<tree.Component>4);
	}
    ;

block_config:
	t_FOR name { $<tree.BlockConfig>$ = start_BlockConfig($2); }
    	    use_clauses
    	    config_items
    	t_END t_FOR t_Semicolon
    	{
    	    $$ = $<tree.BlockConfig>3;
    	    if($$) {
    	    	check_BlockConfig($$);
    		pop_scope($$);
    	    }
    	}
    ;

config_items:
 	/* nothing */		    { }
    |	config_items config_item    { }
    ;

use_clauses:
	/* nothing */
    |	use_clauses use_clause { use($2); }
    ;

config_item:
	block_config  { $$ = $1; }
    |	comp_config   { $$ = $1; }
    ;

comp_config:
	t_FOR comp_spec opt_comp_binding_indic
    	{ $<tree.CompConfig>$ = start_CompConfig(@1, $2, $3); }
    	    opt_block_config
	t_END t_FOR t_Semicolon
    	{
    	    nCompConfig cc = $<tree.CompConfig>4;
    	    if(cc) {
       	    	cc->config = $5;
    		pop_scope(cc);
    	    }
    	}
    ;

opt_block_config:
	/* nothing */	{ $$ = NULL; }
    |	block_config	{ $$ = $1; }
    ;

opt_comp_binding_indic:
    	/* nothing */	{ $$ = NULL; }
    |  	incremental_binding_indic t_Semicolon { $$ = $1; }
    |	pre_binding_indic t_Semicolon { $$ = $1; }
    ;

config_spec:
	t_FOR comp_spec binding_indic t_Semicolon
	{
	    nConfigSpec cs = mConfigSpec(@1, $2, $3);
	    if(cur_scope->is(nkBlock))
		nBlock(cur_scope)->add_spec(cs);
	}
    ;

comp_spec:
	inst_list t_Colon comp_mark
	{
	    $$ = mComponentSpec($1, $3);
	    select_scope($3);
	}
    ;

comp_mark:
	mark
	 { $$ = nComponent(find_single_decl($1, nkComponent, "component")); }
    ;

inst_list:
	idf_list    { $$ = mInstList_Ids(@1, $1); }
    |	t_ALL	    { $$ = mInstList_ALL(@1); }
    |	t_OTHERS    { $$ = mInstList_OTHERS(@1); }
    ;

/* binding_indic et al is to be invoked with the component decl
** as the selected scope
*/

binding_indic:
    	pre_binding_indic { $$ = build_BindingIndic($1); }
    ;

pre_binding_indic:
	 { $<tree.Scope>$ = selected_scope; unselect_scope(); }
	t_USE entity_aspect
	 { select_scope($<tree.Scope>1); }
	opt_generic_map opt_port_map
	{
	    $$ = mIncrementalBindingIndic($3, $5, $6);
	    unselect_scope();
	}
    ;

incremental_binding_indic:
	generic_map opt_port_map
	{
	    $$ = mIncrementalBindingIndic(NULL, $1, $2);
	    unselect_scope();
	}
    |	port_map
    	{
    	    $$ = mIncrementalBindingIndic(NULL, NULL, $1);
    	    unselect_scope();
    	}
    ;

entity_aspect:
	t_ENTITY simple_sel_name opt_arch_id
	 { $$ = get_architecture($2, $3); }
    |	t_CONFIGURATION mark
	 { $$ = get_configuration($2); }
    |	t_OPEN
	 { $$ = NULL; }
    ;

opt_arch_id:
	/* nothing */				{ $$ = NULL; }
    |	t_LeftParen t_Identifier t_RightParen	{ $$ = $2; }
    ;

%%

vaul_error_printer::vaul_error_printer()
{
    log = stderr;
    n_errors = 0;
}

vaul_parser::vaul_parser(vaul_lexer *l)
{
    lex = l;
    lex->set_printer(this, log);
    cur_du = NULL;
    eof = false;
    verbose = false;
}

vaul_parser::~vaul_parser()
{
}

vaul_design_unit *vaul_parser::parse(vaul_libpool *p)
{
    libs = p;
    if(libs->get_worklib() == NULL) {
	set_error(vhdlerr_usage, "no active session");
	return NULL;
    }

    XXX_seen = false;
    init();
    bison_parse();
    if(XXX_seen)
	n_errors++;

    vaul_design_unit *du = finish();
    if(du) {
	lex->pool.store(du->mem);
	du->set_ctime();
    }

    if(n_errors > 0)
	set_error(vhdlerr_errors_detected, "%d errors", n_errors);

    return du;
}

void vaul_parser::clear_errors()
{
    n_errors = 0;
}

