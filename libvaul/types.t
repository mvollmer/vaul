/* types -*- c++ -*-

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

[ \subsection{Typen} 

Die Knotentypen der Typen and Subtypen bilden eine gemeinsame Hierarchie mit
der Basis <Type>, obwohl ein VHDL Subtype kein Type ist. Dadurch wird es
einfacher gleichzeitig mit Typen oder Subtypen zu arbeiten.

Typen (und Subtypen) sind <Declaration>s. Viele dieser Deklarationen (vor
allem Subtypen) sind anonym und sind in keinem <Scope> aufgelistet.

Die Einschr�nkungen, die Subtypen dem Wertebereich eines Objektes
auferlegen, werden vom Parser zwar ermittelt, bleiben aber ansonsten
v�llig unber�cksichtigt. Entsprechende �berpr�fungen (bei
Arrayindizierungen oder sogar bei jeder Rechenoperation) k�nnen im
allgemeinen Fall erst zur Laufzeit des Programms durchgef�hrt werden
und m�ssen daher vom Backend implementiert werden\footnote{Oder auch
nicht, um akzeptable Performance zu erreichen.}

]

node Type : Declaration
[ Die Basis der Knotentypenhierarchie f�r VHDL Typen. Knoten f�r Subtypen
  werden auch von <Type> abgeleitet.
]
{  
    extra {
	nType get_base();
    }
    [\begin{desc}{Type \b{get_base}();}
      Ermittelt den Basistyp dieses <Type>s. Siehe <SubType>.
     \end{desc}
    ]
};

node Constraint : PosNode
[ Die Basis f�r die Einschr�nkungen eines <SubType>s.
]
{
};

node Range : Constraint
{
};

[
\begin{ndesc}{enum \b{RangeDirection}}{}
Die Richtung eines Bereichs.
  \shortdesc{\b{DirectionUp}}
  \shortdesc{\b{DirectionDown}}
\end{ndesc}
]

node ExplicitRange : Range
{
    attr RangeDirection dir;
    Expr first;
    Expr last;
	[ Die Grenzen dieser <Range>. Die Typen der Ausdr�cke
	  passen zu dem <SubType>, der diesen <Constraint> enth�lt.
	  <first> ist die Grenze, die im VHDL-Text links neben "to"
	  oder "downto" steht, <last> steht rechts davon.
	]
};

node ArrayRange : Range
{
    Type type;
    Object array;
    attr int index;
};

node ArrayAttr_RANGE : ArrayRange
{
};

node ArrayAttr_REVERSE_RANGE : ArrayRange
{
};

node PreIndexConstraint : Constraint
[%i;
]
{
    PreIndexConstraint next;
};

node PreIndexRangeConstraint : PreIndexConstraint
[%i3;
]
{
    Range range;
};

node PreIndexSubtypeConstraint : PreIndexConstraint
[%i3;
]
{
    Type type;
};

node IndexConstraint : Constraint
[ Ein <IndexConstraint> legt den <SubType> f�r einen Arrayindex fest. ]
{
    IndexConstraint next;
    [ Zeigt auf das n�chsten <IndexConstraint>. Die Liste ist parallel
      zu der Indexliste des Arraytyps zu dem dieser <IndexConstraint>
      geh�rt.
    ]
    Type type;
    [ Der <SubType> f�r diesen Index. Dieser <SubType> hat immer
      eine <Range>, der die Indexgrenzen festlegt.
    ]
};

node IncompleteType : Type 
[%i; Alle unfertigen Typen m�ssen noch im selben <Scope> 
komplettiert werden, daher tauchen sie im endg�ltigen Graphen nicht auf.
]
{
    extra-init {
	uses = NULL;
    }
    extra {
	struct use { use *next; nType *ref; } *uses;
	void add_use(nType &);
	void complete(nType real);
    }
};

node SubType : Type 
{
    Type base;
	[ Der unmittelbare Basistype dieses <Subtype>. <base> kann ein
	  weiterer <SubType> sein. Der endg�ltige Basistyp kann mit
	  <get_base> (siehe <Type>) ermittelt werden.
	]

    Constraint constraint;
	[ Die Einschr�nkungen f�r diesen <SubType>. Entweder eine
	  <Range> (f�r skalare Basistypen) oder eine
	  <IndexConstraint> Liste (f�r Arrays).
	]

    Name resol_func;	    // XXX - Function resol_func
	[ Noch nicht realisiert.
	]

};

node AccessType : Type
{
    Type designated;
};

[ Die grundlegenden Typen werden gem�� der Einteilung im <LRM> durch
  die folgenden Knotentypen klassifiziert.
]
node ScalarType : Type {
};

node NumericType : ScalarType {
};

node IntegerType : NumericType 
[%4;
]
{
};

node FloatingType : NumericType 
[%4;
]
{
};

[ 
  \bigskip
  Die implizite Definition von <universal integer> und <universal
  real> Typen wird an passender Stelle (irgenwo im "package standard")
  explizit in den Graphen aufgenommen. 
]

node UniversalInteger : IntegerType
[%5;]
{
};

node UniversalReal : FloatingType
[%5;]
{
};

node PhysicalType : NumericType
[ Ein <PhysicalType> listet einfach alle seine definierten 
  Einheiten auf.
]
{
    PhysicalUnit first;
        [ Die erste <PhysicalUnit> dieses Typs. ]
    SubType declaring_subtype;
        [ Der <SubType>, der den Namen dieses Typen hat. ]
};

node PhysicalUnit : Declaration 
[ Jede Einheit eines <PhysicalType> wird als <Declaration> im
  <Scope> des Typs aufgenommen, um sie anhand ihres Namens finden
  zu k�nnen.
]
{
    PhysicalType    type;
    [ Zeigt auf den zugeh�rigen <PhysicalType>.]
    PhysicalLiteralRef value;
    [ Der Wert dieser Einheit, ausgedr�ckt durch eine andere <PhysicalUnit>
      des zugeh�rigen Typs. Die grundlegende Einheit hat $<value> == NULL$.
    ]
    PhysicalUnit    next;
    [ Die n�chste Einheit des zugeh�rigen Typs. ]
};

node EnumType : ScalarType
[ Ein <EnumType> listet alle seine Elemente auf.]
{
    EnumLiteral first;
};

node EnumLiteral : Declaration
[ Es k�nnen mehrer <EnumLiterals> mit der gleichen <id> in einem
  <Scope> existieren. Sie werden an ihrem <EnumType> unterschieden.
]
{
    EnumType type;
    EnumLiteral next;
};

node CompositeType : Type
[ Die Basis f�r Array und Records. ]
{
};

node ArrayType : CompositeType 
[ Ein <ArrayType> beschreibt immer ein <unconstraint array>.
  Die Beschr�nkungen der Indices wird durch einen <SubType> diese
  Typs dargestellt.
]
{
    IndexType first_index;
	[ Die Liste der unbeschr�nkten Indextypen. Der <SubType> eines
          <ArrayType>s hat als Beschr�nkung eine parallele Liste
          von <IndexConstraints>. ]
    Type element_type;
	[ Der Typ der Arrayelemente. ]
};

node IndexType : PosNode
[ Beschreibt den Typ eines einzelnen Indices. ]
{
    IndexType next;
    Type index_type;
};

node SubarrayType : ArrayType
{
    ArrayType complete_type;
};

node RecordType : CompositeType
[ Ein <RecordType> enth�lt einfach eine Liste aller seiner
  Elemente.
]
{
    RecordElement first_element;
};

node RecordElement : PosNode
[ Ein einzelnes Element eines <RecordType>.
]
{
    RecordElement next;
    attr Id id;
	[ Der Name dieses Elements. ]
    Type type;
	[ Der Typ. ]
};

node FileType : Type
{
    Type content_type;
};

node DummyType : Type
[ F�r noch nicht implementierte Typen. Wird irgendwann verschwinden. ]
{
};

