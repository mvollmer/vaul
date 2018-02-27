#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "vaulutil.h"

void *xrealloc(void *old_mem, size_t new_size)
{
    void *new_mem = old_mem? realloc(old_mem, new_size) : malloc(new_size);
    if(new_mem == NULL && new_size != 0)
	fatal("no more memory for %d bytes\n", new_size);
    return new_mem;
}
