/* declarations and names

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

[ \section{Namen}

\begin{ndesc}{"typedef char *"\b{Id};}{}
Alle Attribute die einen einfachen Namen bedeuten, haben diesen Typ.
\end{ndesc}
]

// Identifier und Namen

node IdList : PosNode
[ <IdList>s sind eine einfache Auflistung von <Id>s. Die Reihenfolge
  der <Id>s in der Liste entspricht nicht unbeding der Reihenfolge im
  VHDL Text. ]
{
    attr Id id;
    IdList link;
};

node Name : PosNode 
[%i; Die Basis für alle Namenknoten.]
{
};

node SimpleName : Name
[%i; <SimpleName>s sind einfache Namen, die nur aus einem "Id" bestehen.]
{
    attr Id id;
};

node OpSymbol : Name
[%i; Ein <OpSymbol> beschreibt ein Operator Symbol.]
{
    attr Id op; [Der Name des Operators, komplett mit `"\dq"',
                 z.B. `"\dq and\dq"'. ]
};

node SelName : Name
[%i; <SelName>s stellen <selected names> der Form <prefix>.<suffix> dar.]
{
    Name prefix;
	[Entweder ein weiterer <SelName> oder ein <SimpleName>.]
    attr Id suffix;
};

node IftsName : Name
[%i; <IftsName>s erfüllen viele Aufgabe (dank der wunderbaren
  Eindeutigkeit der VHDL Grammatik). Sie stellen Indizierungen,
  Funktionsaufrufe, Typumwandlungen oder <slices> dar.
]
{
    Name prefix;
    GenAssocElem assoc;
	[ Der jeweilige Kontext des <IftsName> bestimmt die Bedeutung
          und Gültigkeit der verschiedenen <GenAssocElem>s in diese Liste. ]
};

node AttributeName : Name
[%i;]
{
    Name prefix;
    attr Id attribute;
    NamedAssocElem first_actual;
	[ Die optionalen Parameter des Attributes. `Kein Parameter'
          wird durch <first_actual>$ == "NULL"$ ausgedrückt. ]
 };

node SelNameList : PosNode
[%i;]
{
    SelName name;
    SelNameList link;
};


[ \section{Deklarationen und Gültigkeitsbereiche}

Deklarationen geben den Dingen ihren Namen; aber auch die anonymen
Konstrukte werden als Deklarationen dargestellt. 

]

// Scopes and Declarations

node Declaration : PosNode
[ Die Basis für alle Deklarationen. Konkrete Dinge werden durch abgeleitete
  Knotentypen dargetstellt. ]
{
    Declaration next_decl = NULL;
	[ Alle <Declaration>s eines <Scope>s werden
	  mit <nect_decl> zu einer "NULL"-terminierten Liste zusammengehängt.
	]

    attr Id id;
	[ Der Name dieses Dings. Unbenannte Dinge haben $<id> == "NULL"$.
	]

    Scope scope = NULL;
	[ <scope> zeigt auf den <Scope> der diese <Declaration> enthält.
	  Einige <Declaration>s sind keinem <Scope> zugeordnet und haben
	  $<scope> == "NULL"$.
	]

    extra {
	char *idstr() { return id? id : "???"; }
	nDeclaration next();
    }
	[ \begin{desc}{Declaration \b{next}();}
	  Ein Gültigkeitsbereich kann durch mehrere <Scope>s
          dargestellt werden.\footnote{Z.B. bilden ein "package" und
          der zugehörige "package body" einen einzigen
          Gültigkeitsbereich. Jeder wird aber einzeln durch einen
          <Scope>-Knoten represäntiert. Diese beiden <Scope>s sind
          über ihren <continued> Zeiger zusammengehängt.}  <next()>
          wandert durch alle <Declaration>s dieser zusammenhängenden
          <Scope>s, während über <next_decl> nur die <Declaration>s
          eines einzigen <Scope>s erreichbar sind.
	  \end{desc}
	]
};

node AttributedDeclaration : Declaration
[ Die Basis für alle Deklarationen, die mit benutzerdefinierten
  Attributen garniert werden können. Noch nicht realisiert.
]
{
    AttributeValue first_attribute = NULL;
};

node Attribute : Declaration
[ Ein benutzerdefiniertes Attribut. ]
{
    Type type;
};

node AttributeValue : PosNode
[ Der Wert eines benutzerdefinierten Attributes aus einer
  <attribute specification>. Noch nicht realisiert.
]
{
    AttributeValue next;
    Attribute attribute;
    Expr value;
};

node Scope : AttributedDeclaration
[ Ein <Scope> enthält alle <Declaration>s einer <declarative region>.
]
{
    Scope continued = NULL;
	[ Alle <Scope>s eines Gültigkeitsbereichs werden über <continued>
	  zu einer "NULL"-terminierten Liste zusammengehängt.
	]

    Declaration first_decl = NULL;
	[ Die erste <Declaration> dieser <declarative region>.]

    extra {
	nDeclaration first();
	vaul_id_set decls_in_flight;
    }
	[\begin{desc}{Declaration \b{first}();}
	 Liefert die erste <Declaration> des Gültigkeitsbereichs, der mit
	 diesem <Scope> beginnt.
	 \end{desc}
	]
};

node TopScope : Scope
[%5; Ein <TopScope> repräsentiert die Umgebung einer <design unit> und nimmt
 die Informationen aus <library> und <use clauses> auf sowie die <design unit>
 selbst. Ein <TopScope> ist der einzige <Scope> mit $<scope> == "NULL"$
]
{
};

node LibNameDecl : Declaration
[ <LibNameDecl>s tauchen nur in einem <TopScope> auf und nehmen die
  Informationen aus <library clauses> auf.
]
{
};

node IndirectDecl : Declaration 
[ <use clauses> werden in <IndirectDecl>s umgesetzt. 
]
{
    Package ind_scope;
	[ Der <Scope>, dessen Deklarationen im <Scope> dieser <IndirectDecl>
          potentiell sichtbar sein sollen. $<id> == "NULL"$ bedeutet, daß
          alle Deklarationen in <ind_scope> betrachtet werden sollen,
          ansonsten sind nur die potentiell sichtbar, dessen <id> mit der
          <id> dieser <IndirectDecl> übereinstimmen.
	]
};

// design units

[ \subsection{Design Units}

<design units> werden allesamt durch Knoten dargestellt, die von
<Scope> abgeleitet sind. Diese Knoten können nur innerhalb eines
<TopScopes> auftauchen.
]

/* Entity, Architecure are in stats.t */

node Package : Scope
[%5; \ ]
{
};

node StandardPackage : Package
[%4; Da die Deklarationen aus dem <package> "std.standard" für viele Teile des
 Compilers wichtig sind, werden die benötigten Deklarationen hier notiert.
]
{
    Type predef_BIT = NULL;
    Type predef_BOOLEAN = NULL;
    Type predef_INTEGER = NULL;
    Type predef_REAL = NULL;
    Type predef_TIME = NULL;
    Type predef_STRING = NULL;
    Type predef_BIT_VECTOR = NULL;
    Type predef_SEVERITY_LEVEL = NULL;
    Type predef_FILE_OPEN_KIND = NULL;

    UniversalInteger universal_integer = NULL;
    UniversalReal universal_real = NULL;
};

node PackageBody : Scope
[%4; <continued> zeigt auf das zugehörige <Package>. Für Konstanten, die
  im <Package> nicht initialisiert wurden, kann dies im <PackageBody> mit
  einem <ConstantBody> nachgeholt werden.
]
{
};

include types.t
include objects.t
include subprogs.t
