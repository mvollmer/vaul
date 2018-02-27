/* objects -*- tex -*-

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

[ \subsection{Konstanten, Variablen und Signale}
]

node Object : Declaration
[ Die Basis für Variablen, Konstanten, Signale, Files und Aliasse.
]
{
    Type type;
	[ Der <Type> oder <SubType> dieses Objekts. ]
    Expr initial_value;
	[ Der Initialisierungswert, wenn vorhanden. ]
};

node Variable : Object 
{
    attr bool shared;
	[ "true", wenn dies eine <shared variable> ist. Noch nicht relisiert. ]
};

node Constant : Object 
{
};

node ConstantBody : Declaration 
[ Liefert den Initialisierungswert für eine Konstante nach, falls sie
  noch nicht bei ihrer Deklaration initialisiert wurde.
]
{
    Constant decl;
	[ Die Konstante, die verspätet initialisiert wird. ]
    Expr initial_value;
};

[\begin{ndesc}{enum \b{SignalKind}}{}
   Die möglichen Varianten eines Signals.
   \shortdesc{\b{SigKind_None}}
   \shortdesc{\b{SigKind_Bus}}
   \shortdesc{\b{SigKind_Register}}
 \end{ndesc}
]

node Signal : Object 
{
    attr SignalKind signal_kind;
};

node GuardSignal : Signal 
[ Ein implizites <guard signal>. Noch nicht realisiert.
]
{
};

node Alias : Object
{
};

node File : Object
[ Der Dateiname ist in "initial_value".
]
{
    Expr open_mode;
};
