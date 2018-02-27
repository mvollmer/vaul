/* list hacks

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

#ifndef VAUL_LIST_H
#define VAUL_LIST_H

void *generic_reverse(void *l, size_t next_offset);
void *generic_concat(void *l1, void *l2, size_t next_offset);

template<class node>
inline node *reverse2(node *l)
{
    node *next_constrain = l? l->next : NULL;
    return (node *)generic_reverse(l, offsetof(node, next));
}

template<class node>
inline node *concat(node *l1, node *l2)
{
    node *next_constrain = l1? l1->next : NULL;
    return (node *)generic_concat(l1, l2, offsetof(node, next));
}

#endif // !VAUL_LIST_H
