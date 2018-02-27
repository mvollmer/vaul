/************** Syntax for VHDL in YACC format ******************
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
*******************************************************/


%{
#include <stdio.h>


%}
%union{
  struct{
    int pos;
    int len;
    int line;
    char *name;
  } ds_Id;
}

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

/* VHDL binary operators
  ======================= */

%nonassoc t_EQSym t_NESym t_LTSym t_LESym t_GTSym t_GESym
%left t_Plus t_Minus t_Ampersand
%left MED_PRECEDENCE
%left t_Star t_Slash t_MOD t_REM
%nonassoc t_DoubleStar t_ABS t_NOT MAX_PRECEDENCE

/* misc syms */
/*t_Space */
%token t_Apostrophe
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

%token  < ds_Id > t_Identifier
t_AbstractLit
t_CharacterLit

t_StringLit
t_BitStringLit


%%
start: design_file
;design_file     : design_unit_list
;design_unit_list: design_unit
 | design_unit_list design_unit  

;designator : t_Identifier	
 | t_StringLit  

;literal    : t_AbstractLit  
 | t_CharacterLit  
 | t_BitStringLit
 | physical_literal_no_default  
 | t_NULL  


;enumeration_literal  : t_CharacterLit
 | t_Identifier  

;physical_literal     : physical_literal_1 t_Identifier  
;physical_literal_1   :  
 | t_AbstractLit  
;physical_literal_no_default    : t_AbstractLit t_Identifier  

;idf_list : t_Identifier 
 | idf_list t_Comma t_Identifier 


/*------------------------------------------
--  Desing Unit
--------------------------------------------*/

;design_unit     : context_list lib_unit  
;context_list    :  
 | context_list context_item  

;lib_unit    : entity_decl  
 | config_decl  
 | package_decl  
 | arch_body  
 | package_body  

;context_item    : lib_clause  
 | use_clause  

;lib_clause  : t_LIBRARY idf_list t_Semicolon  

;use_clause : t_USE sel_list t_Semicolon  

;sel_list : sel_name		
 | sel_list t_Comma sel_name	

/*------------------------------------------
--  Library Units
--------------------------------------------*/

;entity_decl : entity_start entity_decl_1 entity_decl_2
entity_decl_3 entity_decl_4 t_END entity_decl_5 t_Semicolon 
;entity_start: t_ENTITY t_Identifier t_IS	
;entity_decl_5 :  
 | t_Identifier	
;entity_decl_4 :  
 | t_BEGIN concurrent_stats  
;entity_decl_3 :  
 | entity_decl_3 entity_decl_6  
;entity_decl_6 : entity_decltve_item  
;entity_decl_2 :
 | t_PORT interf_list t_Semicolon  
;entity_decl_1 :  
 | t_GENERIC interf_list t_Semicolon  

;arch_body : arch_start arch_body_1 t_BEGIN concurrent_stats t_END
arch_body_2 t_Semicolon 
;arch_start : t_ARCHITECTURE t_Identifier t_OF t_Identifier t_IS 
;arch_body_2  :  
;arch_body_2  : t_Identifier  
;arch_body_1  :  
;arch_body_1  : arch_body_1 arch_body_3  
;arch_body_3  : block_decltve_item  

;config_decl : config_start config_decl_1 block_config t_END config_decl_2 t_Semicolon
 /* ;config_start : t_CONFIGURATION t_Identifier t_OF mark t_IS 
    replaced: */
;config_start : t_CONFIGURATION t_Identifier t_OF t_Identifier t_IS 
;config_decl_2    :  
;config_decl_2    : t_Identifier  
;config_decl_1    :  
;config_decl_1    : config_decl_1 config_decl_3  
;config_decl_3    : config_decltve_item  

;package_decl  : package_start package_decl_1 t_END package_decl_2 t_Semicolon  
;package_start : t_PACKAGE t_Identifier t_IS 
;package_decl_2     :  
;package_decl_2     : t_Identifier  
;package_decl_1     :  
;package_decl_1     : package_decl_1 package_decl_3  
;package_decl_3     : package_decltve_item  

;package_body    : pack_body_start package_body_1 t_END package_body_2 t_Semicolon  
;pack_body_start : t_PACKAGE t_BODY t_Identifier t_IS 
;package_body_2  :  
;package_body_2  : t_Identifier  
;package_body_1  :  
;package_body_1  : package_body_1 package_body_3  
;package_body_3  : package_body_decltve_item  

