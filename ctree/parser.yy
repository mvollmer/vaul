/* Grammar and almost everything else of ctree.   (Actions in -*- c++ -*-)

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

%{
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#include <vaulutil.h>

#define YYSTYPE char *

int yylex();
void yyerror(char *str);

enum decl_type { dt_arg, dt_attr, dt_child };

decl_type dtype;

struct decl {

    decl *link;

    char *type;
    char *id;
    int rev;
    char *init;
    decl_type dt;
    char *doc;

    decl(char *t, char *i, decl_type dt, int r, char *init, char *doc);

    static decl *head, **tail;
    static decl *consume()
     { decl *d = head; head = NULL; tail = &head; return d; }

    void dump_decl();
};

decl  *decl::head = NULL;
decl **decl::tail = &head;

struct node {

    node *base, *link, *derived, *dlink;

    char *id;
    decl *decls;
    char *extra, *init_extra;
    char *doc, *extra_doc, *postdoc;

    char *pint, *wint, *rint;

    node(node *b, char *i, decl *d, char *e, char *ie, char *doc, char *xdoc,
	 char *pdoc);

    static node *head, **tail;
    static node *find(char *name);

    void dump_args();
    void dump_init();

    void dump_decl();
    void dump_def();
    void dump_rev_decl(), dump_rev_def();
    void dump_announce();
    void dump_maker();

    void visit(void (*pre)(node *), void (*post)(node *));
};

int n_nodes = 1;    // includes nkInvalid

node  *node::head = NULL;
node **node::tail = &node::head;

node *node::find(char *name)
{
    for(node *n = head; n; n = n->link)
	if(!strcmp(n->id, name))
	    return n;
    fatal("unknown Node '%s'\n", name);
    return NULL;
}

char *npx = "n", *mpx = "m", *spx = "s";
node *tree_base_node = NULL;

bool do_header = false, do_maker = false, do_impl = false, do_doc = false;
void emit_header(), emit_maker(), emit_implementation(), emit_doc();
void emit_doc_string(char *doc);

int max_defs = 30;

int enter_code_block = 0;

void init_emit(const char *fn);
bool next_emit_file();
void cleanup_emit();

void emit(const char *fmt, ...);
void vemit(const char *fmt, va_list ap);
void emit_file(const char *file);
extern int emit_fileno;

%}

%token	t_NODE, t_ATTR, t_ID, t_NUM, t_PREFIXES, t_REVERSE,
	t_STOP, t_EXTRA, t_CODE, t_ARG, t_OTHER, t_INIT_EXTRA, t_DOC

%%

start:	pre doc nodes t_STOP 
    {
	if(do_doc) emit_doc_string($2);
        if(do_header) emit_header();
    }
    ;

pre:
    |	t_PREFIXES t_ID ',' t_ID ',' t_ID ';'
	    { npx = $2; mpx = $4; spx = $6; }
    ;

nodes:	/* nothing */
    |	nodes node
    ;

node:	t_NODE t_ID opt_base doc 
        '{'
	    decls
	    opt_init_code
	    opt_code doc
	'}' ';' doc
    {
	node *b = ($3? node::find($3) : NULL);
	node *n = new node(b, $2, decl::consume(), $8, $7, $4, $9, $12);
	if(b == NULL) {
	    if(tree_base_node == NULL)
		tree_base_node = n;
	    else
		fatal("only one base node allowed.\n");
	}
    }
    ;

opt_base:	    { $$ = NULL; }
    |	':' t_ID    { $$ = $2;	 }
    ;

decls:
    |	decls decl
    ;

decl:	t_ATTR { dtype = dt_attr; } comp_decl { dtype = dt_child; }
    |	t_ARG { dtype = dt_arg; } comp_decl { dtype = dt_child; }
    |	type t_ID opt_rev opt_init ';' doc
	     {	new decl($1, $2, dtype, $3 != 0, $4, $6); }
    ;

comp_decl:
	decl
    |	'{' decls '}'
    ;

type:	t_ID mods   { $$ = aprintf("%s %s", $1, $2); }
    ;

mods:		    { $$ = ""; }
    |	mods mod    { $$ = aprintf("%s%s", $1, $2); }
    ;

mod:	'*' | '&' ;

opt_rev:	    { $$ = NULL; }
    |	t_REVERSE   { $$ = ""; }
    ;

opt_init:	    { $$ = NULL; }
    |	'=' tok_seq { $$ = $2; }
    ;

