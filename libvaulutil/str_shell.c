#include "vaulutil.h"

#include <stdlib.h>
#include <unistd.h>

char *str_shell(const char *cmd, const char *shell)
{
    int pfd[2];
    pid_t pid;
    char *result;
    int pos, len, n, status;

    if(shell == NULL && (shell = getenv("SHELL")) == NULL)
	shell = "/bin/sh";

    if(pipe(pfd) < 0)
	return NULL;

    switch(pid = fork()) {
    case -1:
	close(pfd[0]);
	close(pfd[1]);
	return NULL;
    case 0:
	dup2(pfd[1], 1);
	close(pfd[0]);
	close(pfd[1]);
	execlp(shell, shell, "-c", cmd, NULL);
	_exit(1);
    default:
	close(pfd[1]);
	result = xmalloc(32);
	len = 32;
	pos = 0;
	while((n = read(pfd[0], result + pos, len-pos)) > 0) {
	    pos += n;
	    if(pos == len) {
		len *= 2;
		result = xrealloc(result, len);
	    }
	}
	close(pfd[0]);
	if(pos > 0 && result[pos-1] == '\n')
	    pos--;
	result[pos] = '\0';
	result = xrealloc(result, pos+1);
	waitpid(pid, &status, 0);
	if(status || n < 0) {
	    free(result);
	    return NULL;
	}
	return result;
    }
}
