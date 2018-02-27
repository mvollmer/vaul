#include "vaulutil.h"

#include <stdio.h>
#include <stdarg.h>

void message(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vmessage(fmt, ap);
    va_end(ap);
}
