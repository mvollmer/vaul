/* $Id: vaulutil.h,v 1.1 1996/05/29 20:10:56 mvo Exp $
**
**	utility functions found in libutil
*/

#ifndef UTIL_H
#define UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdarg.h>

/* used to initalize libutil, call it at top of main(argc, argv)
*/
void util_init(char **argv);

/* holds name this program was invoked with
*/
extern const char *progname;

/* holds basename of this program
*/
extern const char *base_progname;

/* printfs a message to stderr with "base_progname: " prepended.
*/
void  message(const char *fmt, ...);
void vmessage(const char *fmt, va_list ap);

/* same as { message(fmt, ...); exit(10); }
*/
void  fatal(const char *fmt, ...);
void vfatal(const char *fmt, va_list ap);

/* same as { message(fmt, ...); abort(); }
*/
void  fatal_abort(const char *fmt, ...);
void vfatal_abort(const char *fmt, va_list ap);

/* same as sprintf, but stores result in freshly allocated storage;
**  you must free this storage with free().
**  these functions never return NULL.
*/
char  *aprintf(const char *fmt, ...);
char *vaprintf(const char *fmt, va_list ap);

/* like system(cmd), but you can specify the shell to use and
**  signal handling is not touched.
**  shell == NULL: use $SHELL (if it exists) or /bin/sh
*/
int run_shell(const char *cmd, const char *shell);

/* like run_shell(cmd, shell), returns output of cmd as string
**  returns NULL on failure, free result with free().
*/
char *str_shell(const char *cmd, const char *shell);

/* does wordexpansion as shell does (in fact uses shell to do it).
** 'word' should not contain '>' and friends. never returns NULL.
**  free result with free().
*/
char *shellexp(const char *word, const char *shell);

/* same as GNU getcwd(NULL, 0), but works on systems that don't have it.
**  result must be freed with free().
*/
char *get_cwd();

/* same as malloc(size), but never fails
*/
void *xmalloc(size_t size);

/* same as realloc(old_mem, new_size), but never fails
*/
void *xrealloc(void *old_mem, size_t new_size);

/* Duplicates str into fresh storage, but at most n characters.
** never fails. result is '\0' terminated.
*/
char *xstrndup(const char *str, int n);

/* like strdup(str) but never fails and returns NULL for strdup(NULL)
*/
char *xstrdup(const char *str);

#ifdef __cplusplus
}
#endif

#endif

 
