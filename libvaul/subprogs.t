/* subprograms -*- tex -*

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

[ \section{Funktionen und Prozeduren}

Die Deklaration einer Funktion oder Prozedur wird im Graphen immer
durch zwei Teile dargestellt: die Deklaration der Schnittstelle mit
einem <Function> oder <Procedure> Knoten und die Deklaration des
Funktions- oder Prozedurrumpfes, der die Anweisungen enthält, mit
einem <SubprogramBody>. Diese beiden Teile sind über den <continued>
Zeiger des <SubprogramBody> zusammengehängt. ]

node Subprogram : Scope 
[ Enthält die Parameterliste einer Funktion oder einer
  Prozedur.
]
{
    Interface first_formal;
};

node Procedure : Subprogram
[%5;
]
{
};

node Function : Subprogram 
[%5;
]
{
    attr bool pure;
    Type return_type;
};

node PredefOp : Function 
[%4;
  Die Beschreibung einer vordefinierten Funktion.
  Zu diesen Funktionen gibt es keinen <SubprogramBody>.
]
{
};

[\begin{ndesc}{enum \b{ObjectClass}}{}
  Die verschiedenen Arten von Objekten.
   \shortdesc{\b{ObjClass_None}}
   \shortdesc{\b{ObjClass_Signal}}
   \shortdesc{\b{ObjClass_Variable}}
   \shortdesc{\b{ObjClass_Constant}}
   \shortdesc{\b{ObjClass_File}}
 \end{ndesc}

 \begin{ndesc}{enum \b{Mode}}{}
 Die verschiedenen Modi eines Parameters.
  \shortdesc{\b{Mode_None}}
  \shortdesc{\b{Mode_In}}
  \shortdesc{\b{Mode_Out}}
  \shortdesc{\b{Mode_InOut}}
  \shortdesc{\b{Mode_Buffer}}
  \shortdesc{\b{Mode_Linkage}}
 \end{ndesc}
]

node Interface : Object 
[ Ein Parameter eines <Subprogram>s.
]
{
    Interface next_element;
	[ Der nächste Parameter in der Liste. ]
    attr ObjectClass object_class;
    attr Mode mode;
    attr bool buffer;
};

node SubprogramBody : Scope 
[%4;
  Die Anweisungen zu einem <Subprogram>. Der <continued>-Zeiger
  dieses <Scopes> zeigt auf die zugehörige <Subprogram>-Deklaration.
]
{
    Statement stats = NULL;
};