/*------------------------------------------
--  Declarative Item
--------------------------------------------*/
;common_decltve_item   : type_decl  
;common_decltve_item   : subtype_decl  
;common_decltve_item   : constant_decl  
;common_decltve_item   : file_decl  
;common_decltve_item   : alias_decl  
;common_decltve_item   : subprog_decl  
;common_decltve_item   : use_clause  

;entity_decltve_item   : common_decltve_item  
;entity_decltve_item   : subprog_body  
;entity_decltve_item   : attribute_decl  
;entity_decltve_item   : attribute_spec  
;entity_decltve_item   : disconnection_spec  
;entity_decltve_item   : signal_decl  

;block_decltve_item    : common_decltve_item  
;block_decltve_item    : subprog_body  
;block_decltve_item    : comp_decl  
;block_decltve_item    : attribute_decl  
;block_decltve_item    : attribute_spec  
;block_decltve_item    : config_spec  
;block_decltve_item    : disconnection_spec  
;block_decltve_item    : signal_decl  

;package_decltve_item  : common_decltve_item  
;package_decltve_item  : comp_decl  
;package_decltve_item  : attribute_decl  
;package_decltve_item  : attribute_spec  
;package_decltve_item  : disconnection_spec  
;package_decltve_item  : signal_decl  

;package_body_decltve_item  : common_decltve_item  
;package_body_decltve_item  : subprog_body  

;subprog_decltve_item    : common_decltve_item  
;subprog_decltve_item    : subprog_body  
;subprog_decltve_item    : attribute_decl  
;subprog_decltve_item    : attribute_spec  
;subprog_decltve_item    : variable_decl  

;procs_decltve_item  : common_decltve_item  
;procs_decltve_item  : subprog_body  
;procs_decltve_item  : attribute_decl  
;procs_decltve_item  : attribute_spec  
;procs_decltve_item  : variable_decl  

;config_decltve_item : attribute_spec  
;config_decltve_item : use_clause  
/*------------------------------------------
--  Subprograms
--------------------------------------------*/

;subprog_decl    : subprog_spec t_Semicolon  

;subprog_spec  : t_PROCEDURE t_Identifier subprog_spec_1  
;subprog_spec  : t_FUNCTION designator subprog_spec_2 t_RETURN mark  
;subprog_spec_2     :  
;subprog_spec_2     : interf_list  
;subprog_spec_1     :  
;subprog_spec_1     : interf_list  

;subprog_body : subprog_spec t_IS subprog_body_1 t_BEGIN seq_stats t_END subprog_body_2 t_Semicolon  
;subprog_body_2    :  
;subprog_body_2    : designator  
;subprog_body_1    :  
;subprog_body_1    : subprog_body_1 subprog_body_3  
;subprog_body_3    : subprog_decltve_item  

/*--------------------------------------------------
--  Interface Lists and Associaton Lists
----------------------------------------------------*/

;interf_list  : t_LeftParen interf_element interf_list_1 t_RightParen  
;interf_list_1     :  
;interf_list_1     : interf_list_1 interf_list_2  
;interf_list_2     : t_Semicolon interf_element  

;interf_element    : interf_element_1 idf_list t_Colon interf_element_2 subtype_indic interf_element_3 interf_element_4  
;interf_element_4  :  
;interf_element_4  : t_VarAsgn expr  
;interf_element_3  :  
;interf_element_3  : t_BUFFER
;interf_element_2  :  
;interf_element_2  : mode  
;interf_element_1  :  
;interf_element_1  : object_class  

;mode  : t_IN  
;mode  : t_OUT 
;mode  : t_INOUT  
;mode  : t_BUFFER  
;mode  : t_LINKAGE  

;association_list     : t_LeftParen association_element association_list_1 t_RightParen  
;association_list_1   :  
;association_list_1   : association_list_1 association_list_2  
;association_list_2   : t_Comma association_element  


;gen_association_list : t_LeftParen gen_association_element gen_association_list_1 t_RightParen  
;gen_association_list_1    :  
;gen_association_list_1    : gen_association_list_1 gen_association_list_2  
;gen_association_list_2    : t_Comma gen_association_element  