tok_seq:
	tok	    { $$ = $1; }
    |	tok_seq tok { $$ = aprintf("%s %s", $1, $2); }
    ;

tok:
    '(' | ')' | '{' | '}' | ':' | '*' | ',' | '='
    | t_ID | t_NUM | t_OTHER ;
    /* but not ';'! */

opt_code:
	{ $$ = ""; }
    |	t_EXTRA { enter_code_block = 1; } t_CODE { $$ = $3; }
    ;

opt_init_code:
	{ $$ = ""; }
    |	t_INIT_EXTRA { enter_code_block = 1; } t_CODE { $$ = $3; }
    ;

doc:
            { $$ = NULL; }
    | t_DOC { $$ = $1; }
    ;

%%
decl::decl(char *t, char *i, decl_type dtype, int r, char *in, char *d)
{
    type = t;
    id = i;
    dt = dtype;
    rev = r; 
    init = in;
    doc = d;
    *tail = this;
    tail = &link;
    link = NULL;

    if(dt == dt_child) {
	char *cp;
	for(cp = type + strlen(type); cp > type && cp[-1] == ' '; cp--)
	    ;
	*cp = '\0';
    }
}

node::node(node *b, char *i, decl *d, char *e, char *ie, char *dd, char *ed,
	   char *pd)
{
    if(base = b) {
	dlink = b->derived;
	b->derived = this;
    }
    id = i;
    decls = d;
    *tail = this;
    tail = &link;
    link = NULL;
    derived = NULL;
    extra = e;
    init_extra = ie;
    doc = dd;
    extra_doc = ed;
    postdoc = pd;

    decl *rev_d = NULL;
    int impl_n = 0, expl_n = 0;
    for(d = decls; d; d = d->link) {
	if(d->rev)
	    expl_n++;
	if(!strcmp(d->type, id)) {
	    rev_d = d;
	    impl_n++;
	}
    }
    if(expl_n > 1)
	fatal("%s: only one reversion link allowed.\n", id);
    if(impl_n == 1)
	rev_d->rev = 1;

    pint = strstr(extra, "print(")? "print_g" : "print";
    wint = strstr(extra, "write(")? "write_g" : "write";
    rint = strstr(extra, "read(")? "read_g" : "read";
    
    n_nodes++;
}

void node::visit(void (*pre)(node *), void (*post)(node *))
{
    if(pre) pre(this);
    for(node *d = derived; d; d = d->dlink)
	d->visit(pre, post);
    if(post) post(this);
}

int need_comma = 0;

void maybe_comma()
{
    if(need_comma)
	emit(", ");
    need_comma = 1;
}

void decl::dump_decl()
{
    if(dt != dt_child)
	emit("%s%s", type, id);
    else
	emit("%s%s *%s", spx, type, id);
}

void node::dump_announce()
{
    emit("struct %s%s;\n", spx, id);
    emit("typedef %s%s *%s%s;\n", spx, id, npx, id);
}

void node::dump_args()
{
    if(base)
	base->dump_args();
    for(decl *d = decls; d; d = d->link) {
	if(!d->init) {
	    maybe_comma();
	    d->dump_decl();
	}
    }
}

void node::dump_init()
{
    if(base)
	base->dump_init();
    for(decl *d = decls; d; d = d->link) {
	if(!d->init) {
	    maybe_comma();
	    emit("%s", d->id);
	}
    }
}

