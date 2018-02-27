/* syntactical analysis of VHDL

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

#ifndef VAUL_PARSER_H
#define VAUL_PARSER_H

#include <vaul/errors.h>
#include <vaul/lexer.h>
#include <vaul/printer.h>
#include <vaul/my_dynarray.h>

struct vaul_design_unit;
struct vaul_libpool;

struct vaul_error_printer : vaul_node_printer {

    vaul_error_printer();

    void info(char *fmt, ...);
    void error(char *fmt, ...);
	// %n:	nNode
	// %C:	vaul_lexer *, lexer Kontext
	// %::	nPosNode, Node Position
	// %*:	vaul_lexer *, lexer Position
	//	rest wie printf

    virtual void vinfo(char *fmt, va_list ap);
    int n_errors;
    FILE *log;
};

struct vaul_decl_set {

    vaul_decl_set(vaul_error_printer * = NULL);
    ~vaul_decl_set();

    void reset();

    nDeclaration single_decl(bool print = true);
    bool multi_decls(bool print = true);
    bool found_none();

    void show(bool only_valids=false);

    nName name;

    void add(nDeclaration);
    void begin_indirects();
    void end_indirects();
    bool finish_scope(nScope);

    void refresh();
    void invalidate_pot_invalids();

    void iterate(void (*f)(nDeclaration, void *cl), void *cl);
    void filter(int (*f)(nDeclaration, void *cl), void *cl);
    int retain_lowcost();

private:
    vaul_error_printer *pr;
    enum state_code { invalid, pot_invalid, pot_valid, valid };
    struct item {
	nDeclaration d;
	state_code state;
	int cost;
    } *decls;
    int n_decls;
    bool doing_indirects;
    bool not_overloadable;
};

typedef my_dynarray<nType> nType_vector;

//  vaul_parser enthält den von Bison erzeugten Parser
//
struct vaul_parser : vaul_error_source, vaul_error_printer {

    vaul_parser(vaul_lexer *l);
	// Erzeugt einen Parser, der aus 'l' liest.

    ~vaul_parser();

    vaul_design_unit *parse(vaul_libpool *p);
	// Versucht, eine komplette Design Unit zu lesen
	// und konstruiert den abstrakten Baum.

    void clear_errors();
	// Vergisst alle im VHDL-Text gefundenen Fehler

    bool was_eof()	    { return eof; }
	// Ermittelt, ob das Ende des VHDL-Textes schon erreicht wurde.

    bool verbose;

private:
    vaul_lexer *lex;
    int bison_parse();

    void BisonError(char *msg);

    char *get_source(nNode);

    void vinfo(char *fmt, va_list ap);
    void format_out(FILE *, const char *spec, int len, va_list &ap);
    bool XXX_seen;
    nScope announced_scope;
    
    bool eof;

    vaul_libpool *libs;
    vaul_mempool pool;
    vaul_design_unit *cur_du;
    nSubprogramBody cur_body;

    void init();
    void start(nScope u);
    vaul_design_unit *finish();

    // decls.cc

    void add_libs(nIdList);
    void use(nSelNameList);

    void visit_decls(void f(nDeclaration, void *), void *);

    nEntity get_entity(char *n);
    nPackage get_package(char *n);
    nArchitecture get_architecture(nName, Id);
    nConfiguration get_configuration(nName);

    nScope cur_scope, selected_scope;
    void push_scope(nScope), pop_scope(nScope);
    nDeclaration add_decl(nScope, nDeclaration, int lineno = 0, Id = NULL);
    nDeclaration add_decl(nDeclaration d)
     { return add_decl(cur_scope, d, 0, NULL); }
    void rem_decl(nScope, nDeclaration);

    void find_decls(vaul_decl_set &, nName);
    nDeclaration find_single_decl(nName, nKind exp_k, char *kind_name = NULL);
    nDeclaration find_single_decl(Id, nKind exp_k, char *kind_name = NULL);

    void find_decls(vaul_decl_set &, Id, nScope, bool by_selection);
    void find_decls(vaul_decl_set &, nName, nScope, bool by_selection);

    void start_decl(Id id);

    void select_scope(nScope);
    void unselect_scope();
    
    nAlias add_Alias(int lno, Id id, nType alias_type, nName aliased_thing);
    nFile  add_File(int lno, Id id, nType file_type, nExpr mode, nExpr name);

    nStandardPackage std;
    
    // types.cc

    nType build_SubType(nName resol, nName mark, nConstraint);
    nSubType build_SubType_def(int lno, nRange, nType base = NULL);

    nType get_type(nName);
    void add_PredefOp(int lineno, nType ret, Id, nType l, nType r = NULL);
    void add_predefined_ops(nType);

    nPreIndexConstraint build_PreIndexConstraint(nGenAssocElem);
    nIndexConstraint build_IndexConstraint(nPreIndexConstraint, nType base);
    nType find_index_range_type(nExplicitRange);

    nType adapt_object_type(ObjectClass, nType, nExpr initial_value);
    nSubType build_constrained_array_type(nPreIndexConstraint, nType elt);

    // expr.cc

    bool try_overload_resolution(nExpr, nType, nKind = nkInvalid);
    void overload_resolution(nExpr &, nType, nKind, bool procs_allowed);
    void overload_resolution(nExpr &e, nType t)
     { overload_resolution(e, t, nkInvalid, false); }
    void overload_resolution(nExpr &e, nKind k)
     { overload_resolution(e, NULL, k, false); }

    bool prepare_named_assocs(nGenAssocElem);
    bool validate_gen_assocs(nGenAssocElem);

    nAssociation associate(nNamedAssocElem, nInterface, bool complete = true);

    nExpr build_Expr(nName);
    nExpr build_Expr(nName, vaul_decl_set *set, nKind basic_k);

    nNode build_Expr_or_Attr(nName);
    nNode build_Expr_or_Attr(nName, vaul_decl_set *set, nKind basic_k);
    nExpr validate_Expr(nNode expr_or_attr);

    nArrayObjectRef build_ArrayObjectRef(nExpr prefix, nGenAssocElem indices);
    nSliceObjectRef build_SliceObjectRef(nExpr prefix, nGenAssocElem slice);
    nPhysicalLiteralRef build_PhysicalLiteralRef(int lineno, Literal, Id unit);
    nAggregate build_Aggregate(nAmbgAggregate, nType);

    nExpr build_bcall(int lineno, nExpr, Id id, nExpr);
    nExpr build_QualifiedExpr(nName mark, nExpr);
    nExpr build_TypeConversion(int lineno, nType, nExpr);

    nType expr_type(nExpr);

    nType_vector *ambg_expr_types(nExpr);

    // attr.cc

    nExpr build_AttrExpr(nAttributeName, vaul_decl_set *set, nKind basic_k);
    
    nNode build_AttrNode(nName, vaul_decl_set *set, nKind basic_k);
    nNode build_AttrNode(nAttributeName, vaul_decl_set *set, nKind basic_k);

    void bind_attrspec(nAttributeSpec);

    // subprogs.cc

    void validate_interface(nSubprogram, nInterface);
    void validate_port(nInterface);
    void validate_generic(nInterface);

    // stats.cc

    nLoopStat push_loop(int lineno, nLabel, nIterationScheme);
    nLoopStat pop_loop(nStatement loop_stats, Id loop_id);

    nProcess build_condal_Process(Id label, nCondalSignalAssign);
    nLoopControlStat build_LoopControlStat(int lineno, nKind stat,
					   Id loop_label, nExpr when);
    nVarAssignment build_VarAssignment(int lineno, nExpr target, nExpr value);
    nSignalAssignment build_SignalAssignment(int lineno,
					     nExpr target,
					     bool transport,
					     nWaveformElement wave);
    nCaseStat build_CaseStat(int lineno, nExpr switch_expr,
			     nCaseAlternative alternatives);
    nProcess build_conc_AssertStat(int lineno, Id label, nAssertStat);
    nProcess build_conc_ProcedureCall(int lineno, Id label,
				      nProcedureCallStat);
    void get_implicit_signals(nSignalList &sigs, nExpr e);
    void add_to_signal_list(nSignalList &sigs, nObjectRef sig);

    // blocks.cc

    nBindingIndic build_BindingIndic(nIncrementalBindingIndic);
    nBindingIndic build_BindingIndic(nComponent unit, nNamedAssocElem gmap,
				     nNamedAssocElem pmap);

    void bind_specs(nBlock);

    nBlockConfig start_BlockConfig(nName);
    nCompConfig start_CompConfig(int lineno, nComponentSpec,
				 nIncrementalBindingIndic);
    void check_BlockConfig(nBlockConfig);

    nArchitecture get_architecture(nEntity, nSimpleName);

    nCompInstList list_comps(nComponentSpec, nScope);

    // super private:
    bool associate_one(nAssociation &tail,
		       nInterface, nDeclaration,
		       nExpr, nDeclaration);
    int constrain(nExpr, nType, nKind, bool pre = false);
    int try_one_association(nNamedAssocElem, nInterface);
    int try_association(nNamedAssocElem, nInterface);
    static int filter_return_stub(nDeclaration, void *);
    struct filter_return_closure;
    int filter_return(nDeclaration, filter_return_closure *);
    int conversion_cost(nNode, nType, nKind);
    int try_array_subscription(nArrayType, nGenAssocElem);
    nType is_one_dim_array(nType);
    bool is_one_dim_logical_array(nType);
    bool is_one_dim_discrete_array(nType);
    bool is_discrete_type(nType);
    int choice_conversion_cost(nChoice, nExpr actual, nType, nKind);
    bool check_target(nExpr target, ObjectClass oc, const char *oc_label);
    nDeclaration grab_formal_conversion(nNamedAssocElem assoc,
					nInterface formals,
					int *formal_cost = NULL,
					nInterface *converted_formal = NULL);
    nExpr add_partial_choice(nExpr &pactual, nName formal, nExpr actual);
    nExpr disambiguate_expr(nExpr, nType, bool procs, bool valid);
    bool evaluate_locally_static_universal_integer(nExpr, int &);
    nType find_array_attr_index_type(nArrayType, nExpr, int &);
    nExpr make_appropriate(nExpr);
    void report_mismatched_subprog(nName, vaul_decl_set *, nNamedAssocElem);
};

#endif // VAUL_PARSER_H