;association_element  : formal_part t_Arrow actual_part  
;association_element  : actual_part  

/* changed ;gen_association_element   : association_element  */
;gen_association_element   : expr
;gen_association_element   : discrete_range1  

;formal_part     : name  

;actual_part     : expr  
;actual_part     : t_OPEN  

/*--------------------------------------------------
--  Names and Expressions
----------------------------------------------------*/

;mark  : t_Identifier	
;mark  : sel_name  

;expr : and_relation  
;expr : or_relation  
;expr : xor_relation  
;expr : relation  
;expr : relation t_NAND relation  
;expr : relation t_NOR relation  

;and_relation    : relation t_AND relation  
;and_relation    : and_relation t_AND relation  

;or_relation     : relation t_OR relation  
;or_relation     : or_relation t_OR relation  

;xor_relation    : relation t_XOR relation  
;xor_relation    : xor_relation t_XOR relation  

/* ;relation   : unary_operator primary   */
;relation   : primary  
;relation   : t_Plus primary  %prec MED_PRECEDENCE
;relation   : t_Minus primary %prec MED_PRECEDENCE
;relation   : t_ABS primary   
;relation   : t_NOT primary   
;relation   : primary t_DoubleStar primary
/* ;    relation : relation binary_operator primary */
;relation   : relation t_MOD relation
;relation   : relation t_REM relation
;relation   : relation t_Ampersand relation
;relation   : relation t_Star relation
;relation   : relation t_Plus relation
;relation   : relation t_Minus relation
;relation   : relation t_LESym relation
;relation   : relation t_GESym relation
;relation   : relation t_LTSym relation
;relation   : relation t_GTSym relation
;relation   : relation t_EQSym relation
;relation   : relation t_NESym relation
;relation   : relation t_Slash relation


;primary    : name  
;primary    : literal  
;primary    : aggregate  
;primary    : qualified_expr  
;primary    : allocator  
;primary    : t_LeftParen expr t_RightParen  

;name  : mark  
;name  : name2  

;name2 : t_StringLit
;name2 : attribute_name  
;name2 : ifts_name  

;sel_name   : name t_Dot suffix  

;suffix     : designator  
;suffix     : t_CharacterLit
;suffix     : t_ALL  

;ifts_name  : mark gen_association_list  
;ifts_name  : name2 gen_association_list  

;attribute_name  : mark t_Apostrophe t_Identifier  
;attribute_name  : name2 t_Apostrophe t_Identifier  
;attribute_name : mark t_Apostrophe t_RANGE
;attribute_name : name2 t_Apostrophe t_RANGE

;aggregate  : element_association_list2 t_RightParen  
;aggregate  : t_LeftParen choices t_Arrow expr t_RightParen  

;element_association_list2 : t_LeftParen element_association t_Comma element_association  
;element_association_list2 : element_association_list2 t_Comma element_association  

;qualified_expr : mark t_Apostrophe t_LeftParen expr t_RightParen  
;qualified_expr : mark t_Apostrophe aggregate  


;allocator  : t_NEW mark mark allocator_1  
;allocator  : t_NEW mark allocator_2  
;allocator  : t_NEW qualified_expr  
;allocator_2     :  
;allocator_2     : gen_association_list  
;allocator_1     :  
;allocator_1     : gen_association_list  


/*--------------------------------------------------
--  Element Association and Choices
----------------------------------------------------*/

;element_association  : choices t_Arrow expr  
;element_association  : expr  

;choices    : choice choices_1  
;choices_1  :  
;choices_1  : choices_1 choices_2  
;choices_2  : t_Bar choice  

;choice     : expr  
;choice     : discrete_range1  
;choice     : t_OTHERS  

/*--------------------------------------------------
--	Type Declarations
----------------------------------------------------*/

;type_decl     : t_TYPE t_Identifier type_decl_1 t_Semicolon 
;type_decl_1   :  
;type_decl_1   : t_IS type_definition  

;type_definition : enumeration_type_definition  
;type_definition : range_constraint  
;type_definition : physical_type_definition  
;type_definition : unconstrained_array_definition  
;type_definition : constrained_array_definition  
;type_definition : record_type_definition  
;type_definition : access_type_definition  
;type_definition : file_type_definition  

