#include "vaulutil.h"

#include <stdio.h>
#include <stdarg.h>

void vfatal_abort(const char *fmt, va_list ap)
{
    vmessage(fmt, ap);
    abort();
}
