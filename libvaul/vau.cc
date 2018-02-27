/* VHDL library utility

  Copyright (C) 1994-1996 University of Dortmund
  Department of Electrical Engineering, AG SIV
  All Rights Reserved.

  Permission to use, copy, modify and distribute this software and its
  documentation for non-commercial purposes is hereby granted,
  provided that both the copyright notice and this permission notice
  appear in all copies of the software, derivative works or modified
  versions, and any portions thereof, and that both notices appear in
  supporting documentation.

  THE UNIVERSITY OF DORTMUND ALLOWS FREE USE OF THIS SOFTWARE
  IN ITS "AS IS" CONDITION.  THE UNIVERSITY OF DORTMUND DISCLAIMS
  ANY LIABILITY OF ANY KIND FOR ANY DAMAGES WHATSOEVER RESULTING
  FROM THE USE OF THIS SOFTWARE.

  The University of Dortmund requests users of this software
  to return to

       Arbeitsgebiet Schaltungen der Signalverarbeitung
       Fakultät für Elektrotechnik
       Universität Dortmund
       44221 Dortmund, Germany

  or `msipc@nt.e-technik.uni-dortmund.de' any improvements or
  extensions that they make and grant the University of Dortmund
  the unlimited use and distribution of these changes.

  Read README.LICENSING for further information.

*/

#include <vaul/vaul.h>
#include "smart_lib.h"

#include <vaulutil.h>
#include <ctype.h>

/*
char *sNode::spaces(int l)
{
    if(l > 80) l = 80;
    return "                                                                                " + (80-l);
}
*/

void usage()
{
    fprintf(stderr, "usage: %s rxt [-l lib -svV] units ...\n", progname);
    exit(10);
}

char *libname = "testlib";
smart_libpool vaul(false);

extern int optind, opterr;
extern char *optarg;
extern "C" int getopt(int, char *const *, const char *);

bool verbose = false;

int node_count;
void count_nodes(nNode n, graph_visitor *)
{
    node_count++;
}

int main(int argc, char *argv[])
{
    int status = 0;

    int opt;

    util_init(argv);
    opterr = 0;

    while((opt=getopt(argc, argv, "l:vsV")) != -1) {
	switch(opt) {
	case 'l':
	    libname = optarg;
	    break;
	case 'v':
	    verbose = true;
	    vaul.flags |= slf_verbose;
	    break;
	case 's':
	    vaul.flags |= slf_smart;
	    break;
	case 'V':
	    printf("vau %s\n", vaul.get_version_string());
	    exit(0);
	case '?':
	    usage();
	    break;
	}
    }

    if(optind >= argc || strlen(argv[optind]) != 1)
	usage();

    char action = argv[optind][0];

    switch(action) {

    case 'r': {  // replace
	if(vaul.begin_session(libname)) {
	    for(int i = optind+1; i < argc && !vaul.is_error(); i++) {
		vaul_design_file df(argv[i], NULL, verbose);
		if(!df.insert(&vaul)) {
		    df.print_err(argv[i]);
		    status = 1;
		}
	    }
	    vaul.end_session(status != 0);
	}
	if(vaul.is_error()) {
	    vaul.print_err();
	    status = 1;
	}
	break;
    }

    case 'x': {  // extract
	for(int i = optind+1; i < argc; i++) {
	    vaul_design_unit *du;
	    if(isdigit(argv[i][0]))
		du = vaul.get(libname, atoi(argv[i]));
	    else
		du = vaul.get(libname, argv[i]);

	    if(du == NULL) {
		status = 1;
		message("%s not found in %s\n", argv[i], libname);
	    } else if(du->is_error()) {
		status = 1;
		du->print_err(argv[i]);
	    } else {
		if(verbose)
		    du->print();
		else
		    printf("yes, its there.\n");
	    }

	    release_ref(du);
	}
	break;
    }

    case 't': { // list
	vaul_library *lib = vaul.get(libname);
	vaul_lib_stats *stats = lib->get_stats();
	if(stats->is_error()) {
	    stats->print_err(libname);
	    status = 1;
	} else {
	    for(int i = 0; i < stats->n_entries; i++) {
		vaul_design_unit *du;
		if(verbose)
		    du = vaul.get(libname, stats->stats[i].name);
		else {
		    du = stats->stats[i].du;
		    retain_ref(du);
		}
		printf("%3d: %-20.20s", du->label, stats->stats[i].name);
		if(du) {
		    char buf[50];
		    time_t ctime = du->get_ctime();
		    strftime(buf, 50, "%b %d %H:%M", localtime(&ctime));
		    printf(" %15.15s %s", du->get_source(), buf);
		    if(verbose) {
			node_count = 0;
			graph_visitor v(du);
			v.visit(count_nodes, NULL, du->tree);
			printf(", %d nodes", node_count);
		    }
		    if(du->is_error())
			printf("\n\t%s", du->get_error_desc());
		} else
		    printf(" (can't load)");
		release_ref(du);
		printf("\n");
	    }
	    delete stats;
	}
	break;
    }

    default:
	usage();
	status = 1;
	break;
    }

    return status;
}