;enumeration_type_definition    : t_LeftParen enumeration_literal enumeration_type_definition_1 t_RightParen  
;enumeration_type_definition_1  :  
;enumeration_type_definition_1  : enumeration_type_definition_1 enumeration_type_definition_2  
;enumeration_type_definition_2  : t_Comma enumeration_literal  

;physical_type_definition : range_constraint t_UNITS base_unit_decl
physical_type_definition_1 t_END t_UNITS 
;physical_type_definition_1     :  
;physical_type_definition_1     : physical_type_definition_1 physical_type_definition_2  
;physical_type_definition_2     : secondary_unit_decl  

;base_unit_decl     : t_Identifier t_Semicolon  

;secondary_unit_decl     : t_Identifier t_EQSym physical_literal t_Semicolon  

;unconstrained_array_definition : t_ARRAY t_LeftParen
index_subtype_definition unconstrained_array_definition_1 t_RightParen t_OF
subtype_indic 
;unconstrained_array_definition_1    :  
;unconstrained_array_definition_1    : unconstrained_array_definition_1 unconstrained_array_definition_2  
;unconstrained_array_definition_2    : t_Comma index_subtype_definition  

;index_subtype_definition  : mark t_RANGE t_Box  

;constrained_array_definition   : t_ARRAY index_constraint t_OF subtype_indic  

;record_type_definition : t_RECORD element_decl record_type_definition_1 t_END
t_RECORD 
;record_type_definition_1  :  
;record_type_definition_1  : record_type_definition_1 record_type_definition_2  
;record_type_definition_2  : element_decl  

;element_decl  : idf_list t_Colon subtype_indic t_Semicolon  

;access_type_definition    : t_ACCESS subtype_indic  

;file_type_definition : t_FILE t_OF mark  

/*--------------------------------------------------
--  Subtypes and Constraints
----------------------------------------------------*/

;subtype_decl  : t_SUBTYPE t_Identifier t_IS subtype_indic t_Semicolon  

;subtype_indic   : mark subtype_indic_1  
;subtype_indic   : subtype_indic1  
;subtype_indic_1 :  
;subtype_indic_1 : gen_association_list  

;subtype_indic1  : mark mark range_constraint  
;subtype_indic1  : mark range_constraint  
;subtype_indic1  : mark mark subtype_indic1_1  
;subtype_indic1_1     :  
;subtype_indic1_1     : gen_association_list  

;range_constraint     : t_RANGE range_spec  

;index_constraint     : t_LeftParen discrete_range index_constraint_1 t_RightParen  
;index_constraint_1   :  
;index_constraint_1   : index_constraint_1 index_constraint_2  
;index_constraint_2   : t_Comma discrete_range  

;discrete_range  : subtype_indic  
;discrete_range  : range_spec  

;discrete_range1 : subtype_indic1  
;discrete_range1 : expr direction expr  

;range_spec  : attribute_name  
;range_spec  : expr direction expr  

;direction  : t_TO
;direction  : t_DOWNTO  

/*--------------------------------------------------
--  Objects, Aliases, Files, Disconnections
----------------------------------------------------*/

;constant_decl : t_CONSTANT idf_list t_Colon subtype_indic constant_decl_1 t_Semicolon  
;constant_decl_1    :  
;constant_decl_1    : t_VarAsgn expr  

;signal_decl   : t_SIGNAL idf_list t_Colon subtype_indic signal_decl_1 signal_decl_2 t_Semicolon  
;signal_decl_2 :  
;signal_decl_2 : t_VarAsgn expr  
;signal_decl_1 :  
;signal_decl_1 : signal_kind  

;variable_decl : t_VARIABLE idf_list t_Colon subtype_indic variable_decl_1 t_Semicolon  
;variable_decl_1    :  
;variable_decl_1    : t_VarAsgn expr  

;object_class    : t_CONSTANT  
;object_class    : t_SIGNAL  
;object_class    : t_VARIABLE  

;signal_kind     : t_BUS  
;signal_kind     : t_REGISTER  

;alias_decl    : t_ALIAS t_Identifier t_Colon subtype_indic t_IS name t_Semicolon  

;file_decl     : t_FILE t_Identifier t_Colon subtype_indic t_IS file_decl_1 expr t_Semicolon  
;file_decl_1   :  
;file_decl_1   : mode  

;disconnection_spec    : t_DISCONNECT signal_list t_Colon mark t_AFTER expr t_Semicolon  

