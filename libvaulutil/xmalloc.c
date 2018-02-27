#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "vaulutil.h"

void *xmalloc(size_t size)
{
    void *mem = malloc(size);
    if(mem == NULL)
	fatal("no more memory for %d bytes\n", size);
    return mem;
}
