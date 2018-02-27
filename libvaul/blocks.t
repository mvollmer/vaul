/* blocks and components

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

[ \section{Komponenten}

]

node Component : ConcurrentStatement {
    Interface first_generic = NULL;
    Interface first_port = NULL;
};

node Block : Component {
    ConcurrentStatement stats = NULL;
    ConfigSpec specs = NULL;

    extra {
	void add_spec(nConfigSpec);
    }
};

node Entity : Block {
};

node Architecture : Block {
};

node BlockStat : Block {
    BindingIndic binding = NULL;
};

node ComponentInst : ConcurrentStatement {
    BindingIndic binding;
    ConfigSpec config = NULL;
};

node ConfigSpec : PosNode {
    ComponentSpec comps;
    BindingIndic binding;
    ConfigSpec next = NULL;
};

node ComponentSpec : Node {
    InstList ids;
    Component comp;
};

node InstList : PosNode {
};

node InstList_Ids : InstList {
    IdList ids;
};

node InstList_ALL : InstList {
};

node InstList_OTHERS : InstList {
};

node BindingIndic : Node {
    Component unit;
    Association generic_assoc;
    Association port_assoc;
};

node IncrementalBindingIndic : Node {
    Component unit;
    NamedAssocElem generic_assoc;
    NamedAssocElem port_assoc;
};

node Configuration : Component {
    Entity entity;
    BlockConfig config = NULL;
};

node BaseConfig : Scope {
    BaseConfig next_config = NULL;
};

node BlockConfig : BaseConfig {
    Block block;
    BaseConfig configs = NULL;
};

node CompConfig : BaseConfig {
    ComponentSpec comp_spec;
    CompInstList comps;
    BindingIndic binding;
    BlockConfig config = NULL;
};

node CompInstList : Node {
    CompInstList link;
    ComponentInst inst;
};