void node::dump_decl()
{
    emit("\nstruct %s%s ", spx, id);
    if(base)
	emit(": %s%s ", spx, base->id);
    emit("{\n\n");

    decl *d;

    emit("\t%s%s(", spx, id);
    need_comma = 0;
    dump_args();
    int has_args = need_comma;
    emit(")\n");
    if(base) {
	emit("\t  : %s%s(", spx, base->id);
	need_comma = 0;
	base->dump_init();
	emit(")\n");
    }
    emit("\t{\n");
    for(d = decls; d; d = d->link)
	if(d->dt != dt_arg)
	    emit("\t\tthis->%s = %s;\n", d->id, d->init? d->init : d->id);
    emit("%s", init_extra);
    if(this == tree_base_node) {
	emit("\t\tthis->label = -1;\n");
	emit("\t\tset_region_id(GRAPH_NULL_REGION_ID);\n");
    }
    emit("\t}\n");

    if(has_args) {
	emit("\t%s%s() ", spx, id);
	if(base)
	    emit(": %s%s() ", spx, base->id);
	emit("{\n%s\n", init_extra);
	if(this == tree_base_node) {
	    emit("\t\tthis->label = -1;\n");
	    emit("\t\tset_region_id(GRAPH_NULL_REGION_ID);\n");
	}
	emit("}\n");
    }

    emit("\n");
    for(d = decls; d; d = d->link)
	if(d->dt == dt_child)
	    emit("\t%s%s %s;\n", npx, d->type, d->id);
	else if(d->dt == dt_attr)
	    emit("\t%s%s;\n", d->type, d->id);

    emit("%s", extra);
    if(this == tree_base_node)
	emit("\tint label;\n");

    emit("\n\tvirtual %sKind kind();\n", npx);

    if(this == tree_base_node) {
	emit("\tchar *type_id();\n");
	emit("\tstatic char *type_id(%sKind);\n", npx);
	emit("\tbool is(%sKind k);\n", npx);
	emit("\t%sKind base_kind() { return base_kind(kind()); }\n", npx);
	emit("\tstatic %sKind base_kind(%sKind);\n", npx, npx);
	emit("\tstatic bool is_base_of(%sKind b, %sKind d);\n", npx, npx);
	emit("\tstatic %s%s create(%sKind, void *placement_hint);\n",
	     npx, id, npx);
	emit("\tint get_label() { return label; }\n");
    }

    emit("\n\tvirtual void %s(graph_printer *pr);\n", pint);
    emit("\tvirtual void visit(graph_visit_f *pre,\n");
    emit("\t\tgraph_visit_f *post, graph_visitor *);\n");
    emit("\tvirtual void %s(graph_writer *gr);\n", wint);
    emit("\tvirtual void %s(graph_reader *gr);\n", rint);
    emit("};\n");
}

void node::dump_def()
{
    emit("\nvoid %s%s::%s(graph_printer *gp)\n{\n", spx, id, pint);
    if(base)
	emit("\t%s%s::print(gp);\n", spx, base->id);
    decl *d;
    for(d = decls; d; d = d->link)
	if(d->dt == dt_attr) {
	    emit("\tgp->print_lab(\"%s\");", d->id);
	    emit(" PRINT_ATTR(%s, gp);\n", d->id);
	} else if(d->dt == dt_child) {
	    emit("\tgp->print_lab(\"%s\");", d->id);
	    emit(" gp->print(%s);\n", d->id);
	}
    emit("}\n");

    emit("\nvoid %s%s::visit(graph_visit_f *pre, graph_visit_f *post,"
	 " graph_visitor *gv)\n{\n", spx, id);
    if(base)
	emit("\t%s%s::visit(pre, post, gv);\n", spx, base->id);
    for(d = decls; d; d = d->link)
	if(d->dt == dt_child)
	    emit("\tgv->visit(pre, post, %s);\n", d->id);
    emit("}\n");

    emit("\nvoid %s%s::%s(graph_writer *gw)\n{\n", spx, id, wint);
    if(base)
	emit("\t%s%s::write(gw);\n", spx, base->id);
    for(d = decls; d; d = d->link)
	if(d->dt == dt_child)
	    emit("\tgw->write(%s);\n", d->id);
    for(d = decls; d; d = d->link)
	if(d->dt == dt_attr)
	    emit("\tWRITE_ATTR(%s, gw);\n", d->id);
    emit("}\n");

    emit("\nvoid %s%s::%s(graph_reader *gr)\n{\n", spx, id, rint);
    if(base)
	emit("\t%s%s::read(gr);\n", spx, base->id);
    for(d = decls; d; d = d->link)
	if(d->dt == dt_child)
	    emit("\t%s = %s%s(gr->read(%sk%s));\n",
		 d->id, npx, d->type, npx, d->type);
    for(d = decls; d; d = d->link)
	if(d->dt == dt_attr)
	    emit("\tREAD_ATTR(%s, gr);\n", d->id);
    emit("}\n");

    emit("\n%sKind %s%s::kind()\n{\n\treturn %sk%s;\n}\n",
	npx, spx, id, npx, id);

    if(this == tree_base_node) {
	emit("\nchar *%s%s::type_id()\n{\n", spx, id);
	emit("\treturn type_id(kind());\n", npx);
	emit("}\n");

	emit("\nbool %s%s::is(%sKind k)\n{\n", spx, id, npx);
	emit("\tif(this == NULL) return false;\n");
	emit("\t%sKind this_k = kind();\n", npx);
	emit("\treturn this_k >= k && this_k <= __%sLastDerived[k];\n", npx);
	emit("}\n"); 

	emit("\nbool %s%s::is_base_of(%sKind b, %sKind d)\n{\n",
		 spx, id, npx, npx);
	emit("\treturn d >= b && d <= __%sLastDerived[b];\n", npx);
	emit("}\n");

	emit("\nchar *%s%s::type_id(%sKind k)\n{\n", spx, id, npx);
	emit("\treturn __%sIds[k];\n", npx);
	emit("}\n");
	emit("\n");

	emit("%sKind %s%s::base_kind(%sKind k)\n", npx, spx, id, npx);
	emit("{\n");
	emit("\treturn __%sBaseKind[k];\n", npx);
	emit("}\n");
	emit("\n");

	emit("%s%s %s%s::create(%sKind k, void *placement_hint)\n",
	     npx, id, spx, id, npx);
	emit("{\n");
	emit("\tswitch(k) {\n");
	emit("\tcase %skInvalid:\n", npx);
	emit("\t\treturn NULL;\n");
	for(node *n = node::head; n; n = n->link) {
	    emit("\tcase %sk%s:\n", npx, n->id);
	    emit("\t\treturn %s%s();\n", mpx, n->id);
	}
	emit("\tdefault:\n");
	emit("\t\tassert(false);\n");
	emit("\t}\n");
	emit("}\n");
    }

    dump_rev_def();
}

