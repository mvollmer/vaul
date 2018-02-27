/* statements

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

include expr.t

[ \section{Sequentielle Anweisungen}

Die sequentiellen Anweisungen, die hintereinander ausgef�hrt werden
sollen, werden zu einer Liste zusammengeh�ngt. Zeiger auf eine
Anweisung, bei denen in der Beschreibung von mehreren Anweisungen die
Rede ist, meinen die komplette Liste.

]

node Label : Declaration 
[Die Namen von benannten Anweisungen (<labels>) werden als <Declaration>s
 in den zu der Anweisung geh�renden <Scope> aufgenommen.
]
{
    Statement stat;
	[ Die Anweisung, die diesen Namen tr�gt. ]
};

node Statement : PosNode 
[ Die Basis f�r die verschiedenen Anweisungen. ]
{
    Label label = NULL;
	[ Falls diese Anweisung einen Namen hat: hier ist er. Falls nicht,
	  ist "<label> == NULL". ]
    Statement next = NULL;
	[ Die n�chste Anweisung. ]
};

node DummyStat : Statement 
[ F�r unimplementierte Anweisungen. Verschwindet irgendwann. 
]
{
};

node ReturnStat : Statement 
[ Eine "return"-Anweisung.
]
{
    Expr value;
	[ Der Wert, der zur�ckgegeben werden soll oder "NULL", falls
	  diese Anweisung zu einer Prozedur geh�rt und nicht zu einer
	  Funktion. ]
};

node VarAssignment : Statement 
[ Eine Variablenzuweisung.
]
{
    Expr target;
	[ Das Ding, an das zugewiesen werden soll. ]
    Expr value;
	[ Der neue Wert. ]
};

node IfStat : Statement 
[ Eine "if"-Anweisung.
]
{
    Expr cond;
	[ Die Bedingung. Der Ausdruck ist immer vom Typ "bool". ]
    Statement then_stats;
	[ Die Anweisungen, die ausgef�hrt werden sollen, wenn
	  "<cond> == true" ist. ]
    Statement else_stats;
	[ Die Anweisungen, die ausgef�hrt werden sollen, wenn
	  "<cond> != true" ist. ]
};

node CaseStat : Statement
{
    Expr switch_expr;
    CaseAlternative first_alternative;
};

node CaseAlternative : PosNode
{
    CaseAlternative next;
    Choice first_choice;
    Statement stats;
};

node LoopStat : Statement 
[ Eine Schleife ist ein G�ltigkeitsbereich und gleichzeitig eine
  Anweisung. Mangels Mehrfachvererbung wird der <Scope> einer Schleife
  durch einen separaten <LoopScope> Knoten realisiert.
]
{
    LoopScope scope;
	[ Der zugeh�rige <Scope>. ]
    IterationScheme iteration_scheme;
	[ Das Schleifenschema. ]
    Statement stats;
	[ Die abh�ngigen Anweisungen. ]
};

node LoopScope : Scope 
[ Der zu einer Schleife geh�rende <Scope>.
]
{
    LoopStat loop;
	[ Die zugeh�rige Schleifenanweisung. ]
};

node IterationScheme : PosNode 
[ Die Basis f�r alle Schleifenschema. 
]
{
};

node WhileScheme : IterationScheme 
[ Ein "while"-Schema.
]
{
    Expr condition;
	[ Die Bedingung. Solange "<condition> == true", sollen
	  die abh�ngigen Anweisungen ausgef�hrt werden. ]
};

node ForScheme : IterationScheme 
[ Ein "for"-Schema. ]
{
    Object var;
	[ Die Laufvariable. Die Deklaration von <var> steht im
	  <LoopScope> dieser Schleife. ]
    Range range;
	[ Der Bereich, �ber den <var> laufen soll. ]
};

node PreForScheme : IterationScheme 
[%i;
]
{
    attr Id var;
    PreIndexConstraint range;
};

node LoopControlStat : Statement 
[ Die Basis f�r die "exit"- und "next"-Anweisungen.
]
{
    LoopStat loop;
	[ Die Schleife, auf die sich die Anweisung bezieht. ]
    Expr when;
	[ Die Bedingung f�r die tats�chliche Ausf�hrung der Anweisung. ]
};

node NextStat : LoopControlStat 
[ Eine "next"-Anweisung. 
]
{
};

node ExitStat : LoopControlStat 
[ Eine "exit"-Anweisung.
]
{
};

node NullStat : Statement 
[ Eine "null"-Anweisung.
]
{
};

node ProcedureCallStat : Statement 
[ Ein Prozeduraufruf. Analog zu einem <FunctionCall>, siehe auch dort.
]
{
    Procedure proc;
	[ Die Prozedur, die aufgerufen werden soll. ]
    Association first_actual;
	[ Ein Ausdruck f�r jeden Parameter der Prozedur. Diese
          Ausdr�cke werden nicht in der Reihenfolge der Parameter der Funktion
          aufgelistet, sondern in der Reihenfolge, in der sie im VHDL-Text
          stehen. ]
};

node WaitStat : Statement 
[ Ein "wait"-Statement.
]
{
    SignalList first_sensitivity;
	[ Die Signale, auf die reagiert werden soll. ]
    Expr condition;
    Expr timeout;
};

node SignalList : Node {
    ObjectRef signal;
    SignalList next;
};

node WaveformElement : Node 
[ Ein Teil einer Wellenform. 
]
{
    Expr value;
	[ Der Wert dieses Teils. ]
    Expr after;
	[ Die Zeit, bis es soweit ist. ]
    WaveformElement next;
	[ Das n�chste Element der kompletten Wellenform. ]
};

node SignalAssignment : Statement 
[ Eine Signalzuweisung.
]
{
    Expr target;
	[ Das Signal, an das zugewiesen wird. ]
    attr bool transport;	// XXX - rejection time
    WaveformElement first_wave;
	[ Die Wellenform. ]
};

node AssertStat : Statement
{
    Expr condition;
    Expr report;
    Expr severity;
};

[ \section {Parallele Anweisungen} 

Parellele Anweisungen sind -- wie sequentielle Anweisungen -- zu einer
Liste zusammengeh�ngt.
]

node ConcurrentStatement : Scope 
[%3; Die Basis f�r die parallelen Anweisungen. <ConcurrentStatement> sind
  von <Scope> abgeleitet, da einige von ihnen Deklarationen enthalten.
]
{
    ConcurrentStatement next_stat = NULL;
	[ Das n�chste Statement in der Liste. ]
};

node Process : ConcurrentStatement 
[%4;
]
{
    SignalList sensitivities;
    Statement stats;
	[ Die sequentiellen Anweisungen innerhalb des Prozesses. ]
};

node CondalWaveform : PosNode 
[%i;
]
{
    WaveformElement wave;
    Expr condition;
    CondalWaveform else_wave = NULL;
};

node CondalSignalAssign : PosNode 
[%i;
]
{
    Expr target;
    attr bool transport;    	// XXX - rejection time
    CondalWaveform wave;
};

include blocks.t
