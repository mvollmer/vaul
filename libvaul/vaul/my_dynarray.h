/* a dynamic array as a template

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

#ifndef VAUL_MY_DYNARRAY
#define VAUL_MY_DYNARRAY

template<class T> struct my_dynarray {

    my_dynarray(int sz = 10)
    {
	elts = new T[sz];
	max_elts = sz;
	n_elts = 0;
    }

    ~my_dynarray()
    {
	delete[] elts;
    }

    T& operator[] (int i)    // unchecked, does not expand array
    {
	return elts[i];
    }

    void add(T e)            // add to end, expand array
    {
	if(max_elts <= n_elts) {
	    max_elts += 20;
	    T *new_elts = new T[max_elts];
	    for(int i = 0; i < n_elts; i++)
		new_elts[i] = elts[i];
	    delete[] elts;
	    elts = new_elts;
	}
	elts[n_elts++] = e;
    }

    void add_uniq(T e)
    {
	if(!contains(e))
	    add(e);
    }

    int size()
    {
	return n_elts;
    }

    bool contains(T e)
    {
	for(int i = 0; i < n_elts; i++)
	    if(elts[i] == e)
		return true;
	return false;
    }

private:
    T *elts;
    int n_elts, max_elts;
};

template<class T> struct my_dynparray : my_dynarray<void *> {

    my_dynparray(int sz = 10) : my_dynarray<void*>(sz)
    {
    }

    T*& operator [] (int i)
    {
	return (T*&) my_dynarray<void*>::operator[](i);
    }

    void add(T *e)
    {
	my_dynarray<void *>::add((void *)e);
    }

    void add_uniq(T *e)
    {
	my_dynarray<void *>::add_uniq((void *)e);
    }
};

#endif // !defined(VAUL_MY_DYNARRAY)