void node::dump_rev_decl()
{
    for(decl *d = decls; d; d = d->link)
	if(d->rev) {
	    emit("%s%s reverse(%s%s &);\n", npx, id, npx, id);
	    break;
	}
}

void node::dump_rev_def()
{
    for(decl *d = decls; d; d = d->link)
	if(d->rev) {
	    emit("\n%s%s reverse(%s%s &l)\n{\n", npx, id, npx, id);
	    emit("\t%s%s n, n2, r = NULL;\n", npx, id);
	    emit("\tfor(n = l; n; n = n2) {\n", npx, id);
	    emit("\t\tn2 = n->%s;\n", d->id);
	    emit("\t\tn->%s = r;\n", d->id);
	    emit("\t\tr = n;\n");
	    emit("\t}\n");
	    emit("\treturn r;\n");
	    emit("}\n");
	    break;
	}
}

void node::dump_maker()
{
    emit("#define %s%s new NEW_NODE_PLACEMENT %s%s\n", mpx, id, spx, id);
}

main(int argc, char **argv)
{
    int n_files = 1;

    util_init(argv);
    do_header = do_maker = do_impl = 0;
    if(argc >= 3) {
	if(!strcmp(argv[1], "-h"))
	    do_header = 1;
	else if(!strcmp(argv[1], "-m"))
	    do_maker = 1;
	else if(!strcmp(argv[1], "-cc")) {
	    if(argc > 3)
		n_files = atoi(argv[3]);
	    do_impl = 1;
	} else if(!strcmp(argv[1], "-doc")) {
	    do_doc = 1;
	}
    }
    if(!do_header && !do_maker && !do_impl && !do_doc) {
	fprintf(stderr, "usage: %s -h | -cc | -m output-file [n-files]\n",
		argv[0]);
	exit(1);
    }

    init_emit(argv[2]);
    yyparse();
    max_defs = (n_nodes+2+n_files-1) / n_files;
    if(do_impl)
	emit_implementation();
    if(do_maker)
	emit_maker();
    if(do_doc)
	emit_doc();
    cleanup_emit();

    if(emit_fileno != n_files)
	fprintf(stderr, "wrote %d files instead of %d\n",
		emit_fileno, n_files);

    exit(0);
}

void print_kind(node *n)
{
    maybe_comma();
    emit("\n\t%sk%s", npx, n->id);
}

void emit_verb_head(char *str)
{
    if(do_header)
	emit("%s", str);
}

