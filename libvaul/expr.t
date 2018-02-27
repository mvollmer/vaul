/* expressions -*- tex -*-

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

[ \section{Ausdrücke}
]

node Expr : PosNode 
[ Die Basis für alle Ausdrucksknoten.
]
{
};

node UnresolvedName : Expr 
[%i;]
{
    Name name;
};

node FunctionCall : Expr 
[ Ein Funktionsaufruf.
]
{
    Function func;
	[ Die aufgerufene Funktion. ]
    Association first_actual;
	[ Ein Ausdruck für jeden Parameter der Funktion. Diese
          Ausdrücke werden nicht in der Reihenfolge der Parameter der Funktion
          aufgelistet, sondern in der Reihenfolge, in der sie im VHDL-Text
          stehen. ]
};

node ProcedureCall : Expr 
[%i;]
{
    Procedure proc;
    Association first_actual;
};

node AmbgCall : Expr 
[%i;]
{
    NamedAssocElem first_actual;
    extra { vaul_decl_set *set; }
};

node GenAssocElem : PosNode 
[%i;]
{
    GenAssocElem next;
};

node NamedAssocElem : GenAssocElem 
[%i;]
{
    Name formal;
    Expr actual;
    extra {
	vaul_decl_set *ifts_decls;   // The decls of the ifts prefix
				     // of the formal
	nKind ifts_kind;             // The basic kind of the ifts_decls
	nSimpleName ifts_arg_name;   // The simple name of the only ifts
				     // argument
    }
};

node SubtypeAssocElem : GenAssocElem 
[%i;]
{
    Type type;
};

node RangeAssocElem : GenAssocElem 
[%i;]
{
    Range range;
};

node Association : Node 
[ Eine Verbindung zwischen einem Schnittstellenobjekt und einem Ausdruck.
]
{
    Association next;
    Interface formal;
	[ Die Deklaration des Schnittstellenobjekts. ]
    Declaration formal_conversion;
	[ Noch nicht realisiert. ]
    Expr	actual;
	[ Der Ausdruck, der <formal> zugeordnet wird. ]
    Declaration actual_conversion;
	[ noch nicht realisiert. ]
};

node TypeConversion : Expr 
[ Eine explizite Typumwandlung. ]
{
    Type target_type;
	[ Der Typ, in dem umgewandelt werden soll.]
    Expr expression;
	[ Der Ausdruck, der umgewandelt werden soll.
	  Der Typ dieses Ausdrucks und <target_type> sind
	  <closely related>. Eine Umwandlung ist also möglich.
	  (Wird noch nicht überprüft)
	]
};

node QualifiedExpr : Expr
[ Ein freundlicher und zutreffender Hinweis auf den Typ eines Ausdrucks. ]
{
    Type type;
	[ Der Typ von <expression>. ]
    Expr expression;
	[ Dieser Ausdruck hat den Typ <type>. ]
};

node NewExpr : Expr
{
    Type type;
    Expr initial_value;
};

node PrimaryExpr : Expr 
[ Die Basis für alle Ausdrücke, die kein Funktionsaufruf sind
 (Blätter des Ausdruckbaums).
]
{
};

node OpenExpr : PrimaryExpr
{
};

[\begin{ndesc}{typedef char *\b{Literal}}{}
   Der Typ eines <abstract literals>. Der Parser selbst wandelt diese
   Konstanten nicht in numerische Typen (z.B. "long" oder "double"), um das
   Backend nicht auf eine Representation festzunageln.
 \end{ndesc}
]

node LiteralRef : PrimaryExpr
[ Eine 'literarische' Konstante. ]
{
    attr Literal value;
	[ Der Wert der Konstante als String. ]
};

node AmbgArrayLitRef : PrimaryExpr 
[%i;]
{
    attr Literal value;
};

node ArrayLiteralRef : PrimaryExpr 
[ Eine 'literarische' Arraykonstante (<bit string literal>). ]
{
    attr Literal value;
	[ Der Wert der Konstanten als String. ]
    Type type;
	[ Zusätzlich zum Wert wird noch der Typ der Arraykonstanten angegeben.
	  Dieser Typ beinhaltet in einem <SubType> auch die Ausdehnung dieses
	  Arrays. ]
};

node PhysicalLiteralRef : LiteralRef 
[ Eine physikalische Konstante. ]
{
    PhysicalUnit unit;
	[ Die Einheit dieser Konstanten. Der Zahlenwert steht
	  <value> der <LiteralRef>-Basis. ]
};

node AmbgNullExpr : PrimaryExpr 
[%1;]
{
};

node NullExpr : PrimaryExpr
{
    Type type;
};
 
node ObjectRef : PrimaryExpr 
[ Die Basis für Zugriffe auf <Object>s.
  Ein <ObjectRef> stellt einige Funktionen zur Verfügung, die von
  den konkreten Zugriffen (auf Variablen, Arrayelemente, \dots)
  passend implementiert werden.
]
{

    extra {
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
    }
	[\begin{desc}{virtual ObjectClass \b{get_class}();}
	  Ermittelt die Klasse des Objekts, auf das zugegriffen wird.
	 \end{desc}
	 \begin{desc}{virtual ObjectClass \b{get_mode}();}
	  Ermittelt den Modus.
	 \end{desc}
	 \begin{desc}{virtual ObjectClass \b{get_type}();}
	  Ermittelt den Typ.
	 \end{desc}
	 \begin{desc}{virtual ObjectClass \b{is_constant}();}
	   Äquivalent zu "get_class() == ObjClass_Constant".
	 \end{desc}
	 \begin{desc}{virtual ObjectClass \b{is_variable}();}
	   Äquivalent zu "get_class() == ObjClass_Variable".
	 \end{desc}
	 \begin{desc}{virtual ObjectClass \b{is_signal}();}
	   Äquivalent zu "get_class() == ObjClass_Signal".
	 \end{desc}
	 \begin{desc}{virtual ObjectClass \b{is_file}();}
	   Äquivalent zu "get_class() == ObjClass_File".
	 \end{desc}
	 \begin{desc}{virtual ObjectClass \b{is_readable}();}
	  Ermittelt, ob das Objekt gelesen werden kann.
	 \end{desc}
	 \begin{desc}{virtual ObjectClass \b{is_writeable}();}
	  Ermittelt, ob das Objekt geschrieben werden kann.
	 \end{desc}
	]
};

node SimpleObjectRef : ObjectRef 
[ Ein Zugriff auf ein 'normales' Objekt (kein Array- oder Recordelement).
]
{
    Object object;
	[ Das Objekt höchstselbst. ]
    Name name;
	[ Der Name, der im VHDL-Text stand. ]
};

node AccessObjectRef : ObjectRef
[ Ein Zugriff auf ein Objekt durch ein <access value>.
]
{
    Expr access;
        [ Ein Ausdruck für den <acces value>. ]
    Type accessed_type;
};

node RecordObjectRef : ObjectRef 
[ Ein Zugriff auf ein Recordelement.
]
{
    Expr record;
	[ Das Recordobjekt als ganzes. ]
    RecordType record_type;
	[ Der Typ von <record>. ]
    RecordElement element;
	[ Das Element des Records, auf das zugegriffen wird. ]
};

node GenericArrayObjectRef : ObjectRef 
[%4; Die Basis für einen Zugriff auf einen Teil eines Arrays.
]
{
    Expr array;
	[ Das Array als ganzes. ]
    ArrayType array_type;
	[ Der Typ dieses Arrays. Er könnte theoretisch auch
          aus <array> bestimmt werden. Das ist aber nicht ganz trivial.
	]
};

node ArrayObjectRef : GenericArrayObjectRef 
[%3; Ein Zugriff auf ein Arrayelement.
]
{
    IndexValue first_index;
	[ Ein Wert für jeden Index des Arrays. Die Liste ist parallel zur
	  Indexliste von <array_type>.
	]
};

node IndexValue : PosNode
{
    IndexValue next;
    Expr index;
};

node SliceObjectRef : GenericArrayObjectRef
[%3; Ein Zugriff auf eine Scheibe eines Arrays.
]
{
    Type slice;
	[ Der <Range> dieses Subtypes gibt den Bereich der
	  Scheibe an.
	]
};

node EnumLitRef : PrimaryExpr 
[ Ein Zugriff aif eine Enumerationskonstante. ]
{
    EnumLiteral value;
	[ Die Konstante selbst. ]
};

node AmbgEnumLitRef : PrimaryExpr 
[%i;]
{
    extra { vaul_decl_set *set; }
};

include attr.t
include aggr.t
