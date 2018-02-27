#include "vaulutil.h"

#include <stdio.h>
#include <stdarg.h>

void fatal_abort(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vfatal_abort(fmt, ap);
}