void emit_header()
{
    node *n;
    if(!tree_base_node)
	fatal("no tree defined.\n");

    emit("enum %sKind {\n\t%skInvalid, %skLRef, %skXRef,",
	    npx, npx, npx, npx);
    need_comma = 0;
    tree_base_node->visit(print_kind, NULL);
    emit("\n};\n");

    emit("\ntypedef unsigned %s %sKind_ShortRep;\n\n",
	n_nodes <= UCHAR_MAX ?	"char" :
	n_nodes <= USHRT_MAX ?	"short" :
				"long",
	npx);

    for(n = node::head; n; n = n->link)
	n->dump_announce();

    emit("typedef %s%s graph_base_node;\n", spx, tree_base_node->id);
    emit("typedef %sKind graph_node_kind;\n", npx);
    emit_file(RUNNER_DECLARATION);

    emit("typedef void graph_visit_f(graph_base_node *, graph_visitor *);\n");
    for(n = node::head; n; n = n->link)
	n->dump_decl();

    emit("\n");
    for(n = node::head; n; n = n->link)
	n->dump_rev_decl();

    emit("\nunion %sTree {\n", npx);
    for(n = node::head; n; n = n->link)
	emit("\t%s%s %s;\n", npx, n->id, n->id);
    emit("};\n\n");
}

void emit_maker()
{
    emit("\n#ifndef NEW_NODE_PLACEMENT\n");
    emit("#define NEW_NODE_PLACEMENT\n");
    emit("#endif\n\n");
    for(node *n = node::head; n; n = n->link)
	n->dump_maker();    
}

int n_kinds;

void print_id(node *n)
{
    maybe_comma();
    emit("\n\t\"%s\"", n->id);
    n_kinds++;
}

void print_last_derived(node *n)
{
    node *d;
    for(d = n->derived; d && d->dlink; d = d->dlink)
	;
    if(d)
	print_last_derived(d);
    else {
	maybe_comma();
	emit("\n\t%sk%s", npx, n->id);
    }
}

void print_base_kind(node *n)
{
    maybe_comma();
    emit("\n\t%sk%s", npx, n->base? n->base->id : "Invalid");
}

char *verb_impl = NULL;
int verb_impl_len = 0, verb_impl_max = 0;

void emit_verb_impl(char *str)
{
    if(do_impl) {
	int l = strlen(str);
	if(verb_impl_max <= verb_impl_len + l) {
	    verb_impl_max = verb_impl_len+l+1;
	    verb_impl_max = (verb_impl_max+1023)&(~1023);
	    verb_impl = (char *)xrealloc(verb_impl, verb_impl_max);
	}
	strcpy(verb_impl + verb_impl_len, str);
	verb_impl_len += l;
    }
}

void emit_implementation_header()
{
    emit("// generated by ctree\n\n");

    if(verb_impl)
	emit("%s", verb_impl);

    emit("\n");
    emit("#ifndef PRINT_ATTR\n");
    emit("#define PRINT_ATTR(x, gr) (cout << (x) << \"\\n\")\n");
    emit("#endif\n");
    emit("#ifndef WRITE_ATTR\n");
    emit("#define WRITE_ATTR(x, gr)\n");
    emit("#endif\n\n");
    emit("#ifndef READ_ATTR\n");
    emit("#define READ_ATTR(x, gr)\n");
    emit("#endif\n\n");

    emit("#define GRAPH_INVALID_KIND %skInvalid\n", npx);
    emit("#define GRAPH_XREF_KIND %skXRef\n", npx);
    emit("#define GRAPH_LREF_KIND %skLRef\n", npx);
    emit_file(RUNNER_IMPLEMENTATION);
}

void emit_implementation()
{
    node *n;
    if(!tree_base_node)
	fatal("no tree defined.\n");

    emit_implementation_header();

    emit("char *__%sIds[] = {\n\t\"Invalid!\", \"?\", \"?\",", npx);
    need_comma = 0;
    n_kinds = 0;
    tree_base_node->visit(print_id, NULL);
    emit("\t/* %d kinds */\n", n_kinds);
    emit("\n};\n\n");

    emit("%sKind __%sLastDerived[] ="
	   " {\n\t%skInvalid, %skInvalid, %skInvalid,",
	    npx, npx, npx, npx, npx);
    need_comma = 0;
    tree_base_node->visit(print_last_derived, NULL);
    emit("\n};\n");

    emit("%sKind __%sBaseKind[] = {\n", npx, npx);
    emit("\t%skInvalid, %skInvalid, %skInvalid,", npx, npx, npx);
    need_comma = 0;
    tree_base_node->visit(print_base_kind, NULL);
    emit("\n};\n");
    emit("\n");

    int i = 2; // account for above
    for(n = node::head; n; n = n->link) {
	if(i > max_defs) {
	    if(next_emit_file())
		emit_implementation_header();
	    i = 0;
	}
	n->dump_def();
	i++;
    }
}

