/* aggregates -*- c++ -*-

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

node Aggregate : Expr
{
    Type type;
};

node RecordAggregate : Aggregate
{
    RecAggrAssoc first_assoc;
};

node RecAggrAssoc : PosNode
{
    RecAggrAssoc next;
    RecordElement elem;
    Expr actual;
};

node ArrayAggregate : Aggregate
{
    ArrAggrAssoc first_assoc;
};

node ArrAggrAssoc : PosNode          // pronounced: "Arrgh! Aschloch."
{
    ArrAggrAssoc next;
    Expr actual;
};

node SingleArrAggrAssoc : ArrAggrAssoc
{
    Expr index;
};

node RangeArrAggrAssoc : ArrAggrAssoc
{
    Range range;
};

node SliceArrAggrAssoc : RangeArrAggrAssoc
{
};

node OthersArrAggrAssoc : ArrAggrAssoc
{
};

node AmbgAggregate : Expr
{
    ElemAssoc first_assoc;
};

node ArtificialAmbgAggregate : AmbgAggregate
{
};

node ElemAssoc : PosNode
{
    ElemAssoc next;
    Choice first_choice;
    Expr actual;
};

node Choice : PosNode
{
    Choice next;
};

node ExprChoice : Choice
{
    Expr expr;
};

node RangeChoice : Choice
{
    GenAssocElem range;
    attr bool actual_is_slice = false;
};

node NamedChoice : Choice
{
    Name name;
};

node OthersChoice : Choice
{
};
