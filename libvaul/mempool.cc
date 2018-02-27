/* memory management

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

#include <vaul/mempool.h>

#include <stdio.h>
#include <stdlib.h>
#include <vaulutil.h>

static const size_t block_size = 1016;

vaul_memregion::vaul_memregion()
{
    blocks = NULL;
}

vaul_memregion::~vaul_memregion()
{
    block *b, *b2;
    int n = 0;
    for(b = blocks; b; b = b2) {
	b2 = b->link;
	// printf("freed block @ %p-%p\n", b, ((char *)b) + b->size);
	::free(b);
	n++;
    }
//  printf("freed %d bytes of mem\n", n * block_size);
}

bool vaul_memregion::contains(void *m)
{
    for(block *b = blocks; b; b = b->link)
	if(m >= (void *)(b->mem) && m < (void *)(b->mem + b->size))
	    return 1;
    return 0;
}

vaul_mempool::vaul_mempool()
{
    mem = NULL;
    free = 0;
}

vaul_mempool::~vaul_mempool()
{
}

void *vaul_mempool::alloc(size_t size)
{
    void *ptr;

    if(size > free) {
	size_t new_size = (size > block_size? size : block_size);
	block *b = (block *)xmalloc(sizeof(block) + new_size);
//	printf("new block @ %p\n", b);
	b->link = blocks;
	blocks = b;
	mem = b->mem;
	free = new_size;
	b->size = 0;
    }
    ptr = mem;
    mem += size;
    free -= size;
    blocks->size += size;
//  printf("allocated %d bytes @ %p.\n", size, ptr);
    return ptr;
}

void vaul_mempool::store(vaul_memregion *r)
{
    block **bp;
    for(bp = &blocks; *bp; bp = &(*bp)->link)
	;
    *bp = r->blocks;
    r->blocks = blocks;
    blocks = NULL;
    mem = 0;
    free = 0;
}

void vaul_mempool::store(vaul_memregion &r)
{
    store(&r);
}

vaul_stringpool::vaul_stringpool()
{
}

vaul_stringpool::~vaul_stringpool()
{
}

char *vaul_stringpool::add(char *str)
{
    for(block *b = blocks; b; b = b->link)
	for(char *cp = &b->mem[0]; cp < &b->mem[b->size]; cp += strlen(cp)+1)
	    if(!strcmp(cp, str))
		return cp;
    return strcpy((char *)alloc(strlen(str)+1), str);
}

