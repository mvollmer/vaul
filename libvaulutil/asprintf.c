#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "vaulutil.h"

char *aprintf(const char *fmt, ...)
{
    va_list ap;
    char *ret;
    va_start(ap, fmt);
    ret = vaprintf(fmt, ap);
    va_end(ap);
    return ret;
}
     
