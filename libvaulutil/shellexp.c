#include "vaulutil.h"

char *shellexp(const char *word, const char *shell)
{
    char *cmd, *exp;

    cmd = aprintf("echo %s", word);
    exp = str_shell(cmd, shell);
    free(cmd);
    if(exp == NULL)
	fatal("shellexp: %s\n", str_error());
    return exp;
}

