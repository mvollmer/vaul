#include "vaulutil.h"

#if defined(linux)

#include <stdlib.h>
#include <string.h>

const char *str_error()
{
   return strerror(errno);
}

#elif defined(sun)

extern char *sys_errlist[];
extern int sys_nerr;
extern int errno;

const char *str_error()
{
   if(errno < 0 || errno >= sys_nerr)
      return "unknown error";
   return sys_errlist[errno];
}


#else

#error unsupported target

#endif

