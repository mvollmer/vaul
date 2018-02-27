#include "vaulutil.h"

#include <stdlib.h>
#include <unistd.h>

int run_shell(const char *cmd, const char *shell)
{
    pid_t pid;
    int status;

    if(shell == NULL && (shell = getenv("SHELL")) == NULL)
	shell = "/bin/sh";

    switch(pid = fork()) {
    case -1:
	fatal("can't fork: %s\n", str_error());
    case 0:
	execlp(shell, shell, "-c", cmd, NULL);
	message("%s: %s\n", shell, str_error());
	_exit(0);
    default:
	if(waitpid(pid, &status, 0) < 0)
	    fatal("wait for child: %s\n", str_error());
	return status;
    }
}