;signal_list     : name signal_list_1  
;signal_list     : t_OTHERS  
;signal_list     : t_ALL  
;signal_list_1   :  
;signal_list_1   : signal_list_1 signal_list_2  
;signal_list_2   : t_Comma name  

/*--------------------------------------------------
--  Attribute Declarations and Specifications
----------------------------------------------------*/

;attribute_decl     : t_ATTRIBUTE t_Identifier t_Colon mark t_Semicolon  

;attribute_spec   : t_ATTRIBUTE t_Identifier t_OF entity_spec t_IS expr t_Semicolon  

;entity_spec : entity_name_list t_Colon entity_class  

;entity_name_list     : designator entity_name_list_1  
;entity_name_list     : t_OTHERS  
;entity_name_list     : t_ALL  
;entity_name_list_1   :  
;entity_name_list_1   : entity_name_list_1 entity_name_list_2  
;entity_name_list_2   : t_Comma designator  

;entity_class    : t_ENTITY
;entity_class    : t_ARCHITECTURE  
;entity_class    : t_PACKAGE 
;entity_class    : t_CONFIGURATION
;entity_class    : t_COMPONENT
;entity_class    : t_LABEL  
;entity_class    : t_TYPE
;entity_class    : t_SUBTYPE  
;entity_class    : t_PROCEDURE  
;entity_class    : t_FUNCTION  
;entity_class    : t_SIGNAL
;entity_class    : t_VARIABLE
;entity_class    : t_CONSTANT  

/*--------------------------------------------------
--  Schemes
----------------------------------------------------*/

;generation_scheme    : if_scheme  
;generation_scheme    : for_scheme  

;iteration_scheme     : for_scheme  
;iteration_scheme     : while_scheme  

;if_scheme  : t_IF expr  

;for_scheme : t_FOR t_Identifier t_IN discrete_range  

;while_scheme    : t_WHILE expr  

/*--------------------------------------------------
--  Concurrent Statements
----------------------------------------------------*/

;concurrent_stats     : concurrent_stats_1  
;concurrent_stats_1   :  
;concurrent_stats_1   : concurrent_stats_1 concurrent_stats_2  
;concurrent_stats_2   : concurrent_stat  

;concurrent_stat : block_stat  
 | concurrent_assertion_stat  
 | concurrent_procedure_call  
 | concurrent_signal_assign_stat  
 | comp_inst_stat  
 | generate_stat  
 | procs_stat  

;block_stat : t_Identifier t_Colon t_BLOCK block_stat_1 block_stat_2
block_stat_3 block_stat_4 t_BEGIN concurrent_stats t_END t_BLOCK block_stat_5
t_Semicolon 
;block_stat_5    :  
;block_stat_5    : t_Identifier  
;block_stat_4    :  
;block_stat_4    : block_stat_4 block_stat_6  
;block_stat_6    : block_decltve_item  
;block_stat_3    :  
;block_stat_3    : t_PORT interf_list t_Semicolon block_stat_7  
;block_stat_7    :  
;block_stat_7    : t_PORT t_MAP association_list t_Semicolon  
;block_stat_2    :  
;block_stat_2    : t_GENERIC interf_list t_Semicolon block_stat_8  
;block_stat_8    :  
;block_stat_8    : t_GENERIC t_MAP association_list t_Semicolon  
;block_stat_1    :  
;block_stat_1    : t_LeftParen expr t_RightParen  

;comp_inst_stat   : t_Identifier t_Colon mark t_GENERIC t_MAP association_list comp_inst_stat_1 t_Semicolon  
;comp_inst_stat   : t_Identifier t_Colon mark t_PORT t_MAP association_list t_Semicolon  
;comp_inst_stat_1 :  
;comp_inst_stat_1 : t_PORT t_MAP association_list  

;concurrent_assertion_stat : t_Identifier t_Colon assertion_stat  
;concurrent_assertion_stat : assertion_stat  

;concurrent_procedure_call : t_Identifier t_Colon procedure_call_stat  
;concurrent_procedure_call : procedure_call_stat  

;concurrent_signal_assign_stat    : t_Identifier t_Colon condal_signal_assign  
;concurrent_signal_assign_stat    : condal_signal_assign  
;concurrent_signal_assign_stat    : t_Identifier t_Colon sel_signal_assign  
;concurrent_signal_assign_stat    : sel_signal_assign  

