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
       Fakult�t f�r Elektrotechnik
       Universit�t Dortmund
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
[%i; Die Basis f�r alle Namenknoten.]
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
[%i; <IftsName>s erf�llen viele Aufgabe (dank der wunderbaren
  Eindeutigkeit der VHDL Grammatik). Sie stellen Indizierungen,
  Funktionsaufrufe, Typumwandlungen oder <slices> dar.
]
{
    Name prefix;
    GenAssocElem assoc;
	[ Der jeweilige Kontext des <IftsName> bestimmt die Bedeutung
          und G�ltigkeit der verschiedenen <GenAssocElem>s in diese Liste. ]
};

node AttributeName : Name
[%i;]
{
    Name prefix;
    attr Id attribute;
    NamedAssocElem first_actual;
	[ Die optionalen Parameter des Attributes. `Kein Parameter'
          wird durch <first_actual>$ == "NULL"$ ausgedr�ckt. ]
 };

node SelNameList : PosNode
[%i;]
{
    SelName name;
    SelNameList link;
};


[ \section{Deklarationen und G�ltigkeitsbereiche}

Deklarationen geben den Dingen ihren Namen; aber auch die anonymen
Konstrukte werden als Deklarationen dargestellt. 

]

// Scopes and Declarations

node Declaration : PosNode
[ Die Basis f�r alle Deklarationen. Konkrete Dinge werden durch abgeleitete
  Knotentypen dargetstellt. ]
{
    Declaration next_decl = NULL;
	[ Alle <Declaration>s eines <Scope>s werden
	  mit <nect_decl> zu einer "NULL"-terminierten Liste zusammengeh�ngt.
	]

    attr Id id;
	[ Der Name dieses Dings. Unbenannte Dinge haben $<id> == "NULL"$.
	]

    Scope scope = NULL;
	[ <scope> zeigt auf den <Scope> der diese <Declaration> enth�lt.
	  Einige <Declaration>s sind keinem <Scope> zugeordnet und haben
	  $<scope> == "NULL"$.
	]

    extra {
	char *idstr() { return id? id : "???"; }
	nDeclaration next();
    }
	[ \begin{desc}{Declaration \b{next}();}
	  Ein G�ltigkeitsbereich kann durch mehrere <Scope>s
          dargestellt werden.\footnote{Z.B. bilden ein "package" und
          der zugeh�rige "package body" einen einzigen
          G�ltigkeitsbereich. Jeder wird aber einzeln durch einen
          <Scope>-Knoten repres�ntiert. Diese beiden <Scope>s sind
          �ber ihren <continued> Zeiger zusammengeh�ngt.}  <next()>
          wandert durch alle <Declaration>s dieser zusammenh�ngenden
          <Scope>s, w�hrend �ber <next_decl> nur die <Declaration>s
          eines einzigen <Scope>s erreichbar sind.
	  \end{desc}
	]
};

node AttributedDeclaration : Declaration
[ Die Basis f�r alle Deklarationen, die mit benutzerdefinierten
  Attributen garniert werden k�nnen. Noch nicht realisiert.
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
[ Ein <Scope> enth�lt alle <Declaration>s einer <declarative region>.
]
{
    Scope continued = NULL;
	[ Alle <Scope>s eines G�ltigkeitsbereichs werden �ber <continued>
	  zu einer "NULL"-terminierten Liste zusammengeh�ngt.
	]

    Declaration first_decl = NULL;
	[ Die erste <Declaration> dieser <declarative region>.]

    extra {
	nDeclaration first();
	vaul_id_set decls_in_flight;
    }
	[\begin{desc}{Declaration \b{first}();}
	 Liefert die erste <Declaration> des G�ltigkeitsbereichs, der mit
	 diesem <Scope> beginnt.
	 \end{desc}
	]
};

node TopScope : Scope
[%5; Ein <TopScope> repr�sentiert die Umgebung einer <design unit> und nimmt
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
          potentiell sichtbar sein sollen. $<id> == "NULL"$ bedeutet, da�
          alle Deklarationen in <ind_scope> betrachtet werden sollen,
          ansonsten sind nur die potentiell sichtbar, dessen <id> mit der
          <id> dieser <IndirectDecl> �bereinstimmen.
	]
};

// design units

[ \subsection{Design Units}

<design units> werden allesamt durch Knoten dargestellt, die von
<Scope> abgeleitet sind. Diese Knoten k�nnen nur innerhalb eines
<TopScopes> auftauchen.
]

/* Entity, Architecure are in stats.t */

node Package : Scope
[%5; \ ]
{
};

node StandardPackage : Package
[%4; Da die Deklarationen aus dem <package> "std.standard" f�r viele Teile des
 Compilers wichtig sind, werden die ben�tigten Deklarationen hier notiert.
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
[%4; <continued> zeigt auf das zugeh�rige <Package>. F�r Konstanten, die
  im <Package> nicht initialisiert wurden, kann dies im <PackageBody> mit
  einem <ConstantBody> nachgeholt werden.
]
{
};

include types.t
include objects.t
include subprogs.t
