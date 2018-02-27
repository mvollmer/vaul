/* definition of the abstract graph nodes

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

#ifndef VAUL_TREE_H
#define VAUL_TREE_H

#include <stdio.h>
#include <stddef.h>
#include <vaulutil.h>

#include <vaul/errors.h>
#include <vaul/mempool.h>
#include <vaul/printer.h>

struct vaul_binfile;
struct vaul_design_unit;
struct vaul_libpool;
struct vaul_error_printer;
struct vaul_decl_set;

/* Typen für Attribute
*/
typedef char *Id;
typedef char *Literal;

#define _enum(e) typedef unsigned char e; enum

_enum(RangeDirection) {
    DirectionUp = 'u',
    DirectionDown = 'd'
};

_enum(ObjectClass) {
    ObjClass_None = '-',
    ObjClass_Signal = 's',
    ObjClass_Variable = 'v',
    ObjClass_Constant = 'c',
    ObjClass_File = 'f'
};

_enum(Mode) {
    Mode_None = '-',
    Mode_In = 'i',
    Mode_Out = 'o',
    Mode_InOut = 'x',
    Mode_Buffer = 'b',
    Mode_Linkage = 'l'
};

_enum(SignalKind) {
    SigKind_None = '-',
    SigKind_Bus = 'b',
    SigKind_Register = 'r'
};

struct vaul_id_set {

    vaul_id_set();
    ~vaul_id_set();

    void add(Id);
    void remove(Id);
    void init();

    bool contains(Id);

private:
    Id *ids;
    int n_ids;
};

typedef vaul_design_unit *graph_region_id;
#define GRAPH_NULL_REGION_ID NULL

enum nKind {
	nkInvalid, nkLRef, nkXRef,
	nkNode, 
	nkDummy, 
	nkAssociationList, 
	nkCompInstList, 
	nkIncrementalBindingIndic, 
	nkBindingIndic, 
	nkComponentSpec, 
	nkWaveformElement, 
	nkSignalList, 
	nkAssociation, 
	nkPosNode, 
	nkInstList, 
	nkInstList_OTHERS, 
	nkInstList_ALL, 
	nkInstList_Ids, 
	nkConfigSpec, 
	nkCondalSignalAssign, 
	nkCondalWaveform, 
	nkIterationScheme, 
	nkPreForScheme, 
	nkForScheme, 
	nkWhileScheme, 
	nkCaseAlternative, 
	nkStatement, 
	nkAssertStat, 
	nkSignalAssignment, 
	nkWaitStat, 
	nkProcedureCallStat, 
	nkNullStat, 
	nkLoopControlStat, 
	nkExitStat, 
	nkNextStat, 
	nkLoopStat, 
	nkCaseStat, 
	nkIfStat, 
	nkVarAssignment, 
	nkReturnStat, 
	nkDummyStat, 
	nkChoice, 
	nkOthersChoice, 
	nkNamedChoice, 
	nkRangeChoice, 
	nkExprChoice, 
	nkElemAssoc, 
	nkArrAggrAssoc, 
	nkOthersArrAggrAssoc, 
	nkRangeArrAggrAssoc, 
	nkSliceArrAggrAssoc, 
	nkSingleArrAggrAssoc, 
	nkRecAggrAssoc, 
	nkEntityNameList, 
	nkEntityNameList_OTHERS, 
	nkEntityNameList_ALL, 
	nkEntityNameList_Ids, 
	nkEntitySpec, 
	nkAttributeSpec, 
	nkIndexValue, 
	nkGenAssocElem, 
	nkRangeAssocElem, 
	nkSubtypeAssocElem, 
	nkNamedAssocElem, 
	nkExpr, 
	nkAmbgAggregate, 
	nkArtificialAmbgAggregate, 
	nkAggregate, 
	nkArrayAggregate, 
	nkRecordAggregate, 
	nkArrayAttr, 
	nkArrayAttr_LENGTH, 
	nkArrayAttr_ASCENDING, 
	nkArrayAttr_LOW, 
	nkArrayAttr_HIGH, 
	nkArrayAttr_RIGHT, 
	nkArrayAttr_LEFT, 
	nkAttrFunctionCall, 
	nkAttrTypeFunc, 
	nkAttr_RIGHTOF, 
	nkAttr_LEFTOF, 
	nkAttr_PRED, 
	nkAttr_SUCC, 
	nkAttr_VAL, 
	nkAttr_POS, 
	nkAttr_VALUE, 
	nkAttr_IMAGE, 
	nkAttr_ASCENDING, 
	nkAttr_HIGH, 
	nkAttr_LOW, 
	nkAttr_RIGHT, 
	nkAttr_LEFT, 
	nkAttrSigFunc, 
	nkAttr_DRIVING_VALUE, 
	nkAttr_DRIVING, 
	nkAttr_LAST_VALUE, 
	nkAttr_LAST_ACTIVE, 
	nkAttr_LAST_EVENT, 
	nkAttr_ACTIVE, 
	nkAttr_EVENT, 
	nkPrimaryExpr, 
	nkAmbgEnumLitRef, 
	nkEnumLitRef, 
	nkObjectRef, 
	nkAttrSignalRef, 
	nkAttr_TRANSACTION, 
	nkAttr_QUIET, 
	nkAttr_STABLE, 
	nkAttr_DELAYED, 
	nkGenericArrayObjectRef, 
	nkSliceObjectRef, 
	nkArrayObjectRef, 
	nkRecordObjectRef, 
	nkAccessObjectRef, 
	nkSimpleObjectRef, 
	nkNullExpr, 
	nkAmbgNullExpr, 
	nkArrayLiteralRef, 
	nkAmbgArrayLitRef, 
	nkLiteralRef, 
	nkPhysicalLiteralRef, 
	nkOpenExpr, 
	nkNewExpr, 
	nkQualifiedExpr, 
	nkTypeConversion, 
	nkAmbgCall, 
	nkProcedureCall, 
	nkFunctionCall, 
	nkUnresolvedName, 
	nkRecordElement, 
	nkIndexType, 
	nkConstraint, 
	nkIndexConstraint, 
	nkPreIndexConstraint, 
	nkPreIndexSubtypeConstraint, 
	nkPreIndexRangeConstraint, 
	nkRange, 
	nkArrayRange, 
	nkArrayAttr_REVERSE_RANGE, 
	nkArrayAttr_RANGE, 
	nkExplicitRange, 
	nkAttributeValue, 
	nkDeclaration, 
	nkLabel, 
	nkConstantBody, 
	nkObject, 
	nkInterface, 
	nkFile, 
	nkAlias, 
	nkSignal, 
	nkGuardSignal, 
	nkConstant, 
	nkVariable, 
	nkEnumLiteral, 
	nkPhysicalUnit, 
	nkType, 
	nkDummyType, 
	nkFileType, 
	nkCompositeType, 
	nkRecordType, 
	nkArrayType, 
	nkSubarrayType, 
	nkScalarType, 
	nkEnumType, 
	nkNumericType, 
	nkPhysicalType, 
	nkFloatingType, 
	nkUniversalReal, 
	nkIntegerType, 
	nkUniversalInteger, 
	nkAccessType, 
	nkSubType, 
	nkIncompleteType, 
	nkIndirectDecl, 
	nkLibNameDecl, 
	nkAttribute, 
	nkAttributedDeclaration, 
	nkScope, 
	nkBaseConfig, 
	nkCompConfig, 
	nkBlockConfig, 
	nkConcurrentStatement, 
	nkComponentInst, 
	nkComponent, 
	nkConfiguration, 
	nkBlock, 
	nkBlockStat, 
	nkArchitecture, 
	nkEntity, 
	nkProcess, 
	nkLoopScope, 
	nkSubprogramBody, 
	nkSubprogram, 
	nkFunction, 
	nkPredefOp, 
	nkProcedure, 
	nkPackageBody, 
	nkPackage, 
	nkStandardPackage, 
	nkTopScope, 
	nkSelNameList, 
	nkName, 
	nkAttributeName, 
	nkIftsName, 
	nkSelName, 
	nkOpSymbol, 
	nkSimpleName, 
	nkIdList
};

typedef unsigned char nKind_ShortRep;