;condal_signal_assign  : target t_LESym opts condal_wavefrms t_Semicolon  

;condal_wavefrms     : condal_wavefrms_1 wavefrm  
;condal_wavefrms_1   :  
;condal_wavefrms_1   : condal_wavefrms_1 condal_wavefrms_2  
;condal_wavefrms_2   : wavefrm t_WHEN expr t_ELSE  

;wavefrm   : wavefrm_element wavefrm_1  
;wavefrm_1 :  
;wavefrm_1 : wavefrm_1 wavefrm_2  
;wavefrm_2 : t_Comma wavefrm_element  

;wavefrm_element     : expr wavefrm_element_1  
;wavefrm_element_1   :  
;wavefrm_element_1   : t_AFTER expr  

;target     : name  
;target     : aggregate  

;opts    : opts_1 opts_2  
;opts_2  :  
;opts_2  : t_TRANSPORT  
;opts_1  :  
;opts_1  : t_GUARDED  

;sel_signal_assign : t_WITH expr t_SELECT target t_LESym opts sel_wavefrms t_Semicolon 

;sel_wavefrms   : sel_wavefrms_1 wavefrm t_WHEN choices  
;sel_wavefrms_1 :  
;sel_wavefrms_1 : sel_wavefrms_1 sel_wavefrms_2  
;sel_wavefrms_2 : wavefrm t_WHEN choices t_Comma  

;generate_stat : t_Identifier t_Colon generation_scheme t_GENERATE
concurrent_stats t_END t_GENERATE generate_stat_1 t_Semicolon 
;generate_stat_1 :  
;generate_stat_1 : t_Identifier  

;procs_stat    : t_Identifier t_Colon procs_stat1  
;procs_stat    : procs_stat1  

;procs_stat1 : t_PROCESS procs_stat1_1 procs_stat1_2 t_BEGIN seq_stats t_END
t_PROCESS procs_stat1_3 t_Semicolon 
;procs_stat1_3 :  
;procs_stat1_3 : t_Identifier  
;procs_stat1_2 :  
;procs_stat1_2 : procs_stat1_2 procs_stat1_4  
;procs_stat1_4 : procs_decltve_item  
;procs_stat1_1 :  
;procs_stat1_1 : t_LeftParen sensitivity_list t_RightParen  

;sensitivity_list     : name sensitivity_list_1  
;sensitivity_list_1   :  
;sensitivity_list_1   : sensitivity_list_1 sensitivity_list_2  
;sensitivity_list_2   : t_Comma name  

/*--------------------------------------------------
--  Sequential Statements
----------------------------------------------------*/

;seq_stats     : seq_stats_1  
;seq_stats_1   :  
;seq_stats_1   : seq_stats_1 seq_stats_2  
;seq_stats_2   : seq_stat  

;seq_stat : assertion_stat  
;seq_stat : case_stat  
;seq_stat : exit_stat  
;seq_stat : if_stat  
;seq_stat : loop_stat  
;seq_stat : next_stat  
;seq_stat : null_stat  
;seq_stat : procedure_call_stat  
;seq_stat : return_stat  
;seq_stat : signal_assign_stat  
;seq_stat : variable_assign_stat  
;seq_stat : wait_stat  

;assertion_stat  : t_ASSERT expr assertion_stat_1 assertion_stat_2 t_Semicolon  
;assertion_stat_2     :  
;assertion_stat_2     : t_SEVERITY expr  
;assertion_stat_1     :  
;assertion_stat_1     : t_REPORT expr  

;case_stat  : t_CASE expr t_IS case_stat_alternative case_stat_1 t_END t_CASE t_Semicolon  
;case_stat_1     :  
;case_stat_1     : case_stat_1 case_stat_2  
;case_stat_2     : case_stat_alternative  

;case_stat_alternative     : t_WHEN choices t_Arrow seq_stats  

;exit_stat  : t_EXIT exit_stat_1 exit_stat_2 t_Semicolon  
;exit_stat_2     :  
;exit_stat_2     : t_WHEN expr  
;exit_stat_1     :  
;exit_stat_1     : t_Identifier  

