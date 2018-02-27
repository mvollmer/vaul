#include "vaulutil.h"

#include <string.h>

const char *progname = "<unknown>";
const char *base_progname = "<unknown>";

void util_init(char **argv)
{
    progname = argv[0];
    base_progname = strrchr(progname, '/');
    if(!base_progname)
	base_progname = progname;
    else
	base_progname++;
}
