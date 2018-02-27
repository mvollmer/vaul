#include "vaulutil.h"

#include <stdio.h>
#include <stdarg.h>

void fatal(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vfatal(fmt, ap);
}