;if_stat    : t_IF expr t_THEN seq_stats if_stat_1 if_stat_2 t_END t_IF t_Semicolon  
;if_stat_2  :  
;if_stat_2  : t_ELSE seq_stats  
;if_stat_1  :  
;if_stat_1  : if_stat_1 if_stat_3  
;if_stat_3  : t_ELSIF expr t_THEN seq_stats  

;loop_stat  : loop_stat_1 loop_stat_2 t_LOOP seq_stats t_END t_LOOP loop_stat_3 t_Semicolon  
;loop_stat_3     :  
;loop_stat_3     : t_Identifier  
;loop_stat_2     :  
;loop_stat_2     : iteration_scheme  
;loop_stat_1     :  
;loop_stat_1     : t_Identifier t_Colon  

;next_stat  : t_NEXT next_stat_1 next_stat_2 t_Semicolon  
;next_stat_2     :  
;next_stat_2     : t_WHEN expr  
;next_stat_1     :  
;next_stat_1     : t_Identifier  

;null_stat  : t_NULL t_Semicolon  

;procedure_call_stat  : name t_Semicolon  

;return_stat     : t_RETURN return_stat_1 t_Semicolon  
;return_stat_1   :  
;return_stat_1   : expr  

;signal_assign_stat    : target t_LESym signal_assign_stat_1 wavefrm t_Semicolon  
;signal_assign_stat_1  :  
;signal_assign_stat_1  : t_TRANSPORT  

;variable_assign_stat  : target t_VarAsgn expr t_Semicolon  

;wait_stat  : t_WAIT wait_stat_1 wait_stat_2 wait_stat_3 t_Semicolon  
;wait_stat_3     :  
;wait_stat_3     : t_FOR expr  
;wait_stat_2     :  
;wait_stat_2     : t_UNTIL expr  
;wait_stat_1     :  
;wait_stat_1     : t_ON sensitivity_list  
/*--------------------------------------------------
--  Components and Configurations
----------------------------------------------------*/

;comp_decl     : t_COMPONENT t_Identifier comp_decl_1 comp_decl_2 t_END t_COMPONENT t_Semicolon  
;comp_decl_2   :  
;comp_decl_2   : t_PORT interf_list t_Semicolon  
;comp_decl_1   :  
;comp_decl_1   : t_GENERIC interf_list t_Semicolon  

;block_config  : t_FOR block_spec block_config_1 block_config_2 t_END t_FOR t_Semicolon  
;block_config_2     :  
;block_config_2     : block_config_2 block_config_3  
;block_config_3     : config_item  
;block_config_1     :  
;block_config_1     : block_config_1 block_config_4  
;block_config_4     : use_clause  

;block_spec  : name  

;config_item   : block_config  
;config_item   : comp_config  

;comp_config   : t_FOR comp_spec comp_config_1 comp_config_2 t_END t_FOR t_Semicolon  
;comp_config_2 :  
;comp_config_2 : block_config  
;comp_config_1 :  
;comp_config_1 : t_USE binding_indic t_Semicolon  

;config_spec : t_FOR comp_spec t_USE
binding_indic t_Semicolon 

;comp_spec   : inst_list t_Colon mark  

;inst_list   : idf_list  
;inst_list   : t_ALL  
;inst_list   : t_OTHERS  

;binding_indic   : entity_aspect binding_indic_1 binding_indic_2  
;binding_indic_2 :  
;binding_indic_2 : t_PORT t_MAP association_list  
;binding_indic_1 :  
;binding_indic_1 : t_GENERIC t_MAP association_list  

;entity_aspect   : t_ENTITY name  
;entity_aspect   : t_CONFIGURATION mark  
;entity_aspect : t_OPEN 
;
%%
extern FILE* yyout;

void yyerror(str)
char* str;
{
  fprintf( yyout, "ERROR: %s\n", str );
}
/************************************************************
* RESOLVED BUGS
* J.Gaisler reported:
*       Range attributes in FOR loops are not supported 
*	Unary minus not supported in second argument to relational exp. 
* Thorsten Groetker
*	precedence and associtivity of operators
*	(formal=>actual) was permitted in gen_associatin_list
*		and thus in subtype_indic e.g. 
*************************************************************
* OTHER KNOWN PROBLEMS:
* the grammar allows expressions: 3*-5 thats pretty nice but illegal 
* in VHDL - only 3*(-5) is permitted from LRM.
*************************************************************/