struct sNode;
typedef sNode *nNode;
struct sPosNode;
typedef sPosNode *nPosNode;
struct sIdList;
typedef sIdList *nIdList;
struct sName;
typedef sName *nName;
struct sSimpleName;
typedef sSimpleName *nSimpleName;
struct sOpSymbol;
typedef sOpSymbol *nOpSymbol;
struct sSelName;
typedef sSelName *nSelName;
struct sIftsName;
typedef sIftsName *nIftsName;
struct sAttributeName;
typedef sAttributeName *nAttributeName;
struct sSelNameList;
typedef sSelNameList *nSelNameList;
struct sDeclaration;
typedef sDeclaration *nDeclaration;
struct sAttributedDeclaration;
typedef sAttributedDeclaration *nAttributedDeclaration;
struct sAttribute;
typedef sAttribute *nAttribute;
struct sAttributeValue;
typedef sAttributeValue *nAttributeValue;
struct sScope;
typedef sScope *nScope;
struct sTopScope;
typedef sTopScope *nTopScope;
struct sLibNameDecl;
typedef sLibNameDecl *nLibNameDecl;
struct sIndirectDecl;
typedef sIndirectDecl *nIndirectDecl;
struct sPackage;
typedef sPackage *nPackage;
struct sStandardPackage;
typedef sStandardPackage *nStandardPackage;
struct sPackageBody;
typedef sPackageBody *nPackageBody;
struct sType;
typedef sType *nType;
struct sConstraint;
typedef sConstraint *nConstraint;
struct sRange;
typedef sRange *nRange;
struct sExplicitRange;
typedef sExplicitRange *nExplicitRange;
struct sArrayRange;
typedef sArrayRange *nArrayRange;
struct sArrayAttr_RANGE;
typedef sArrayAttr_RANGE *nArrayAttr_RANGE;
struct sArrayAttr_REVERSE_RANGE;
typedef sArrayAttr_REVERSE_RANGE *nArrayAttr_REVERSE_RANGE;
struct sPreIndexConstraint;
typedef sPreIndexConstraint *nPreIndexConstraint;
struct sPreIndexRangeConstraint;
typedef sPreIndexRangeConstraint *nPreIndexRangeConstraint;
struct sPreIndexSubtypeConstraint;
typedef sPreIndexSubtypeConstraint *nPreIndexSubtypeConstraint;
struct sIndexConstraint;
typedef sIndexConstraint *nIndexConstraint;
struct sIncompleteType;
typedef sIncompleteType *nIncompleteType;
struct sSubType;
typedef sSubType *nSubType;
struct sAccessType;
typedef sAccessType *nAccessType;
struct sScalarType;
typedef sScalarType *nScalarType;
struct sNumericType;
typedef sNumericType *nNumericType;
struct sIntegerType;
typedef sIntegerType *nIntegerType;
struct sFloatingType;
typedef sFloatingType *nFloatingType;
struct sUniversalInteger;
typedef sUniversalInteger *nUniversalInteger;
struct sUniversalReal;
typedef sUniversalReal *nUniversalReal;
struct sPhysicalType;
typedef sPhysicalType *nPhysicalType;
struct sPhysicalUnit;
typedef sPhysicalUnit *nPhysicalUnit;
struct sEnumType;
typedef sEnumType *nEnumType;
struct sEnumLiteral;
typedef sEnumLiteral *nEnumLiteral;
struct sCompositeType;
typedef sCompositeType *nCompositeType;
struct sArrayType;
typedef sArrayType *nArrayType;
struct sIndexType;
typedef sIndexType *nIndexType;
struct sSubarrayType;
typedef sSubarrayType *nSubarrayType;
struct sRecordType;
typedef sRecordType *nRecordType;
struct sRecordElement;
typedef sRecordElement *nRecordElement;
struct sFileType;
typedef sFileType *nFileType;
struct sDummyType;
typedef sDummyType *nDummyType;
struct sObject;
typedef sObject *nObject;
struct sVariable;
typedef sVariable *nVariable;
struct sConstant;
typedef sConstant *nConstant;
struct sConstantBody;
typedef sConstantBody *nConstantBody;
struct sSignal;
typedef sSignal *nSignal;
struct sGuardSignal;
typedef sGuardSignal *nGuardSignal;
struct sAlias;
typedef sAlias *nAlias;
struct sFile;
typedef sFile *nFile;
struct sSubprogram;
typedef sSubprogram *nSubprogram;
struct sProcedure;
typedef sProcedure *nProcedure;
struct sFunction;
typedef sFunction *nFunction;
struct sPredefOp;
typedef sPredefOp *nPredefOp;
struct sInterface;
typedef sInterface *nInterface;
struct sSubprogramBody;
typedef sSubprogramBody *nSubprogramBody;
struct sExpr;
typedef sExpr *nExpr;
struct sUnresolvedName;
typedef sUnresolvedName *nUnresolvedName;
struct sFunctionCall;
typedef sFunctionCall *nFunctionCall;
struct sProcedureCall;
typedef sProcedureCall *nProcedureCall;
struct sAmbgCall;
typedef sAmbgCall *nAmbgCall;
struct sGenAssocElem;
typedef sGenAssocElem *nGenAssocElem;
struct sNamedAssocElem;
typedef sNamedAssocElem *nNamedAssocElem;
struct sSubtypeAssocElem;
typedef sSubtypeAssocElem *nSubtypeAssocElem;
struct sRangeAssocElem;
typedef sRangeAssocElem *nRangeAssocElem;
struct sAssociation;
typedef sAssociation *nAssociation;
struct sTypeConversion;
typedef sTypeConversion *nTypeConversion;
struct sQualifiedExpr;
typedef sQualifiedExpr *nQualifiedExpr;
struct sNewExpr;
typedef sNewExpr *nNewExpr;
struct sPrimaryExpr;
typedef sPrimaryExpr *nPrimaryExpr;
struct sOpenExpr;
typedef sOpenExpr *nOpenExpr;
struct sLiteralRef;
typedef sLiteralRef *nLiteralRef;
struct sAmbgArrayLitRef;
typedef sAmbgArrayLitRef *nAmbgArrayLitRef;
struct sArrayLiteralRef;
typedef sArrayLiteralRef *nArrayLiteralRef;
struct sPhysicalLiteralRef;
typedef sPhysicalLiteralRef *nPhysicalLiteralRef;
struct sAmbgNullExpr;
typedef sAmbgNullExpr *nAmbgNullExpr;
struct sNullExpr;
typedef sNullExpr *nNullExpr;
struct sObjectRef;
typedef sObjectRef *nObjectRef;
struct sSimpleObjectRef;
typedef sSimpleObjectRef *nSimpleObjectRef;
struct sAccessObjectRef;
typedef sAccessObjectRef *nAccessObjectRef;
struct sRecordObjectRef;
typedef sRecordObjectRef *nRecordObjectRef;
struct sGenericArrayObjectRef;
typedef sGenericArrayObjectRef *nGenericArrayObjectRef;
struct sArrayObjectRef;
typedef sArrayObjectRef *nArrayObjectRef;
struct sIndexValue;
typedef sIndexValue *nIndexValue;
struct sSliceObjectRef;
typedef sSliceObjectRef *nSliceObjectRef;
struct sEnumLitRef;
typedef sEnumLitRef *nEnumLitRef;
struct sAmbgEnumLitRef;
typedef sAmbgEnumLitRef *nAmbgEnumLitRef;
struct sAttrSignalRef;
typedef sAttrSignalRef *nAttrSignalRef;
struct sAttr_DELAYED;
typedef sAttr_DELAYED *nAttr_DELAYED;
struct sAttr_STABLE;
typedef sAttr_STABLE *nAttr_STABLE;
struct sAttr_QUIET;
typedef sAttr_QUIET *nAttr_QUIET;
struct sAttr_TRANSACTION;
typedef sAttr_TRANSACTION *nAttr_TRANSACTION;
struct sAttrFunctionCall;
typedef sAttrFunctionCall *nAttrFunctionCall;
struct sAttrSigFunc;
typedef sAttrSigFunc *nAttrSigFunc;
struct sAttr_EVENT;
typedef sAttr_EVENT *nAttr_EVENT;
struct sAttr_ACTIVE;
typedef sAttr_ACTIVE *nAttr_ACTIVE;
struct sAttr_LAST_EVENT;
typedef sAttr_LAST_EVENT *nAttr_LAST_EVENT;
struct sAttr_LAST_ACTIVE;
typedef sAttr_LAST_ACTIVE *nAttr_LAST_ACTIVE;
struct sAttr_LAST_VALUE;
typedef sAttr_LAST_VALUE *nAttr_LAST_VALUE;
struct sAttr_DRIVING;
typedef sAttr_DRIVING *nAttr_DRIVING;
struct sAttr_DRIVING_VALUE;
typedef sAttr_DRIVING_VALUE *nAttr_DRIVING_VALUE;
struct sAttrTypeFunc;
typedef sAttrTypeFunc *nAttrTypeFunc;
struct sAttr_LEFT;
typedef sAttr_LEFT *nAttr_LEFT;
struct sAttr_RIGHT;
typedef sAttr_RIGHT *nAttr_RIGHT;
struct sAttr_LOW;
typedef sAttr_LOW *nAttr_LOW;
struct sAttr_HIGH;
typedef sAttr_HIGH *nAttr_HIGH;
struct sAttr_ASCENDING;
typedef sAttr_ASCENDING *nAttr_ASCENDING;
struct sAttr_IMAGE;
typedef sAttr_IMAGE *nAttr_IMAGE;
struct sAttr_VALUE;
typedef sAttr_VALUE *nAttr_VALUE;
struct sAttr_POS;
typedef sAttr_POS *nAttr_POS;
struct sAttr_VAL;
typedef sAttr_VAL *nAttr_VAL;
struct sAttr_SUCC;
typedef sAttr_SUCC *nAttr_SUCC;
struct sAttr_PRED;
typedef sAttr_PRED *nAttr_PRED;
struct sAttr_LEFTOF;
typedef sAttr_LEFTOF *nAttr_LEFTOF;
struct sAttr_RIGHTOF;
typedef sAttr_RIGHTOF *nAttr_RIGHTOF;
struct sArrayAttr;
typedef sArrayAttr *nArrayAttr;
struct sArrayAttr_LEFT;
typedef sArrayAttr_LEFT *nArrayAttr_LEFT;
struct sArrayAttr_RIGHT;
typedef sArrayAttr_RIGHT *nArrayAttr_RIGHT;
struct sArrayAttr_HIGH;
typedef sArrayAttr_HIGH *nArrayAttr_HIGH;
struct sArrayAttr_LOW;
typedef sArrayAttr_LOW *nArrayAttr_LOW;
struct sArrayAttr_ASCENDING;
typedef sArrayAttr_ASCENDING *nArrayAttr_ASCENDING;
struct sArrayAttr_LENGTH;
typedef sArrayAttr_LENGTH *nArrayAttr_LENGTH;
struct sAttributeSpec;
typedef sAttributeSpec *nAttributeSpec;
struct sEntitySpec;
typedef sEntitySpec *nEntitySpec;
struct sEntityNameList;
typedef sEntityNameList *nEntityNameList;
struct sEntityNameList_Ids;
typedef sEntityNameList_Ids *nEntityNameList_Ids;
struct sEntityNameList_ALL;
typedef sEntityNameList_ALL *nEntityNameList_ALL;
struct sEntityNameList_OTHERS;
typedef sEntityNameList_OTHERS *nEntityNameList_OTHERS;
struct sAggregate;
typedef sAggregate *nAggregate;
struct sRecordAggregate;
typedef sRecordAggregate *nRecordAggregate;
struct sRecAggrAssoc;
typedef sRecAggrAssoc *nRecAggrAssoc;
struct sArrayAggregate;
typedef sArrayAggregate *nArrayAggregate;
struct sArrAggrAssoc;
typedef sArrAggrAssoc *nArrAggrAssoc;
struct sSingleArrAggrAssoc;
typedef sSingleArrAggrAssoc *nSingleArrAggrAssoc;
struct sRangeArrAggrAssoc;
typedef sRangeArrAggrAssoc *nRangeArrAggrAssoc;
struct sSliceArrAggrAssoc;
typedef sSliceArrAggrAssoc *nSliceArrAggrAssoc;
struct sOthersArrAggrAssoc;
typedef sOthersArrAggrAssoc *nOthersArrAggrAssoc;
struct sAmbgAggregate;
typedef sAmbgAggregate *nAmbgAggregate;
struct sArtificialAmbgAggregate;
typedef sArtificialAmbgAggregate *nArtificialAmbgAggregate;
struct sElemAssoc;
typedef sElemAssoc *nElemAssoc;
struct sChoice;
typedef sChoice *nChoice;
struct sExprChoice;
typedef sExprChoice *nExprChoice;
struct sRangeChoice;
typedef sRangeChoice *nRangeChoice;
struct sNamedChoice;
typedef sNamedChoice *nNamedChoice;
struct sOthersChoice;
typedef sOthersChoice *nOthersChoice;
struct sLabel;
typedef sLabel *nLabel;
struct sStatement;
typedef sStatement *nStatement;
struct sDummyStat;
typedef sDummyStat *nDummyStat;
struct sReturnStat;
typedef sReturnStat *nReturnStat;
struct sVarAssignment;
typedef sVarAssignment *nVarAssignment;
struct sIfStat;
typedef sIfStat *nIfStat;
struct sCaseStat;
typedef sCaseStat *nCaseStat;
struct sCaseAlternative;
typedef sCaseAlternative *nCaseAlternative;
struct sLoopStat;
typedef sLoopStat *nLoopStat;
struct sLoopScope;
typedef sLoopScope *nLoopScope;
struct sIterationScheme;
typedef sIterationScheme *nIterationScheme;
struct sWhileScheme;
typedef sWhileScheme *nWhileScheme;
struct sForScheme;
typedef sForScheme *nForScheme;
struct sPreForScheme;
typedef sPreForScheme *nPreForScheme;
struct sLoopControlStat;
typedef sLoopControlStat *nLoopControlStat;
struct sNextStat;
typedef sNextStat *nNextStat;
struct sExitStat;
typedef sExitStat *nExitStat;
struct sNullStat;
typedef sNullStat *nNullStat;
struct sProcedureCallStat;
typedef sProcedureCallStat *nProcedureCallStat;
struct sWaitStat;
typedef sWaitStat *nWaitStat;
struct sSignalList;
typedef sSignalList *nSignalList;
struct sWaveformElement;
typedef sWaveformElement *nWaveformElement;
struct sSignalAssignment;
typedef sSignalAssignment *nSignalAssignment;
struct sAssertStat;
typedef sAssertStat *nAssertStat;
struct sConcurrentStatement;
typedef sConcurrentStatement *nConcurrentStatement;
struct sProcess;
typedef sProcess *nProcess;
struct sCondalWaveform;
typedef sCondalWaveform *nCondalWaveform;
struct sCondalSignalAssign;
typedef sCondalSignalAssign *nCondalSignalAssign;
struct sComponent;
typedef sComponent *nComponent;
struct sBlock;
typedef sBlock *nBlock;
struct sEntity;
typedef sEntity *nEntity;
struct sArchitecture;
typedef sArchitecture *nArchitecture;
struct sBlockStat;
typedef sBlockStat *nBlockStat;
struct sComponentInst;
typedef sComponentInst *nComponentInst;
struct sConfigSpec;
typedef sConfigSpec *nConfigSpec;
struct sComponentSpec;
typedef sComponentSpec *nComponentSpec;
struct sInstList;
typedef sInstList *nInstList;
struct sInstList_Ids;
typedef sInstList_Ids *nInstList_Ids;
struct sInstList_ALL;
typedef sInstList_ALL *nInstList_ALL;
struct sInstList_OTHERS;
typedef sInstList_OTHERS *nInstList_OTHERS;
struct sBindingIndic;
typedef sBindingIndic *nBindingIndic;
struct sIncrementalBindingIndic;
typedef sIncrementalBindingIndic *nIncrementalBindingIndic;
struct sConfiguration;
typedef sConfiguration *nConfiguration;
struct sBaseConfig;
typedef sBaseConfig *nBaseConfig;
struct sBlockConfig;
typedef sBlockConfig *nBlockConfig;
struct sCompConfig;
typedef sCompConfig *nCompConfig;
struct sCompInstList;
typedef sCompInstList *nCompInstList;
struct sDummy;
typedef sDummy *nDummy;
struct sAssociationList;
typedef sAssociationList *nAssociationList;
typedef sNode graph_base_node;
typedef nKind graph_node_kind;
/* this file is inserted into the graph header file

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

// The whole graph is devided into several regions. Every node belongs
// to exactly one region. One of the regions is the special `region of
// the homeless nodes' (rohn).

// All regions except the rohn are considered static and each node of
// such a region is identified by a label. Thus, a node is completely
// identified by a region and a label. Nodes of the rohn can not be
// identified at all. Likewise, nodes of the rohn can not be
// systematically visited. They can only be written to a file and
// thereby become ordinary 'homed' nodes.


#if 0
// identifies a region. All values denote valid regions, including
// NULL which denotes the special `region of the homeless nodes'.
//
typedef blah graph_region_id; // defined by user

struct graph_base_node {

    graph_base_node()   { label = -1 }
    ~graph_base_node()  { }

    graph_region_id get_region_id();     // implemented by user
    void set_region_id(graph_region_id); // implemented by user

    int get_label()     { return label; }

private:
    friend struct graph_runner;
    int label;
};
#endif

struct graph_marker {

    graph_marker(graph_region_id);
    ~graph_marker();

    bool marked(graph_base_node *);
    void mark(graph_base_node *);

    graph_base_node *find_node(int label);

protected:
    graph_region_id region_id;

private:
    int n_marks, max_marks;
    graph_base_node **marks;
};

struct graph_visitor;
typedef void graph_visit_f(graph_base_node *, graph_visitor *);

struct graph_visitor : private graph_marker {

    graph_visitor(graph_region_id);
    ~graph_visitor();

    void visit(graph_visit_f *pre,
	       graph_visit_f *post,
	       graph_base_node *start);
};

struct graph_reader : private graph_marker {

    graph_reader(graph_region_id);
    ~graph_reader();

    graph_base_node *read(graph_node_kind expected);

    graph_base_node *find_node(int label);

private:
    virtual graph_node_kind read_kind() = 0;
    virtual graph_base_node *read_xref() = 0;
    virtual int read_label() = 0;
    virtual void read_error(graph_node_kind read,
			    graph_node_kind expected) = 0;
};

struct graph_writer : private graph_marker {

    graph_writer(graph_region_id);
    ~graph_writer();

    void write(graph_base_node *head);

private:
    virtual void write_kind(graph_node_kind);
    virtual void write_xref(graph_base_node *);
    virtual void write_label(int);
};

struct graph_printer : private graph_marker {

    graph_printer(graph_region_id);
    ~graph_printer();

    void print(graph_base_node *);

    void print_lab(const char *lab);

private:
    virtual void print_lref(graph_base_node *);
    virtual void print_xref(graph_base_node *);
    virtual void print_header(graph_base_node *);
    virtual void print_string(const char *);

    int level;
};
typedef void graph_visit_f(graph_base_node *, graph_visitor *);

struct sNode {

	sNode()
	{
		this->label = -1;
		set_region_id(GRAPH_NULL_REGION_ID);
	}

 	vaul_design_unit *owner;
	vaul_design_unit *get_region_id() { return owner; }
	void set_region_id(vaul_design_unit *o) { owner = o; }
	int label;

	virtual nKind kind();
	char *type_id();
	static char *type_id(nKind);
	bool is(nKind k);
	nKind base_kind() { return base_kind(kind()); }
	static nKind base_kind(nKind);
	static bool is_base_of(nKind b, nKind d);
	static nNode create(nKind, void *placement_hint);
	int get_label() { return label; }

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sPosNode : sNode {

	sPosNode(int lineno)
	  : sNode()
	{
		this->lineno = lineno;
	}
	sPosNode() : sNode() {

}

	int lineno;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sIdList : sPosNode {

	sIdList(int lineno, Id id, sIdList *link)
	  : sPosNode(lineno)
	{
		this->id = id;
		this->link = link;
	}
	sIdList() : sPosNode() {

}

	Id id;
	nIdList link;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sName : sPosNode {

	sName(int lineno)
	  : sPosNode(lineno)
	{
	}
	sName() : sPosNode() {

}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sSimpleName : sName {

	sSimpleName(int lineno, Id id)
	  : sName(lineno)
	{
		this->id = id;
	}
	sSimpleName() : sName() {

}

	Id id;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sOpSymbol : sName {

	sOpSymbol(int lineno, Id op)
	  : sName(lineno)
	{
		this->op = op;
	}
	sOpSymbol() : sName() {

}

	Id op;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sSelName : sName {

	sSelName(int lineno, sName *prefix, Id suffix)
	  : sName(lineno)
	{
		this->prefix = prefix;
		this->suffix = suffix;
	}
	sSelName() : sName() {

}

	nName prefix;
	Id suffix;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sIftsName : sName {

	sIftsName(int lineno, sName *prefix, sGenAssocElem *assoc)
	  : sName(lineno)
	{
		this->prefix = prefix;
		this->assoc = assoc;
	}
	sIftsName() : sName() {

}

	nName prefix;
	nGenAssocElem assoc;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sAttributeName : sName {

	sAttributeName(int lineno, sName *prefix, Id attribute, sNamedAssocElem *first_actual)
	  : sName(lineno)
	{
		this->prefix = prefix;
		this->attribute = attribute;
		this->first_actual = first_actual;
	}
	sAttributeName() : sName() {

}

	nName prefix;
	Id attribute;
	nNamedAssocElem first_actual;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sSelNameList : sPosNode {

	sSelNameList(int lineno, sSelName *name, sSelNameList *link)
	  : sPosNode(lineno)
	{
		this->name = name;
		this->link = link;
	}
	sSelNameList() : sPosNode() {

}

	nSelName name;
	nSelNameList link;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sDeclaration : sPosNode {

	sDeclaration(int lineno, Id id)
	  : sPosNode(lineno)
	{
		this->next_decl = NULL;
		this->id = id;
		this->scope = NULL;
	}
	sDeclaration() : sPosNode() {

}

	nDeclaration next_decl;
	Id id;
	nScope scope;
 	char *idstr() { return id? id : "???"; }
	nDeclaration next();

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sAttributedDeclaration : sDeclaration {

	sAttributedDeclaration(int lineno, Id id)
	  : sDeclaration(lineno, id)
	{
		this->first_attribute = NULL;
	}
	sAttributedDeclaration() : sDeclaration() {

}

	nAttributeValue first_attribute;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sAttribute : sDeclaration {

	sAttribute(int lineno, Id id, sType *type)
	  : sDeclaration(lineno, id)
	{
		this->type = type;
	}
	sAttribute() : sDeclaration() {

}

	nType type;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sAttributeValue : sPosNode {

	sAttributeValue(int lineno, sAttributeValue *next, sAttribute *attribute, sExpr *value)
	  : sPosNode(lineno)
	{
		this->next = next;
		this->attribute = attribute;
		this->value = value;
	}
	sAttributeValue() : sPosNode() {

}

	nAttributeValue next;
	nAttribute attribute;
	nExpr value;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sScope : sAttributedDeclaration {

	sScope(int lineno, Id id)
	  : sAttributedDeclaration(lineno, id)
	{
		this->continued = NULL;
		this->first_decl = NULL;
	}
	sScope() : sAttributedDeclaration() {

}

	nScope continued;
	nDeclaration first_decl;
 	nDeclaration first();
	vaul_id_set decls_in_flight;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sTopScope : sScope {

	sTopScope(int lineno, Id id)
	  : sScope(lineno, id)
	{
	}
	sTopScope() : sScope() {

}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sLibNameDecl : sDeclaration {

	sLibNameDecl(int lineno, Id id)
	  : sDeclaration(lineno, id)
	{
	}
	sLibNameDecl() : sDeclaration() {

}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sIndirectDecl : sDeclaration {

	sIndirectDecl(int lineno, Id id, sPackage *ind_scope)
	  : sDeclaration(lineno, id)
	{
		this->ind_scope = ind_scope;
	}
	sIndirectDecl() : sDeclaration() {

}

	nPackage ind_scope;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sPackage : sScope {

	sPackage(int lineno, Id id)
	  : sScope(lineno, id)
	{
	}
	sPackage() : sScope() {

}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sStandardPackage : sPackage {

	sStandardPackage(int lineno, Id id)
	  : sPackage(lineno, id)
	{
		this->predef_BIT = NULL;
		this->predef_BOOLEAN = NULL;
		this->predef_INTEGER = NULL;
		this->predef_REAL = NULL;
		this->predef_TIME = NULL;
		this->predef_STRING = NULL;
		this->predef_BIT_VECTOR = NULL;
		this->predef_SEVERITY_LEVEL = NULL;
		this->predef_FILE_OPEN_KIND = NULL;
		this->universal_integer = NULL;
		this->universal_real = NULL;
	}
	sStandardPackage() : sPackage() {

}

	nType predef_BIT;
	nType predef_BOOLEAN;
	nType predef_INTEGER;
	nType predef_REAL;
	nType predef_TIME;
	nType predef_STRING;
	nType predef_BIT_VECTOR;
	nType predef_SEVERITY_LEVEL;
	nType predef_FILE_OPEN_KIND;
	nUniversalInteger universal_integer;
	nUniversalReal universal_real;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sPackageBody : sScope {

	sPackageBody(int lineno, Id id)
	  : sScope(lineno, id)
	{
	}
	sPackageBody() : sScope() {

}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sType : sDeclaration {

	sType(int lineno, Id id)
	  : sDeclaration(lineno, id)
	{
	}
	sType() : sDeclaration() {

}

 	nType get_base();

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sConstraint : sPosNode {

	sConstraint(int lineno)
	  : sPosNode(lineno)
	{
	}
	sConstraint() : sPosNode() {

}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sRange : sConstraint {

	sRange(int lineno)
	  : sConstraint(lineno)
	{
	}
	sRange() : sConstraint() {

}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sExplicitRange : sRange {

	sExplicitRange(int lineno, RangeDirection dir, sExpr *first, sExpr *last)
	  : sRange(lineno)
	{
		this->dir = dir;
		this->first = first;
		this->last = last;
	}
	sExplicitRange() : sRange() {

}

	RangeDirection dir;
	nExpr first;
	nExpr last;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sArrayRange : sRange {

	sArrayRange(int lineno, sType *type, sObject *array, int index)
	  : sRange(lineno)
	{
		this->type = type;
		this->array = array;
		this->index = index;
	}
	sArrayRange() : sRange() {

}

	nType type;
	nObject array;
	int index;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sArrayAttr_RANGE : sArrayRange {

	sArrayAttr_RANGE(int lineno, sType *type, sObject *array, int index)
	  : sArrayRange(lineno, type, array, index)
	{
	}
	sArrayAttr_RANGE() : sArrayRange() {

}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sArrayAttr_REVERSE_RANGE : sArrayRange {

	sArrayAttr_REVERSE_RANGE(int lineno, sType *type, sObject *array, int index)
	  : sArrayRange(lineno, type, array, index)
	{
	}
	sArrayAttr_REVERSE_RANGE() : sArrayRange() {

}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sPreIndexConstraint : sConstraint {

	sPreIndexConstraint(int lineno, sPreIndexConstraint *next)
	  : sConstraint(lineno)
	{
		this->next = next;
	}
	sPreIndexConstraint() : sConstraint() {

}

	nPreIndexConstraint next;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sPreIndexRangeConstraint : sPreIndexConstraint {

	sPreIndexRangeConstraint(int lineno, sPreIndexConstraint *next, sRange *range)
	  : sPreIndexConstraint(lineno, next)
	{
		this->range = range;
	}
	sPreIndexRangeConstraint() : sPreIndexConstraint() {

}

	nRange range;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sPreIndexSubtypeConstraint : sPreIndexConstraint {

	sPreIndexSubtypeConstraint(int lineno, sPreIndexConstraint *next, sType *type)
	  : sPreIndexConstraint(lineno, next)
	{
		this->type = type;
	}
	sPreIndexSubtypeConstraint() : sPreIndexConstraint() {

}

	nType type;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sIndexConstraint : sConstraint {

	sIndexConstraint(int lineno, sIndexConstraint *next, sType *type)
	  : sConstraint(lineno)
	{
		this->next = next;
		this->type = type;
	}
	sIndexConstraint() : sConstraint() {

}

	nIndexConstraint next;
	nType type;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sIncompleteType : sType {

	sIncompleteType(int lineno, Id id)
	  : sType(lineno, id)
	{
 	uses = NULL;
	}
	sIncompleteType() : sType() {
 	uses = NULL;

}

 	struct use { use *next; nType *ref; } *uses;
	void add_use(nType &);
	void complete(nType real);

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sSubType : sType {

	sSubType(int lineno, Id id, sType *base, sConstraint *constraint, sName *resol_func)
	  : sType(lineno, id)
	{
		this->base = base;
		this->constraint = constraint;
		this->resol_func = resol_func;
	}
	sSubType() : sType() {

}

	nType base;
	nConstraint constraint;
	nName resol_func;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sAccessType : sType {

	sAccessType(int lineno, Id id, sType *designated)
	  : sType(lineno, id)
	{
		this->designated = designated;
	}
	sAccessType() : sType() {

}

	nType designated;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sScalarType : sType {

	sScalarType(int lineno, Id id)
	  : sType(lineno, id)
	{
	}
	sScalarType() : sType() {

}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sNumericType : sScalarType {

	sNumericType(int lineno, Id id)
	  : sScalarType(lineno, id)
	{
	}
	sNumericType() : sScalarType() {

}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sIntegerType : sNumericType {

	sIntegerType(int lineno, Id id)
	  : sNumericType(lineno, id)
	{
	}
	sIntegerType() : sNumericType() {

}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sFloatingType : sNumericType {

	sFloatingType(int lineno, Id id)
	  : sNumericType(lineno, id)
	{
	}
	sFloatingType() : sNumericType() {

}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sUniversalInteger : sIntegerType {

	sUniversalInteger(int lineno, Id id)
	  : sIntegerType(lineno, id)
	{
	}
	sUniversalInteger() : sIntegerType() {

}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sUniversalReal : sFloatingType {

	sUniversalReal(int lineno, Id id)
	  : sFloatingType(lineno, id)
	{
	}
	sUniversalReal() : sFloatingType() {

}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sPhysicalType : sNumericType {

	sPhysicalType(int lineno, Id id, sPhysicalUnit *first, sSubType *declaring_subtype)
	  : sNumericType(lineno, id)
	{
		this->first = first;
		this->declaring_subtype = declaring_subtype;
	}
	sPhysicalType() : sNumericType() {

}

	nPhysicalUnit first;
	nSubType declaring_subtype;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sPhysicalUnit : sDeclaration {

	sPhysicalUnit(int lineno, Id id, sPhysicalType *type, sPhysicalLiteralRef *value, sPhysicalUnit *next)
	  : sDeclaration(lineno, id)
	{
		this->type = type;
		this->value = value;
		this->next = next;
	}
	sPhysicalUnit() : sDeclaration() {

}

	nPhysicalType type;
	nPhysicalLiteralRef value;
	nPhysicalUnit next;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sEnumType : sScalarType {

	sEnumType(int lineno, Id id, sEnumLiteral *first)
	  : sScalarType(lineno, id)
	{
		this->first = first;
	}
	sEnumType() : sScalarType() {

}

	nEnumLiteral first;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sEnumLiteral : sDeclaration {

	sEnumLiteral(int lineno, Id id, sEnumType *type, sEnumLiteral *next)
	  : sDeclaration(lineno, id)
	{
		this->type = type;
		this->next = next;
	}
	sEnumLiteral() : sDeclaration() {

}

	nEnumType type;
	nEnumLiteral next;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sCompositeType : sType {

	sCompositeType(int lineno, Id id)
	  : sType(lineno, id)
	{
	}
	sCompositeType() : sType() {

}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sArrayType : sCompositeType {

	sArrayType(int lineno, Id id, sIndexType *first_index, sType *element_type)
	  : sCompositeType(lineno, id)
	{
		this->first_index = first_index;
		this->element_type = element_type;
	}
	sArrayType() : sCompositeType() {

}

	nIndexType first_index;
	nType element_type;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sIndexType : sPosNode {

	sIndexType(int lineno, sIndexType *next, sType *index_type)
	  : sPosNode(lineno)
	{
		this->next = next;
		this->index_type = index_type;
	}
	sIndexType() : sPosNode() {

}

	nIndexType next;
	nType index_type;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sSubarrayType : sArrayType {

	sSubarrayType(int lineno, Id id, sIndexType *first_index, sType *element_type, sArrayType *complete_type)
	  : sArrayType(lineno, id, first_index, element_type)
	{
		this->complete_type = complete_type;
	}
	sSubarrayType() : sArrayType() {

}

	nArrayType complete_type;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sRecordType : sCompositeType {

	sRecordType(int lineno, Id id, sRecordElement *first_element)
	  : sCompositeType(lineno, id)
	{
		this->first_element = first_element;
	}
	sRecordType() : sCompositeType() {

}

	nRecordElement first_element;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sRecordElement : sPosNode {

	sRecordElement(int lineno, sRecordElement *next, Id id, sType *type)
	  : sPosNode(lineno)
	{
		this->next = next;
		this->id = id;
		this->type = type;
	}
	sRecordElement() : sPosNode() {

}

	nRecordElement next;
	Id id;
	nType type;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sFileType : sType {

	sFileType(int lineno, Id id, sType *content_type)
	  : sType(lineno, id)
	{
		this->content_type = content_type;
	}
	sFileType() : sType() {

}

	nType content_type;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sDummyType : sType {

	sDummyType(int lineno, Id id)
	  : sType(lineno, id)
	{
	}
	sDummyType() : sType() {

}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sObject : sDeclaration {

	sObject(int lineno, Id id, sType *type, sExpr *initial_value)
	  : sDeclaration(lineno, id)
	{
		this->type = type;
		this->initial_value = initial_value;
	}
	sObject() : sDeclaration() {

}

	nType type;
	nExpr initial_value;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sVariable : sObject {

	sVariable(int lineno, Id id, sType *type, sExpr *initial_value, bool shared)
	  : sObject(lineno, id, type, initial_value)
	{
		this->shared = shared;
	}
	sVariable() : sObject() {

}

	bool shared;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sConstant : sObject {

	sConstant(int lineno, Id id, sType *type, sExpr *initial_value)
	  : sObject(lineno, id, type, initial_value)
	{
	}
	sConstant() : sObject() {

}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sConstantBody : sDeclaration {

	sConstantBody(int lineno, Id id, sConstant *decl, sExpr *initial_value)
	  : sDeclaration(lineno, id)
	{
		this->decl = decl;
		this->initial_value = initial_value;
	}
	sConstantBody() : sDeclaration() {

}

	nConstant decl;
	nExpr initial_value;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sSignal : sObject {

	sSignal(int lineno, Id id, sType *type, sExpr *initial_value, SignalKind signal_kind)
	  : sObject(lineno, id, type, initial_value)
	{
		this->signal_kind = signal_kind;
	}
	sSignal() : sObject() {

}

	SignalKind signal_kind;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sGuardSignal : sSignal {

	sGuardSignal(int lineno, Id id, sType *type, sExpr *initial_value, SignalKind signal_kind)
	  : sSignal(lineno, id, type, initial_value, signal_kind)
	{
	}
	sGuardSignal() : sSignal() {

}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sAlias : sObject {

	sAlias(int lineno, Id id, sType *type, sExpr *initial_value)
	  : sObject(lineno, id, type, initial_value)
	{
	}
	sAlias() : sObject() {

}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sFile : sObject {

	sFile(int lineno, Id id, sType *type, sExpr *initial_value, sExpr *open_mode)
	  : sObject(lineno, id, type, initial_value)
	{
		this->open_mode = open_mode;
	}
	sFile() : sObject() {

}

	nExpr open_mode;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sSubprogram : sScope {

	sSubprogram(int lineno, Id id, sInterface *first_formal)
	  : sScope(lineno, id)
	{
		this->first_formal = first_formal;
	}
	sSubprogram() : sScope() {

}

	nInterface first_formal;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sProcedure : sSubprogram {

	sProcedure(int lineno, Id id, sInterface *first_formal)
	  : sSubprogram(lineno, id, first_formal)
	{
	}
	sProcedure() : sSubprogram() {

}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sFunction : sSubprogram {

	sFunction(int lineno, Id id, sInterface *first_formal, bool pure, sType *return_type)
	  : sSubprogram(lineno, id, first_formal)
	{
		this->pure = pure;
		this->return_type = return_type;
	}
	sFunction() : sSubprogram() {

}

	bool pure;
	nType return_type;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sPredefOp : sFunction {

	sPredefOp(int lineno, Id id, sInterface *first_formal, bool pure, sType *return_type)
	  : sFunction(lineno, id, first_formal, pure, return_type)
	{
	}
	sPredefOp() : sFunction() {

}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sInterface : sObject {

	sInterface(int lineno, Id id, sType *type, sExpr *initial_value, sInterface *next_element, ObjectClass object_class, Mode mode, bool buffer)
	  : sObject(lineno, id, type, initial_value)
	{
		this->next_element = next_element;
		this->object_class = object_class;
		this->mode = mode;
		this->buffer = buffer;
	}
	sInterface() : sObject() {

}

	nInterface next_element;
	ObjectClass object_class;
	Mode mode;
	bool buffer;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sSubprogramBody : sScope {

	sSubprogramBody(int lineno, Id id)
	  : sScope(lineno, id)
	{
		this->stats = NULL;
	}
	sSubprogramBody() : sScope() {

}

	nStatement stats;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sExpr : sPosNode {

	sExpr(int lineno)
	  : sPosNode(lineno)
	{
	}
	sExpr() : sPosNode() {

}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sUnresolvedName : sExpr {

	sUnresolvedName(int lineno, sName *name)
	  : sExpr(lineno)
	{
		this->name = name;
	}
	sUnresolvedName() : sExpr() {

}

	nName name;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sFunctionCall : sExpr {

	sFunctionCall(int lineno, sFunction *func, sAssociation *first_actual)
	  : sExpr(lineno)
	{
		this->func = func;
		this->first_actual = first_actual;
	}
	sFunctionCall() : sExpr() {

}

	nFunction func;
	nAssociation first_actual;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sProcedureCall : sExpr {

	sProcedureCall(int lineno, sProcedure *proc, sAssociation *first_actual)
	  : sExpr(lineno)
	{
		this->proc = proc;
		this->first_actual = first_actual;
	}
	sProcedureCall() : sExpr() {

}

	nProcedure proc;
	nAssociation first_actual;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sAmbgCall : sExpr {

	sAmbgCall(int lineno, sNamedAssocElem *first_actual)
	  : sExpr(lineno)
	{
		this->first_actual = first_actual;
	}
	sAmbgCall() : sExpr() {

}

	nNamedAssocElem first_actual;
 	vaul_decl_set *set;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sGenAssocElem : sPosNode {

	sGenAssocElem(int lineno, sGenAssocElem *next)
	  : sPosNode(lineno)
	{
		this->next = next;
	}
	sGenAssocElem() : sPosNode() {

}

	nGenAssocElem next;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sNamedAssocElem : sGenAssocElem {

	sNamedAssocElem(int lineno, sGenAssocElem *next, sName *formal, sExpr *actual)
	  : sGenAssocElem(lineno, next)
	{
		this->formal = formal;
		this->actual = actual;
	}
	sNamedAssocElem() : sGenAssocElem() {

}

	nName formal;
	nExpr actual;
 	vaul_decl_set *ifts_decls;   // The decls of the ifts prefix
	// of the formal
	nKind ifts_kind;             // The basic kind of the ifts_decls
	nSimpleName ifts_arg_name;   // The simple name of the only ifts
	// argument

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sSubtypeAssocElem : sGenAssocElem {

	sSubtypeAssocElem(int lineno, sGenAssocElem *next, sType *type)
	  : sGenAssocElem(lineno, next)
	{
		this->type = type;
	}
	sSubtypeAssocElem() : sGenAssocElem() {

}

	nType type;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sRangeAssocElem : sGenAssocElem {

	sRangeAssocElem(int lineno, sGenAssocElem *next, sRange *range)
	  : sGenAssocElem(lineno, next)
	{
		this->range = range;
	}
	sRangeAssocElem() : sGenAssocElem() {

}

	nRange range;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sAssociation : sNode {

	sAssociation(sAssociation *next, sInterface *formal, sDeclaration *formal_conversion, sExpr *actual, sDeclaration *actual_conversion)
	  : sNode()
	{
		this->next = next;
		this->formal = formal;
		this->formal_conversion = formal_conversion;
		this->actual = actual;
		this->actual_conversion = actual_conversion;
	}
	sAssociation() : sNode() {

}

	nAssociation next;
	nInterface formal;
	nDeclaration formal_conversion;
	nExpr actual;
	nDeclaration actual_conversion;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sTypeConversion : sExpr {

	sTypeConversion(int lineno, sType *target_type, sExpr *expression)
	  : sExpr(lineno)
	{
		this->target_type = target_type;
		this->expression = expression;
	}
	sTypeConversion() : sExpr() {

}

	nType target_type;
	nExpr expression;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sQualifiedExpr : sExpr {

	sQualifiedExpr(int lineno, sType *type, sExpr *expression)
	  : sExpr(lineno)
	{
		this->type = type;
		this->expression = expression;
	}
	sQualifiedExpr() : sExpr() {

}

	nType type;
	nExpr expression;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sNewExpr : sExpr {

	sNewExpr(int lineno, sType *type, sExpr *initial_value)
	  : sExpr(lineno)
	{
		this->type = type;
		this->initial_value = initial_value;
	}
	sNewExpr() : sExpr() {

}

	nType type;
	nExpr initial_value;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sPrimaryExpr : sExpr {

	sPrimaryExpr(int lineno)
	  : sExpr(lineno)
	{
	}
	sPrimaryExpr() : sExpr() {

}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sOpenExpr : sPrimaryExpr {

	sOpenExpr(int lineno)
	  : sPrimaryExpr(lineno)
	{
	}
	sOpenExpr() : sPrimaryExpr() {

}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sLiteralRef : sPrimaryExpr {

	sLiteralRef(int lineno, Literal value)
	  : sPrimaryExpr(lineno)
	{
		this->value = value;
	}
	sLiteralRef() : sPrimaryExpr() {

}

	Literal value;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sAmbgArrayLitRef : sPrimaryExpr {

	sAmbgArrayLitRef(int lineno, Literal value)
	  : sPrimaryExpr(lineno)
	{
		this->value = value;
	}
	sAmbgArrayLitRef() : sPrimaryExpr() {

}

	Literal value;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sArrayLiteralRef : sPrimaryExpr {

	sArrayLiteralRef(int lineno, Literal value, sType *type)
	  : sPrimaryExpr(lineno)
	{
		this->value = value;
		this->type = type;
	}
	sArrayLiteralRef() : sPrimaryExpr() {

}

	Literal value;
	nType type;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sPhysicalLiteralRef : sLiteralRef {

	sPhysicalLiteralRef(int lineno, Literal value, sPhysicalUnit *unit)
	  : sLiteralRef(lineno, value)
	{
		this->unit = unit;
	}
	sPhysicalLiteralRef() : sLiteralRef() {

}

	nPhysicalUnit unit;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sAmbgNullExpr : sPrimaryExpr {

	sAmbgNullExpr(int lineno)
	  : sPrimaryExpr(lineno)
	{
	}
	sAmbgNullExpr() : sPrimaryExpr() {

}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sNullExpr : sPrimaryExpr {

	sNullExpr(int lineno, sType *type)
	  : sPrimaryExpr(lineno)
	{
		this->type = type;
	}
	sNullExpr() : sPrimaryExpr() {

}

	nType type;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sObjectRef : sPrimaryExpr {

	sObjectRef(int lineno)
	  : sPrimaryExpr(lineno)
	{
	}
	sObjectRef() : sPrimaryExpr() {

}

 	ObjectClass get_class();    
	Mode	    get_mode();
	nType	    get_type();
	bool is_constant() { return get_class() == ObjClass_Constant; }
	bool is_variable() { return get_class() == ObjClass_Variable; }
	bool is_signal()   { return get_class() == ObjClass_Signal; }
	bool is_file()	   { return get_class() == ObjClass_File; }
	bool is_readable() { return /*get_mode() != Mode_Out;*/ true; }
	bool is_writeable()
	{ /*Mode m = get_mode(); return m != Mode_In && m != Mode_Linkage;*/
	return !is_constant(); }

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sSimpleObjectRef : sObjectRef {

	sSimpleObjectRef(int lineno, sObject *object, sName *name)
	  : sObjectRef(lineno)
	{
		this->object = object;
		this->name = name;
	}
	sSimpleObjectRef() : sObjectRef() {

}

	nObject object;
	nName name;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sAccessObjectRef : sObjectRef {

	sAccessObjectRef(int lineno, sExpr *access, sType *accessed_type)
	  : sObjectRef(lineno)
	{
		this->access = access;
		this->accessed_type = accessed_type;
	}
	sAccessObjectRef() : sObjectRef() {

}

	nExpr access;
	nType accessed_type;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sRecordObjectRef : sObjectRef {

	sRecordObjectRef(int lineno, sExpr *record, sRecordType *record_type, sRecordElement *element)
	  : sObjectRef(lineno)
	{
		this->record = record;
		this->record_type = record_type;
		this->element = element;
	}
	sRecordObjectRef() : sObjectRef() {

}

	nExpr record;
	nRecordType record_type;
	nRecordElement element;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sGenericArrayObjectRef : sObjectRef {

	sGenericArrayObjectRef(int lineno, sExpr *array, sArrayType *array_type)
	  : sObjectRef(lineno)
	{
		this->array = array;
		this->array_type = array_type;
	}
	sGenericArrayObjectRef() : sObjectRef() {

}

	nExpr array;
	nArrayType array_type;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sArrayObjectRef : sGenericArrayObjectRef {

	sArrayObjectRef(int lineno, sExpr *array, sArrayType *array_type, sIndexValue *first_index)
	  : sGenericArrayObjectRef(lineno, array, array_type)
	{
		this->first_index = first_index;
	}
	sArrayObjectRef() : sGenericArrayObjectRef() {

}

	nIndexValue first_index;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sIndexValue : sPosNode {

	sIndexValue(int lineno, sIndexValue *next, sExpr *index)
	  : sPosNode(lineno)
	{
		this->next = next;
		this->index = index;
	}
	sIndexValue() : sPosNode() {

}

	nIndexValue next;
	nExpr index;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sSliceObjectRef : sGenericArrayObjectRef {

	sSliceObjectRef(int lineno, sExpr *array, sArrayType *array_type, sType *slice)
	  : sGenericArrayObjectRef(lineno, array, array_type)
	{
		this->slice = slice;
	}
	sSliceObjectRef() : sGenericArrayObjectRef() {

}

	nType slice;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sEnumLitRef : sPrimaryExpr {

	sEnumLitRef(int lineno, sEnumLiteral *value)
	  : sPrimaryExpr(lineno)
	{
		this->value = value;
	}
	sEnumLitRef() : sPrimaryExpr() {

}

	nEnumLiteral value;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sAmbgEnumLitRef : sPrimaryExpr {

	sAmbgEnumLitRef(int lineno)
	  : sPrimaryExpr(lineno)
	{
	}
	sAmbgEnumLitRef() : sPrimaryExpr() {

}

 	vaul_decl_set *set;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sAttrSignalRef : sObjectRef {

	sAttrSignalRef(int lineno, sType *attr_type, sObjectRef *signal)
	  : sObjectRef(lineno)
	{
		this->attr_type = attr_type;
		this->signal = signal;
	}
	sAttrSignalRef() : sObjectRef() {

}

	nType attr_type;
	nObjectRef signal;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sAttr_DELAYED : sAttrSignalRef {

	sAttr_DELAYED(int lineno, sType *attr_type, sObjectRef *signal, sExpr *delay)
	  : sAttrSignalRef(lineno, attr_type, signal)
	{
		this->delay = delay;
	}
	sAttr_DELAYED() : sAttrSignalRef() {

}

	nExpr delay;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sAttr_STABLE : sAttrSignalRef {

	sAttr_STABLE(int lineno, sType *attr_type, sObjectRef *signal, sExpr *duration)
	  : sAttrSignalRef(lineno, attr_type, signal)
	{
		this->duration = duration;
	}
	sAttr_STABLE() : sAttrSignalRef() {

}

	nExpr duration;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sAttr_QUIET : sAttrSignalRef {

	sAttr_QUIET(int lineno, sType *attr_type, sObjectRef *signal, sExpr *duration)
	  : sAttrSignalRef(lineno, attr_type, signal)
	{
		this->duration = duration;
	}
	sAttr_QUIET() : sAttrSignalRef() {

}

	nExpr duration;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sAttr_TRANSACTION : sAttrSignalRef {

	sAttr_TRANSACTION(int lineno, sType *attr_type, sObjectRef *signal)
	  : sAttrSignalRef(lineno, attr_type, signal)
	{
	}
	sAttr_TRANSACTION() : sAttrSignalRef() {

}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sAttrFunctionCall : sExpr {

	sAttrFunctionCall(int lineno, sType *attr_type)
	  : sExpr(lineno)
	{
		this->attr_type = attr_type;
	}
	sAttrFunctionCall() : sExpr() {

}

	nType attr_type;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sAttrSigFunc : sAttrFunctionCall {

	sAttrSigFunc(int lineno, sType *attr_type, sObjectRef *signal)
	  : sAttrFunctionCall(lineno, attr_type)
	{
		this->signal = signal;
	}
	sAttrSigFunc() : sAttrFunctionCall() {

}

	nObjectRef signal;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sAttr_EVENT : sAttrSigFunc {

	sAttr_EVENT(int lineno, sType *attr_type, sObjectRef *signal)
	  : sAttrSigFunc(lineno, attr_type, signal)
	{
	}
	sAttr_EVENT() : sAttrSigFunc() {

}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sAttr_ACTIVE : sAttrSigFunc {

	sAttr_ACTIVE(int lineno, sType *attr_type, sObjectRef *signal)
	  : sAttrSigFunc(lineno, attr_type, signal)
	{
	}
	sAttr_ACTIVE() : sAttrSigFunc() {

}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sAttr_LAST_EVENT : sAttrSigFunc {

	sAttr_LAST_EVENT(int lineno, sType *attr_type, sObjectRef *signal)
	  : sAttrSigFunc(lineno, attr_type, signal)
	{
	}
	sAttr_LAST_EVENT() : sAttrSigFunc() {

}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sAttr_LAST_ACTIVE : sAttrSigFunc {

	sAttr_LAST_ACTIVE(int lineno, sType *attr_type, sObjectRef *signal)
	  : sAttrSigFunc(lineno, attr_type, signal)
	{
	}
	sAttr_LAST_ACTIVE() : sAttrSigFunc() {

}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sAttr_LAST_VALUE : sAttrSigFunc {

	sAttr_LAST_VALUE(int lineno, sType *attr_type, sObjectRef *signal)
	  : sAttrSigFunc(lineno, attr_type, signal)
	{
	}
	sAttr_LAST_VALUE() : sAttrSigFunc() {

}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sAttr_DRIVING : sAttrSigFunc {

	sAttr_DRIVING(int lineno, sType *attr_type, sObjectRef *signal)
	  : sAttrSigFunc(lineno, attr_type, signal)
	{
	}
	sAttr_DRIVING() : sAttrSigFunc() {

}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sAttr_DRIVING_VALUE : sAttrSigFunc {

	sAttr_DRIVING_VALUE(int lineno, sType *attr_type, sObjectRef *signal)
	  : sAttrSigFunc(lineno, attr_type, signal)
	{
	}
	sAttr_DRIVING_VALUE() : sAttrSigFunc() {

}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sAttrTypeFunc : sAttrFunctionCall {

	sAttrTypeFunc(int lineno, sType *attr_type, sType *prefix, sExpr *argument)
	  : sAttrFunctionCall(lineno, attr_type)
	{
		this->prefix = prefix;
		this->argument = argument;
	}
	sAttrTypeFunc() : sAttrFunctionCall() {

}

	nType prefix;
	nExpr argument;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sAttr_LEFT : sAttrTypeFunc {

	sAttr_LEFT(int lineno, sType *attr_type, sType *prefix, sExpr *argument)
	  : sAttrTypeFunc(lineno, attr_type, prefix, argument)
	{
	}
	sAttr_LEFT() : sAttrTypeFunc() {

}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sAttr_RIGHT : sAttrTypeFunc {

	sAttr_RIGHT(int lineno, sType *attr_type, sType *prefix, sExpr *argument)
	  : sAttrTypeFunc(lineno, attr_type, prefix, argument)
	{
	}
	sAttr_RIGHT() : sAttrTypeFunc() {

}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sAttr_LOW : sAttrTypeFunc {

	sAttr_LOW(int lineno, sType *attr_type, sType *prefix, sExpr *argument)
	  : sAttrTypeFunc(lineno, attr_type, prefix, argument)
	{
	}
	sAttr_LOW() : sAttrTypeFunc() {

}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sAttr_HIGH : sAttrTypeFunc {

	sAttr_HIGH(int lineno, sType *attr_type, sType *prefix, sExpr *argument)
	  : sAttrTypeFunc(lineno, attr_type, prefix, argument)
	{
	}
	sAttr_HIGH() : sAttrTypeFunc() {

}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sAttr_ASCENDING : sAttrTypeFunc {

	sAttr_ASCENDING(int lineno, sType *attr_type, sType *prefix, sExpr *argument)
	  : sAttrTypeFunc(lineno, attr_type, prefix, argument)
	{
	}
	sAttr_ASCENDING() : sAttrTypeFunc() {

}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sAttr_IMAGE : sAttrTypeFunc {

	sAttr_IMAGE(int lineno, sType *attr_type, sType *prefix, sExpr *argument)
	  : sAttrTypeFunc(lineno, attr_type, prefix, argument)
	{
	}
	sAttr_IMAGE() : sAttrTypeFunc() {

}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sAttr_VALUE : sAttrTypeFunc {

	sAttr_VALUE(int lineno, sType *attr_type, sType *prefix, sExpr *argument)
	  : sAttrTypeFunc(lineno, attr_type, prefix, argument)
	{
	}
	sAttr_VALUE() : sAttrTypeFunc() {

}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sAttr_POS : sAttrTypeFunc {

	sAttr_POS(int lineno, sType *attr_type, sType *prefix, sExpr *argument)
	  : sAttrTypeFunc(lineno, attr_type, prefix, argument)
	{
	}
	sAttr_POS() : sAttrTypeFunc() {

}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sAttr_VAL : sAttrTypeFunc {

	sAttr_VAL(int lineno, sType *attr_type, sType *prefix, sExpr *argument)
	  : sAttrTypeFunc(lineno, attr_type, prefix, argument)
	{
	}
	sAttr_VAL() : sAttrTypeFunc() {

}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sAttr_SUCC : sAttrTypeFunc {

	sAttr_SUCC(int lineno, sType *attr_type, sType *prefix, sExpr *argument)
	  : sAttrTypeFunc(lineno, attr_type, prefix, argument)
	{
	}
	sAttr_SUCC() : sAttrTypeFunc() {

}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sAttr_PRED : sAttrTypeFunc {

	sAttr_PRED(int lineno, sType *attr_type, sType *prefix, sExpr *argument)
	  : sAttrTypeFunc(lineno, attr_type, prefix, argument)
	{
	}
	sAttr_PRED() : sAttrTypeFunc() {

}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sAttr_LEFTOF : sAttrTypeFunc {

	sAttr_LEFTOF(int lineno, sType *attr_type, sType *prefix, sExpr *argument)
	  : sAttrTypeFunc(lineno, attr_type, prefix, argument)
	{
	}
	sAttr_LEFTOF() : sAttrTypeFunc() {

}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sAttr_RIGHTOF : sAttrTypeFunc {

	sAttr_RIGHTOF(int lineno, sType *attr_type, sType *prefix, sExpr *argument)
	  : sAttrTypeFunc(lineno, attr_type, prefix, argument)
	{
	}
	sAttr_RIGHTOF() : sAttrTypeFunc() {

}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sArrayAttr : sExpr {

	sArrayAttr(int lineno, sType *attr_type, sObject *array, int index)
	  : sExpr(lineno)
	{
		this->attr_type = attr_type;
		this->array = array;
		this->index = index;
	}
	sArrayAttr() : sExpr() {

}

	nType attr_type;
	nObject array;
	int index;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sArrayAttr_LEFT : sArrayAttr {

	sArrayAttr_LEFT(int lineno, sType *attr_type, sObject *array, int index)
	  : sArrayAttr(lineno, attr_type, array, index)
	{
	}
	sArrayAttr_LEFT() : sArrayAttr() {

}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sArrayAttr_RIGHT : sArrayAttr {

	sArrayAttr_RIGHT(int lineno, sType *attr_type, sObject *array, int index)
	  : sArrayAttr(lineno, attr_type, array, index)
	{
	}
	sArrayAttr_RIGHT() : sArrayAttr() {

}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sArrayAttr_HIGH : sArrayAttr {

	sArrayAttr_HIGH(int lineno, sType *attr_type, sObject *array, int index)
	  : sArrayAttr(lineno, attr_type, array, index)
	{
	}
	sArrayAttr_HIGH() : sArrayAttr() {

}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sArrayAttr_LOW : sArrayAttr {

	sArrayAttr_LOW(int lineno, sType *attr_type, sObject *array, int index)
	  : sArrayAttr(lineno, attr_type, array, index)
	{
	}
	sArrayAttr_LOW() : sArrayAttr() {

}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sArrayAttr_ASCENDING : sArrayAttr {

	sArrayAttr_ASCENDING(int lineno, sType *attr_type, sObject *array, int index)
	  : sArrayAttr(lineno, attr_type, array, index)
	{
	}
	sArrayAttr_ASCENDING() : sArrayAttr() {

}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sArrayAttr_LENGTH : sArrayAttr {

	sArrayAttr_LENGTH(int lineno, sType *attr_type, sObject *array, int index)
	  : sArrayAttr(lineno, attr_type, array, index)
	{
	}
	sArrayAttr_LENGTH() : sArrayAttr() {

}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sAttributeSpec : sPosNode {

	sAttributeSpec(int lineno, Id attr_desig, sEntitySpec *entities, sExpr *value)
	  : sPosNode(lineno)
	{
		this->attr_desig = attr_desig;
		this->entities = entities;
		this->value = value;
	}
	sAttributeSpec() : sPosNode() {

}

	Id attr_desig;
	nEntitySpec entities;
	nExpr value;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sEntitySpec : sPosNode {

	sEntitySpec(int lineno, sEntityNameList *names, int entity_class)
	  : sPosNode(lineno)
	{
		this->names = names;
		this->entity_class = entity_class;
	}
	sEntitySpec() : sPosNode() {

}

	nEntityNameList names;
	int entity_class;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sEntityNameList : sPosNode {

	sEntityNameList(int lineno)
	  : sPosNode(lineno)
	{
	}
	sEntityNameList() : sPosNode() {

}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sEntityNameList_Ids : sEntityNameList {

	sEntityNameList_Ids(int lineno, sIdList *ids)
	  : sEntityNameList(lineno)
	{
		this->ids = ids;
	}
	sEntityNameList_Ids() : sEntityNameList() {

}

	nIdList ids;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sEntityNameList_ALL : sEntityNameList {

	sEntityNameList_ALL(int lineno)
	  : sEntityNameList(lineno)
	{
	}
	sEntityNameList_ALL() : sEntityNameList() {

}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sEntityNameList_OTHERS : sEntityNameList {

	sEntityNameList_OTHERS(int lineno)
	  : sEntityNameList(lineno)
	{
	}
	sEntityNameList_OTHERS() : sEntityNameList() {

}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sAggregate : sExpr {

	sAggregate(int lineno, sType *type)
	  : sExpr(lineno)
	{
		this->type = type;
	}
	sAggregate() : sExpr() {

}

	nType type;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sRecordAggregate : sAggregate {

	sRecordAggregate(int lineno, sType *type, sRecAggrAssoc *first_assoc)
	  : sAggregate(lineno, type)
	{
		this->first_assoc = first_assoc;
	}
	sRecordAggregate() : sAggregate() {

}

	nRecAggrAssoc first_assoc;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sRecAggrAssoc : sPosNode {

	sRecAggrAssoc(int lineno, sRecAggrAssoc *next, sRecordElement *elem, sExpr *actual)
	  : sPosNode(lineno)
	{
		this->next = next;
		this->elem = elem;
		this->actual = actual;
	}
	sRecAggrAssoc() : sPosNode() {

}

	nRecAggrAssoc next;
	nRecordElement elem;
	nExpr actual;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sArrayAggregate : sAggregate {

	sArrayAggregate(int lineno, sType *type, sArrAggrAssoc *first_assoc)
	  : sAggregate(lineno, type)
	{
		this->first_assoc = first_assoc;
	}
	sArrayAggregate() : sAggregate() {

}

	nArrAggrAssoc first_assoc;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sArrAggrAssoc : sPosNode {

	sArrAggrAssoc(int lineno, sArrAggrAssoc *next, sExpr *actual)
	  : sPosNode(lineno)
	{
		this->next = next;
		this->actual = actual;
	}
	sArrAggrAssoc() : sPosNode() {

}

	nArrAggrAssoc next;
	nExpr actual;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sSingleArrAggrAssoc : sArrAggrAssoc {

	sSingleArrAggrAssoc(int lineno, sArrAggrAssoc *next, sExpr *actual, sExpr *index)
	  : sArrAggrAssoc(lineno, next, actual)
	{
		this->index = index;
	}
	sSingleArrAggrAssoc() : sArrAggrAssoc() {

}

	nExpr index;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sRangeArrAggrAssoc : sArrAggrAssoc {

	sRangeArrAggrAssoc(int lineno, sArrAggrAssoc *next, sExpr *actual, sRange *range)
	  : sArrAggrAssoc(lineno, next, actual)
	{
		this->range = range;
	}
	sRangeArrAggrAssoc() : sArrAggrAssoc() {

}

	nRange range;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sSliceArrAggrAssoc : sRangeArrAggrAssoc {

	sSliceArrAggrAssoc(int lineno, sArrAggrAssoc *next, sExpr *actual, sRange *range)
	  : sRangeArrAggrAssoc(lineno, next, actual, range)
	{
	}
	sSliceArrAggrAssoc() : sRangeArrAggrAssoc() {

}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sOthersArrAggrAssoc : sArrAggrAssoc {

	sOthersArrAggrAssoc(int lineno, sArrAggrAssoc *next, sExpr *actual)
	  : sArrAggrAssoc(lineno, next, actual)
	{
	}
	sOthersArrAggrAssoc() : sArrAggrAssoc() {

}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sAmbgAggregate : sExpr {

	sAmbgAggregate(int lineno, sElemAssoc *first_assoc)
	  : sExpr(lineno)
	{
		this->first_assoc = first_assoc;
	}
	sAmbgAggregate() : sExpr() {

}

	nElemAssoc first_assoc;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sArtificialAmbgAggregate : sAmbgAggregate {

	sArtificialAmbgAggregate(int lineno, sElemAssoc *first_assoc)
	  : sAmbgAggregate(lineno, first_assoc)
	{
	}
	sArtificialAmbgAggregate() : sAmbgAggregate() {

}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sElemAssoc : sPosNode {

	sElemAssoc(int lineno, sElemAssoc *next, sChoice *first_choice, sExpr *actual)
	  : sPosNode(lineno)
	{
		this->next = next;
		this->first_choice = first_choice;
		this->actual = actual;
	}
	sElemAssoc() : sPosNode() {

}

	nElemAssoc next;
	nChoice first_choice;
	nExpr actual;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sChoice : sPosNode {

	sChoice(int lineno, sChoice *next)
	  : sPosNode(lineno)
	{
		this->next = next;
	}
	sChoice() : sPosNode() {

}

	nChoice next;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sExprChoice : sChoice {

	sExprChoice(int lineno, sChoice *next, sExpr *expr)
	  : sChoice(lineno, next)
	{
		this->expr = expr;
	}
	sExprChoice() : sChoice() {

}

	nExpr expr;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sRangeChoice : sChoice {

	sRangeChoice(int lineno, sChoice *next, sGenAssocElem *range)
	  : sChoice(lineno, next)
	{
		this->range = range;
		this->actual_is_slice = false;
	}
	sRangeChoice() : sChoice() {

}

	nGenAssocElem range;
	bool actual_is_slice;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sNamedChoice : sChoice {

	sNamedChoice(int lineno, sChoice *next, sName *name)
	  : sChoice(lineno, next)
	{
		this->name = name;
	}
	sNamedChoice() : sChoice() {

}

	nName name;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sOthersChoice : sChoice {

	sOthersChoice(int lineno, sChoice *next)
	  : sChoice(lineno, next)
	{
	}
	sOthersChoice() : sChoice() {

}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sLabel : sDeclaration {

	sLabel(int lineno, Id id, sStatement *stat)
	  : sDeclaration(lineno, id)
	{
		this->stat = stat;
	}
	sLabel() : sDeclaration() {

}

	nStatement stat;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sStatement : sPosNode {

	sStatement(int lineno)
	  : sPosNode(lineno)
	{
		this->label = NULL;
		this->next = NULL;
	}
	sStatement() : sPosNode() {

}

	nLabel label;
	nStatement next;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sDummyStat : sStatement {

	sDummyStat(int lineno)
	  : sStatement(lineno)
	{
	}
	sDummyStat() : sStatement() {

}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sReturnStat : sStatement {

	sReturnStat(int lineno, sExpr *value)
	  : sStatement(lineno)
	{
		this->value = value;
	}
	sReturnStat() : sStatement() {

}

	nExpr value;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sVarAssignment : sStatement {

	sVarAssignment(int lineno, sExpr *target, sExpr *value)
	  : sStatement(lineno)
	{
		this->target = target;
		this->value = value;
	}
	sVarAssignment() : sStatement() {

}

	nExpr target;
	nExpr value;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sIfStat : sStatement {

	sIfStat(int lineno, sExpr *cond, sStatement *then_stats, sStatement *else_stats)
	  : sStatement(lineno)
	{
		this->cond = cond;
		this->then_stats = then_stats;
		this->else_stats = else_stats;
	}
	sIfStat() : sStatement() {

}

	nExpr cond;
	nStatement then_stats;
	nStatement else_stats;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sCaseStat : sStatement {

	sCaseStat(int lineno, sExpr *switch_expr, sCaseAlternative *first_alternative)
	  : sStatement(lineno)
	{
		this->switch_expr = switch_expr;
		this->first_alternative = first_alternative;
	}
	sCaseStat() : sStatement() {

}

	nExpr switch_expr;
	nCaseAlternative first_alternative;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sCaseAlternative : sPosNode {

	sCaseAlternative(int lineno, sCaseAlternative *next, sChoice *first_choice, sStatement *stats)
	  : sPosNode(lineno)
	{
		this->next = next;
		this->first_choice = first_choice;
		this->stats = stats;
	}
	sCaseAlternative() : sPosNode() {

}

	nCaseAlternative next;
	nChoice first_choice;
	nStatement stats;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sLoopStat : sStatement {

	sLoopStat(int lineno, sLoopScope *scope, sIterationScheme *iteration_scheme, sStatement *stats)
	  : sStatement(lineno)
	{
		this->scope = scope;
		this->iteration_scheme = iteration_scheme;
		this->stats = stats;
	}
	sLoopStat() : sStatement() {

}

	nLoopScope scope;
	nIterationScheme iteration_scheme;
	nStatement stats;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sLoopScope : sScope {

	sLoopScope(int lineno, Id id, sLoopStat *loop)
	  : sScope(lineno, id)
	{
		this->loop = loop;
	}
	sLoopScope() : sScope() {

}

	nLoopStat loop;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sIterationScheme : sPosNode {

	sIterationScheme(int lineno)
	  : sPosNode(lineno)
	{
	}
	sIterationScheme() : sPosNode() {

}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sWhileScheme : sIterationScheme {

	sWhileScheme(int lineno, sExpr *condition)
	  : sIterationScheme(lineno)
	{
		this->condition = condition;
	}
	sWhileScheme() : sIterationScheme() {

}

	nExpr condition;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sForScheme : sIterationScheme {

	sForScheme(int lineno, sObject *var, sRange *range)
	  : sIterationScheme(lineno)
	{
		this->var = var;
		this->range = range;
	}
	sForScheme() : sIterationScheme() {

}

	nObject var;
	nRange range;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sPreForScheme : sIterationScheme {

	sPreForScheme(int lineno, Id var, sPreIndexConstraint *range)
	  : sIterationScheme(lineno)
	{
		this->var = var;
		this->range = range;
	}
	sPreForScheme() : sIterationScheme() {

}

	Id var;
	nPreIndexConstraint range;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sLoopControlStat : sStatement {

	sLoopControlStat(int lineno, sLoopStat *loop, sExpr *when)
	  : sStatement(lineno)
	{
		this->loop = loop;
		this->when = when;
	}
	sLoopControlStat() : sStatement() {

}

	nLoopStat loop;
	nExpr when;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sNextStat : sLoopControlStat {

	sNextStat(int lineno, sLoopStat *loop, sExpr *when)
	  : sLoopControlStat(lineno, loop, when)
	{
	}
	sNextStat() : sLoopControlStat() {

}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sExitStat : sLoopControlStat {

	sExitStat(int lineno, sLoopStat *loop, sExpr *when)
	  : sLoopControlStat(lineno, loop, when)
	{
	}
	sExitStat() : sLoopControlStat() {

}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sNullStat : sStatement {

	sNullStat(int lineno)
	  : sStatement(lineno)
	{
	}
	sNullStat() : sStatement() {

}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sProcedureCallStat : sStatement {

	sProcedureCallStat(int lineno, sProcedure *proc, sAssociation *first_actual)
	  : sStatement(lineno)
	{
		this->proc = proc;
		this->first_actual = first_actual;
	}
	sProcedureCallStat() : sStatement() {

}

	nProcedure proc;
	nAssociation first_actual;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sWaitStat : sStatement {

	sWaitStat(int lineno, sSignalList *first_sensitivity, sExpr *condition, sExpr *timeout)
	  : sStatement(lineno)
	{
		this->first_sensitivity = first_sensitivity;
		this->condition = condition;
		this->timeout = timeout;
	}
	sWaitStat() : sStatement() {

}

	nSignalList first_sensitivity;
	nExpr condition;
	nExpr timeout;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sSignalList : sNode {

	sSignalList(sObjectRef *signal, sSignalList *next)
	  : sNode()
	{
		this->signal = signal;
		this->next = next;
	}
	sSignalList() : sNode() {

}

	nObjectRef signal;
	nSignalList next;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sWaveformElement : sNode {

	sWaveformElement(sExpr *value, sExpr *after, sWaveformElement *next)
	  : sNode()
	{
		this->value = value;
		this->after = after;
		this->next = next;
	}
	sWaveformElement() : sNode() {

}

	nExpr value;
	nExpr after;
	nWaveformElement next;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sSignalAssignment : sStatement {

	sSignalAssignment(int lineno, sExpr *target, bool transport, sWaveformElement *first_wave)
	  : sStatement(lineno)
	{
		this->target = target;
		this->transport = transport;
		this->first_wave = first_wave;
	}
	sSignalAssignment() : sStatement() {

}

	nExpr target;
	bool transport;
	nWaveformElement first_wave;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sAssertStat : sStatement {

	sAssertStat(int lineno, sExpr *condition, sExpr *report, sExpr *severity)
	  : sStatement(lineno)
	{
		this->condition = condition;
		this->report = report;
		this->severity = severity;
	}
	sAssertStat() : sStatement() {

}

	nExpr condition;
	nExpr report;
	nExpr severity;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sConcurrentStatement : sScope {

	sConcurrentStatement(int lineno, Id id)
	  : sScope(lineno, id)
	{
		this->next_stat = NULL;
	}
	sConcurrentStatement() : sScope() {

}

	nConcurrentStatement next_stat;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sProcess : sConcurrentStatement {

	sProcess(int lineno, Id id, sSignalList *sensitivities, sStatement *stats)
	  : sConcurrentStatement(lineno, id)
	{
		this->sensitivities = sensitivities;
		this->stats = stats;
	}
	sProcess() : sConcurrentStatement() {

}

	nSignalList sensitivities;
	nStatement stats;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sCondalWaveform : sPosNode {

	sCondalWaveform(int lineno, sWaveformElement *wave, sExpr *condition)
	  : sPosNode(lineno)
	{
		this->wave = wave;
		this->condition = condition;
		this->else_wave = NULL;
	}
	sCondalWaveform() : sPosNode() {

}

	nWaveformElement wave;
	nExpr condition;
	nCondalWaveform else_wave;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sCondalSignalAssign : sPosNode {

	sCondalSignalAssign(int lineno, sExpr *target, bool transport, sCondalWaveform *wave)
	  : sPosNode(lineno)
	{
		this->target = target;
		this->transport = transport;
		this->wave = wave;
	}
	sCondalSignalAssign() : sPosNode() {

}

	nExpr target;
	bool transport;
	nCondalWaveform wave;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sComponent : sConcurrentStatement {

	sComponent(int lineno, Id id)
	  : sConcurrentStatement(lineno, id)
	{
		this->first_generic = NULL;
		this->first_port = NULL;
	}
	sComponent() : sConcurrentStatement() {

}

	nInterface first_generic;
	nInterface first_port;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sBlock : sComponent {

	sBlock(int lineno, Id id)
	  : sComponent(lineno, id)
	{
		this->stats = NULL;
		this->specs = NULL;
	}
	sBlock() : sComponent() {

}

	nConcurrentStatement stats;
	nConfigSpec specs;
 	void add_spec(nConfigSpec);

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sEntity : sBlock {

	sEntity(int lineno, Id id)
	  : sBlock(lineno, id)
	{
	}
	sEntity() : sBlock() {

}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sArchitecture : sBlock {

	sArchitecture(int lineno, Id id)
	  : sBlock(lineno, id)
	{
	}
	sArchitecture() : sBlock() {

}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sBlockStat : sBlock {

	sBlockStat(int lineno, Id id)
	  : sBlock(lineno, id)
	{
		this->binding = NULL;
	}
	sBlockStat() : sBlock() {

}

	nBindingIndic binding;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sComponentInst : sConcurrentStatement {

	sComponentInst(int lineno, Id id, sBindingIndic *binding)
	  : sConcurrentStatement(lineno, id)
	{
		this->binding = binding;
		this->config = NULL;
	}
	sComponentInst() : sConcurrentStatement() {

}

	nBindingIndic binding;
	nConfigSpec config;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sConfigSpec : sPosNode {

	sConfigSpec(int lineno, sComponentSpec *comps, sBindingIndic *binding)
	  : sPosNode(lineno)
	{
		this->comps = comps;
		this->binding = binding;
		this->next = NULL;
	}
	sConfigSpec() : sPosNode() {

}

	nComponentSpec comps;
	nBindingIndic binding;
	nConfigSpec next;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sComponentSpec : sNode {

	sComponentSpec(sInstList *ids, sComponent *comp)
	  : sNode()
	{
		this->ids = ids;
		this->comp = comp;
	}
	sComponentSpec() : sNode() {

}

	nInstList ids;
	nComponent comp;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sInstList : sPosNode {

	sInstList(int lineno)
	  : sPosNode(lineno)
	{
	}
	sInstList() : sPosNode() {

}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sInstList_Ids : sInstList {

	sInstList_Ids(int lineno, sIdList *ids)
	  : sInstList(lineno)
	{
		this->ids = ids;
	}
	sInstList_Ids() : sInstList() {

}

	nIdList ids;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sInstList_ALL : sInstList {

	sInstList_ALL(int lineno)
	  : sInstList(lineno)
	{
	}
	sInstList_ALL() : sInstList() {

}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sInstList_OTHERS : sInstList {

	sInstList_OTHERS(int lineno)
	  : sInstList(lineno)
	{
	}
	sInstList_OTHERS() : sInstList() {

}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sBindingIndic : sNode {

	sBindingIndic(sComponent *unit, sAssociation *generic_assoc, sAssociation *port_assoc)
	  : sNode()
	{
		this->unit = unit;
		this->generic_assoc = generic_assoc;
		this->port_assoc = port_assoc;
	}
	sBindingIndic() : sNode() {

}

	nComponent unit;
	nAssociation generic_assoc;
	nAssociation port_assoc;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sIncrementalBindingIndic : sNode {

	sIncrementalBindingIndic(sComponent *unit, sNamedAssocElem *generic_assoc, sNamedAssocElem *port_assoc)
	  : sNode()
	{
		this->unit = unit;
		this->generic_assoc = generic_assoc;
		this->port_assoc = port_assoc;
	}
	sIncrementalBindingIndic() : sNode() {

}

	nComponent unit;
	nNamedAssocElem generic_assoc;
	nNamedAssocElem port_assoc;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sConfiguration : sComponent {

	sConfiguration(int lineno, Id id, sEntity *entity)
	  : sComponent(lineno, id)
	{
		this->entity = entity;
		this->config = NULL;
	}
	sConfiguration() : sComponent() {

}

	nEntity entity;
	nBlockConfig config;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sBaseConfig : sScope {

	sBaseConfig(int lineno, Id id)
	  : sScope(lineno, id)
	{
		this->next_config = NULL;
	}
	sBaseConfig() : sScope() {

}

	nBaseConfig next_config;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sBlockConfig : sBaseConfig {

	sBlockConfig(int lineno, Id id, sBlock *block)
	  : sBaseConfig(lineno, id)
	{
		this->block = block;
		this->configs = NULL;
	}
	sBlockConfig() : sBaseConfig() {

}

	nBlock block;
	nBaseConfig configs;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sCompConfig : sBaseConfig {

	sCompConfig(int lineno, Id id, sComponentSpec *comp_spec, sCompInstList *comps, sBindingIndic *binding)
	  : sBaseConfig(lineno, id)
	{
		this->comp_spec = comp_spec;
		this->comps = comps;
		this->binding = binding;
		this->config = NULL;
	}
	sCompConfig() : sBaseConfig() {

}

	nComponentSpec comp_spec;
	nCompInstList comps;
	nBindingIndic binding;
	nBlockConfig config;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sCompInstList : sNode {

	sCompInstList(sCompInstList *link, sComponentInst *inst)
	  : sNode()
	{
		this->link = link;
		this->inst = inst;
	}
	sCompInstList() : sNode() {

}

	nCompInstList link;
	nComponentInst inst;

	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sDummy : sNode {

	sDummy()
	  : sNode()
	{
	}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

struct sAssociationList : sDummy {

	sAssociationList()
	  : sDummy()
	{
	}


	virtual nKind kind();

	virtual void print(graph_printer *pr);
	virtual void visit(graph_visit_f *pre,
		graph_visit_f *post, graph_visitor *);
	virtual void write(graph_writer *gr);
	virtual void read(graph_reader *gr);
};

nIdList reverse(nIdList &);
nSelNameList reverse(nSelNameList &);
nDeclaration reverse(nDeclaration &);
nAttributeValue reverse(nAttributeValue &);
nScope reverse(nScope &);
nPreIndexConstraint reverse(nPreIndexConstraint &);
nIndexConstraint reverse(nIndexConstraint &);
nPhysicalUnit reverse(nPhysicalUnit &);
nEnumLiteral reverse(nEnumLiteral &);
nIndexType reverse(nIndexType &);
nRecordElement reverse(nRecordElement &);
nInterface reverse(nInterface &);
nGenAssocElem reverse(nGenAssocElem &);
nAssociation reverse(nAssociation &);
nIndexValue reverse(nIndexValue &);
nRecAggrAssoc reverse(nRecAggrAssoc &);
nArrAggrAssoc reverse(nArrAggrAssoc &);
nElemAssoc reverse(nElemAssoc &);
nChoice reverse(nChoice &);
nStatement reverse(nStatement &);
nCaseAlternative reverse(nCaseAlternative &);
nSignalList reverse(nSignalList &);
nWaveformElement reverse(nWaveformElement &);
nConcurrentStatement reverse(nConcurrentStatement &);
nCondalWaveform reverse(nCondalWaveform &);
nConfigSpec reverse(nConfigSpec &);
nBaseConfig reverse(nBaseConfig &);
nCompInstList reverse(nCompInstList &);

union nTree {
	nNode Node;
	nPosNode PosNode;
	nIdList IdList;
	nName Name;
	nSimpleName SimpleName;
	nOpSymbol OpSymbol;
	nSelName SelName;
	nIftsName IftsName;
	nAttributeName AttributeName;
	nSelNameList SelNameList;
	nDeclaration Declaration;
	nAttributedDeclaration AttributedDeclaration;
	nAttribute Attribute;
	nAttributeValue AttributeValue;
	nScope Scope;
	nTopScope TopScope;
	nLibNameDecl LibNameDecl;
	nIndirectDecl IndirectDecl;
	nPackage Package;
	nStandardPackage StandardPackage;
	nPackageBody PackageBody;
	nType Type;
	nConstraint Constraint;
	nRange Range;
	nExplicitRange ExplicitRange;
	nArrayRange ArrayRange;
	nArrayAttr_RANGE ArrayAttr_RANGE;
	nArrayAttr_REVERSE_RANGE ArrayAttr_REVERSE_RANGE;
	nPreIndexConstraint PreIndexConstraint;
	nPreIndexRangeConstraint PreIndexRangeConstraint;
	nPreIndexSubtypeConstraint PreIndexSubtypeConstraint;
	nIndexConstraint IndexConstraint;
	nIncompleteType IncompleteType;
	nSubType SubType;
	nAccessType AccessType;
	nScalarType ScalarType;
	nNumericType NumericType;
	nIntegerType IntegerType;
	nFloatingType FloatingType;
	nUniversalInteger UniversalInteger;
	nUniversalReal UniversalReal;
	nPhysicalType PhysicalType;
	nPhysicalUnit PhysicalUnit;
	nEnumType EnumType;
	nEnumLiteral EnumLiteral;
	nCompositeType CompositeType;
	nArrayType ArrayType;
	nIndexType IndexType;
	nSubarrayType SubarrayType;
	nRecordType RecordType;
	nRecordElement RecordElement;
	nFileType FileType;
	nDummyType DummyType;
	nObject Object;
	nVariable Variable;
	nConstant Constant;
	nConstantBody ConstantBody;
	nSignal Signal;
	nGuardSignal GuardSignal;
	nAlias Alias;
	nFile File;
	nSubprogram Subprogram;
	nProcedure Procedure;
	nFunction Function;
	nPredefOp PredefOp;
	nInterface Interface;
	nSubprogramBody SubprogramBody;
	nExpr Expr;
	nUnresolvedName UnresolvedName;
	nFunctionCall FunctionCall;
	nProcedureCall ProcedureCall;
	nAmbgCall AmbgCall;
	nGenAssocElem GenAssocElem;
	nNamedAssocElem NamedAssocElem;
	nSubtypeAssocElem SubtypeAssocElem;
	nRangeAssocElem RangeAssocElem;
	nAssociation Association;
	nTypeConversion TypeConversion;
	nQualifiedExpr QualifiedExpr;
	nNewExpr NewExpr;
	nPrimaryExpr PrimaryExpr;
	nOpenExpr OpenExpr;
	nLiteralRef LiteralRef;
	nAmbgArrayLitRef AmbgArrayLitRef;
	nArrayLiteralRef ArrayLiteralRef;
	nPhysicalLiteralRef PhysicalLiteralRef;
	nAmbgNullExpr AmbgNullExpr;
	nNullExpr NullExpr;
	nObjectRef ObjectRef;
	nSimpleObjectRef SimpleObjectRef;
	nAccessObjectRef AccessObjectRef;
	nRecordObjectRef RecordObjectRef;
	nGenericArrayObjectRef GenericArrayObjectRef;
	nArrayObjectRef ArrayObjectRef;
	nIndexValue IndexValue;
	nSliceObjectRef SliceObjectRef;
	nEnumLitRef EnumLitRef;
	nAmbgEnumLitRef AmbgEnumLitRef;
	nAttrSignalRef AttrSignalRef;
	nAttr_DELAYED Attr_DELAYED;
	nAttr_STABLE Attr_STABLE;
	nAttr_QUIET Attr_QUIET;
	nAttr_TRANSACTION Attr_TRANSACTION;
	nAttrFunctionCall AttrFunctionCall;
	nAttrSigFunc AttrSigFunc;
	nAttr_EVENT Attr_EVENT;
	nAttr_ACTIVE Attr_ACTIVE;
	nAttr_LAST_EVENT Attr_LAST_EVENT;
	nAttr_LAST_ACTIVE Attr_LAST_ACTIVE;
	nAttr_LAST_VALUE Attr_LAST_VALUE;
	nAttr_DRIVING Attr_DRIVING;
	nAttr_DRIVING_VALUE Attr_DRIVING_VALUE;
	nAttrTypeFunc AttrTypeFunc;
	nAttr_LEFT Attr_LEFT;
	nAttr_RIGHT Attr_RIGHT;
	nAttr_LOW Attr_LOW;
	nAttr_HIGH Attr_HIGH;
	nAttr_ASCENDING Attr_ASCENDING;
	nAttr_IMAGE Attr_IMAGE;
	nAttr_VALUE Attr_VALUE;
	nAttr_POS Attr_POS;
	nAttr_VAL Attr_VAL;
	nAttr_SUCC Attr_SUCC;
	nAttr_PRED Attr_PRED;
	nAttr_LEFTOF Attr_LEFTOF;
	nAttr_RIGHTOF Attr_RIGHTOF;
	nArrayAttr ArrayAttr;
	nArrayAttr_LEFT ArrayAttr_LEFT;
	nArrayAttr_RIGHT ArrayAttr_RIGHT;
	nArrayAttr_HIGH ArrayAttr_HIGH;
	nArrayAttr_LOW ArrayAttr_LOW;
	nArrayAttr_ASCENDING ArrayAttr_ASCENDING;
	nArrayAttr_LENGTH ArrayAttr_LENGTH;
	nAttributeSpec AttributeSpec;
	nEntitySpec EntitySpec;
	nEntityNameList EntityNameList;
	nEntityNameList_Ids EntityNameList_Ids;
	nEntityNameList_ALL EntityNameList_ALL;
	nEntityNameList_OTHERS EntityNameList_OTHERS;
	nAggregate Aggregate;
	nRecordAggregate RecordAggregate;
	nRecAggrAssoc RecAggrAssoc;
	nArrayAggregate ArrayAggregate;
	nArrAggrAssoc ArrAggrAssoc;
	nSingleArrAggrAssoc SingleArrAggrAssoc;
	nRangeArrAggrAssoc RangeArrAggrAssoc;
	nSliceArrAggrAssoc SliceArrAggrAssoc;
	nOthersArrAggrAssoc OthersArrAggrAssoc;
	nAmbgAggregate AmbgAggregate;
	nArtificialAmbgAggregate ArtificialAmbgAggregate;
	nElemAssoc ElemAssoc;
	nChoice Choice;
	nExprChoice ExprChoice;
	nRangeChoice RangeChoice;
	nNamedChoice NamedChoice;
	nOthersChoice OthersChoice;
	nLabel Label;
	nStatement Statement;
	nDummyStat DummyStat;
	nReturnStat ReturnStat;
	nVarAssignment VarAssignment;
	nIfStat IfStat;
	nCaseStat CaseStat;
	nCaseAlternative CaseAlternative;
	nLoopStat LoopStat;
	nLoopScope LoopScope;
	nIterationScheme IterationScheme;
	nWhileScheme WhileScheme;
	nForScheme ForScheme;
	nPreForScheme PreForScheme;
	nLoopControlStat LoopControlStat;
	nNextStat NextStat;
	nExitStat ExitStat;
	nNullStat NullStat;
	nProcedureCallStat ProcedureCallStat;
	nWaitStat WaitStat;
	nSignalList SignalList;
	nWaveformElement WaveformElement;
	nSignalAssignment SignalAssignment;
	nAssertStat AssertStat;
	nConcurrentStatement ConcurrentStatement;
	nProcess Process;
	nCondalWaveform CondalWaveform;
	nCondalSignalAssign CondalSignalAssign;
	nComponent Component;
	nBlock Block;
	nEntity Entity;
	nArchitecture Architecture;
	nBlockStat BlockStat;
	nComponentInst ComponentInst;
	nConfigSpec ConfigSpec;
	nComponentSpec ComponentSpec;
	nInstList InstList;
	nInstList_Ids InstList_Ids;
	nInstList_ALL InstList_ALL;
	nInstList_OTHERS InstList_OTHERS;
	nBindingIndic BindingIndic;
	nIncrementalBindingIndic IncrementalBindingIndic;
	nConfiguration Configuration;
	nBaseConfig BaseConfig;
	nBlockConfig BlockConfig;
	nCompConfig CompConfig;
	nCompInstList CompInstList;
	nDummy Dummy;
	nAssociationList AssociationList;
};


struct vaul_graph_writer : graph_writer, vaul_error_source {

    vaul_graph_writer(vaul_design_unit *, vaul_binfile *);
    ~vaul_graph_writer();

    vaul_binfile *file;
   
private:
    vaul_design_unit *unit_owner;

    void write_kind(nKind k);
    void write_xref(nNode);
    void write_label(int);
};

struct vaul_graph_reader : graph_reader, vaul_error_source {

    vaul_graph_reader(vaul_design_unit *, vaul_binfile *, vaul_libpool *);
    ~vaul_graph_reader();

    vaul_binfile *file;
    vaul_mempool pool;

private:
    vaul_design_unit *unit_owner;
    vaul_libpool *libs;

    nKind read_kind();
    nNode read_xref();
    int read_label();
    void read_error(nKind read, nKind expected);
}; 

struct vaul_graph_printer : graph_printer {

    vaul_graph_printer(vaul_design_unit *);
    ~vaul_graph_printer();

private:
    void print_xref(nNode);
};

#if 0
struct vaul_graph_runner : graph_runner, vaul_error_source {

    vaul_graph_runner();
    ~vaul_graph_runner();

    void reset(nNode head);
    void mark(nNode);
    bool marked(nNode);
    bool same_unit(nNode n);

    void write_kind(nKind k);
    void read_kind(nKind &k);

    void print_xref(nNode);
    void write_xref(nNode);
    void read_xref(nNode &);

    void read_error(nKind, nKind);

    vaul_binfile *file;
    vaul_mempool pool;

    vaul_design_unit *unit_owner;
    vaul_libpool *libs;

    nNode found;
    int label;

    void *closure;

    vaul_design_unit *seen[20];
    int n_seen;
};
#endif

struct vaul_node_printer : vaul_std_printer {

protected:
    virtual void format_out(FILE *, const char *spec, int len, va_list &ap);
    virtual char *get_source(nNode);

    void print_node(FILE *f, nNode n, int arg);
};

#endif // VAUL_TREE_H


