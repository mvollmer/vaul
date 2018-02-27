#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "vaulutil.h"

char *vaprintf(const char *fmt, va_list ap)
{
#ifdef HAVE_ASPRINTF
    char *str;
    vasprintf(&str, fmt, ap);
    if(str == NULL)
	fatal("virtual memory exhausted");
    return str;
#else
    int n;
    char *buf;
    FILE *f = fopen("/dev/null", "w");
    if(f == NULL)
	fatal("/dev/null: %s\n", strerror(errno));
    n = vfprintf(f, fmt, ap);
    fclose(f);
    if(n < 0)
	fatal("/dev/null: %s\n", strerror(errno));
    buf = (char *)xmalloc(n + 1);
    vsprintf(buf, fmt, ap);
    return buf;
#endif
}

