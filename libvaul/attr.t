/* attributes

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

node AttrSignalRef : ObjectRef 
[ Ein Zugriff auf ein eingebautes Attribut eines Signals, das
  ein Signal als Wert hat.
  Konkrete Attribute werden durch abgeleitete Knoten unterschieden.
]
{
    Type attr_type;
	[ Der Type des Attributes. ]
    ObjectRef signal;
	[ Das Signal, auf das sich dieses Attribut bezieht. ]
};

node Attr_DELAYED : AttrSignalRef 
[%3;
]
{
    Expr delay;
	[ Der Parameter des "DELAY" Attributes, oder "NULL" wenn keiner
	  angegeben wurde. ]
};

node Attr_STABLE : AttrSignalRef 
[%3;
]
{
    Expr duration;
	[ Der Parameter des "STABLE" Attributes, oder "NULL" wenn keiner
	  angegeben wurde. ]
};

node Attr_QUIET : AttrSignalRef 
[%3;
]
{
    Expr duration;
	[ Der Parameter des "QUIT" Attributes, oder "NULL" wenn keiner
	  angegeben wurde. ]
};

node Attr_TRANSACTION : AttrSignalRef 
[%3;
]
{
};

node AttrFunctionCall : Expr 
[ Ein Aufruf eines Attributes, das eine Funktion darstellt.
]
{
    Type attr_type;
	[ Der Typ dieses Attributes (der Rückgabetyp der Funktion). ]
};

node AttrSigFunc : AttrFunctionCall 
[ Ein Aufruf eines Attributes, das eine Funktion darstellt und sich auf ein
  Signal bezieht.
]
{
    ObjectRef signal;
	[ Ein Zugriff auf das Signal. ]
};

node Attr_EVENT : AttrSigFunc 
[%3;
]
{
};

node Attr_ACTIVE : AttrSigFunc 
[%3;
]
{
};

node Attr_LAST_EVENT : AttrSigFunc 
[%3;
]
{
};

node Attr_LAST_ACTIVE : AttrSigFunc 
[%3;
]
{
};

node Attr_LAST_VALUE : AttrSigFunc 
[%3;
]
{
};

node Attr_DRIVING : AttrSigFunc 
[%3;
]
{
};

node Attr_DRIVING_VALUE : AttrSigFunc 
[%3;
]
{
};

node AttrTypeFunc : AttrFunctionCall
[ Ein Aufruf eines Aggregates, das eine Funktion darstellt und sich auf einen
  Typen bezieht.
]
{
    Type prefix;
    	[ Der Prefix des Attributes. ]
    Expr argument;
	[ Das Argument. ]
};

node Attr_LEFT : AttrTypeFunc
[%3;
]
{
};

node Attr_RIGHT : AttrTypeFunc
[%3;
]
{
};

node Attr_LOW : AttrTypeFunc
[%3;
]
{
};

node Attr_HIGH : AttrTypeFunc
[%3;
]
{
};

node Attr_ASCENDING : AttrTypeFunc
[%3;
]
{
};

node Attr_IMAGE : AttrTypeFunc
[%3;
]
{
};

node Attr_VALUE : AttrTypeFunc
[%3;
]
{
};

node Attr_POS : AttrTypeFunc
[%3;
]
{
};

node Attr_VAL : AttrTypeFunc
[%3;
]
{
};

node Attr_SUCC : AttrTypeFunc
[%3;
]
{
};

node Attr_PRED : AttrTypeFunc
[%3;
]
{
};

node Attr_LEFTOF : AttrTypeFunc
[%3;
]
{
};

node Attr_RIGHTOF : AttrTypeFunc
[%3;
]
{
};

node ArrayAttr : Expr
{
    Type attr_type;
    Object array;
    attr int index;
};

node ArrayAttr_LEFT: ArrayAttr
{
};

node ArrayAttr_RIGHT: ArrayAttr
{
};

node ArrayAttr_HIGH: ArrayAttr
{
};

node ArrayAttr_LOW: ArrayAttr
{
};

node ArrayAttr_ASCENDING: ArrayAttr
{
};

/* ArrayAttr_RANGE and ArrayAttr_REVERSE_RANGE are not expressions
   and are defined in types.t (as <Range>s).
*/

node ArrayAttr_LENGTH : ArrayAttr
{
};

/***
** user defined attribute specifications
*/

node AttributeSpec : PosNode {
    attr Id attr_desig;
    EntitySpec entities;
    Expr value;
};

node EntitySpec : PosNode {
    EntityNameList names;
    attr int entity_class;
};

node EntityNameList : PosNode {
};

node EntityNameList_Ids : EntityNameList {
    IdList ids;
    // XXX - signatures
};

node EntityNameList_ALL : EntityNameList {
};

node EntityNameList_OTHERS : EntityNameList {
};
