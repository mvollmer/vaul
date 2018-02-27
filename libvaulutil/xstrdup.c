#include "vaulutil.h"

char *xstrdup(const char *str)
{
    if(str == NULL)
	return NULL;
    else {
	char *nstr = (char *)xmalloc(strlen(str)+1);
	strcpy(nstr, str);
	return nstr;
    }
}