bool empty_doc(const char *doc)
{
    if(doc == NULL)
	return true;
    if(*doc == '[')
	doc++;
    if(*doc == '%') {
	while(*doc && *doc != ';')
	    doc++;
    }
    if(*doc == ';')
	doc++;
    while(isspace(*doc))
	doc++;
    return *doc == '\0' || (*doc == ']' && *(doc+1) == '\0');
}

void emit_doc()
{
    for(node *n = node::head; n; n = n->link) {
	bool ndesc = false;
	emit("\\smallskip\n");
	if(!empty_doc(n->doc) || !empty_doc(n->extra_doc) || n->decls) {
	    ndesc = true;
	    emit("\\begin{ndesc}");
	} else
	    emit("\\shortndesc");
	emit("{\\b{%s}", n->id);
	bool internal = false;
	int bases = 100;
	if(n->doc && n->doc[0] && n->doc[1] == '%') {
	    // process options
	    char *d = n->doc+2;
	    while(*d && *d != ';') {
		if(*d == 'i') {
		    internal = true;
		    d++;
		} else if(isdigit(*d))
		    bases = strtol(d, &d, 0);
		else {
		    fprintf(stderr, "unknown doc-option `%c' ignored\n", *d);
		    d++;
		}
	    }
	    if(*d == ';')
		d++;
	    n->doc = d;
	}

	node *b = n->base;
	for(int i = 1; b && i < bases; b = b->base, i++)
	    emit(" : %s", b->id);
	if(b)
	    emit(" : \\dots");
	emit("}{");
	if(internal)
	    emit("\\ \\internal");
	emit("}\n");
	if(n->doc)
	    emit_doc_string(n->doc);
	for(decl *d = n->decls; d; d = d->link) {
	    if(!empty_doc(d->doc))
		emit(" \\begin{desc}");
	    else
		emit(" \\shortdesc");
	    while(d->type[strlen(d->type)-1] == ' ')
		d->type[strlen(d->type)-1] = '\0';
	    if(d->dt != dt_child)
		emit("{\"%s\" \\b{%s};}\n", d->type, d->id);
	    else
		emit("{%s \\b{%s};}\n", d->type, d->id);
	    if(!empty_doc(d->doc)) {
		emit_doc_string(d->doc);
		emit(" \\end{desc}\n");
	    }
	}
	if(!empty_doc(n->extra_doc))
	    emit_doc_string(n->extra_doc);
	if(!empty_doc(n->doc) || !empty_doc(n->extra_doc) || n->decls) {
	    ndesc = !ndesc;
	    emit("\\end{ndesc}\n\n");
	}
	if(ndesc)
	    fprintf(stderr, "mismatch on %s!\n", n->id);
	if(!empty_doc(n->postdoc))
	    emit_doc_string(n->postdoc);
    }
}

void emit_doc_string(char *d)
{
    if(d == NULL)
	return;
    if(d[0] == '[')
	d++;
    if(d[strlen(d)-1] == ']')
	d[strlen(d)-1] = '\0';
    emit("%s\n", d);
}

FILE *efile;
int emit_fileno;
const char *emit_filename;

void init_emit(const char *fn)
{
    emit_filename = fn;
    emit_fileno = 1;
    char *rfn = aprintf(fn, emit_fileno);
    efile = fopen(rfn, "w");
    if(efile == NULL)
	fatal("%s: %s", rfn, strerror(errno));
    if(strcmp(fn, rfn))
	printf("%s\n", rfn);
}

void cleanup_emit()
{
    fclose(efile);
}

bool next_emit_file()
{
    emit_fileno++;
    char *rfn = aprintf(emit_filename, emit_fileno);
    if(!strcmp(rfn, emit_filename)) {
	fprintf(stderr, "would switch to %d. file\n", emit_fileno);
	return false;
    }
    fclose(efile);
    efile = fopen(rfn, "w");
    if(efile == NULL)
	fatal("%s: %s", rfn, strerror(errno));
    printf("%s\n", rfn);
    return true;
}

void emit(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vemit(fmt, ap);
    va_end(ap);
}

void vemit(const char *fmt, va_list ap)
{
    vfprintf(efile, fmt, ap);
}

void emit_file(const char *file)
{
    FILE *f = fopen(file, "r");
    if(f == NULL)
	fatal("%s: %s", file, strerror(errno));
    int c;
    while((c = fgetc(f)) != EOF)
	fputc(c, efile);
    fclose(f);
}
