#include "vaulutil.h"

char *xstrndup(const char *str, int n)
{
    char *nstr = (char *)xmalloc(n+1);
    strncpy(nstr, str, n);
    nstr[n] = '\0';
    return nstr;
}
