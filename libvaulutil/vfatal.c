#include "vaulutil.h"

#include <stdio.h>
#include <stdarg.h>

void vfatal(const char *fmt, va_list ap)
{
    vmessage(fmt, ap);
    exit(10);
}
