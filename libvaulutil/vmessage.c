#include "vaulutil.h"

#include <stdio.h>
#include <stdarg.h>

void vmessage(const char *fmt, va_list ap)
{
    fprintf(stderr, "%s: ", base_progname);
    vfprintf(stderr, fmt, ap);
}
