/* process a (small) graph description into eepic code.  -*- c++ -*- 

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
#include <vaulutil.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <errno.h>

#define YYSTYPE char *

int yylex();
void yyerror(char *str);

struct TeX_measure {
    TeX_measure *link;

    void (*func)(FILE *, void *);
    void *data;

    double pt_width, pt_height;
};

TeX_measure *measures;

double unit = 10;

struct node;

struct field {
    field *next;
    node *n;

    char type;
    char *desc;
    char *val;
    node *child;
    char *path;
    TeX_measure *tm;

    double height();
    double ymid();
    double xright();
};

struct node {
    node *link;
    char *label;

    char *header;
    double x, y;
    field *fields;

    TeX_measure *tm;

    double anchor_x(), anchor_y();
};

node *nodes;

void add_field(char t, char *desc, char *val, char *path = NULL);

double pic_width, pic_height, pic_x, pic_y;

void init();
void emit(const char *filename);

char *texify(const char *str)
{
    char buf[2*strlen(str)+1];
    char *cp;
    for(cp = buf; *str; str++) {
	if(strchr("_\\${}^%\"<>", *str))
	    *cp++ = '\\';
	*cp++ = *str;
    }
    *cp++ = '\0';
    return xstrdup(buf);
}

char *tex_header, *tex_footer;

int n_points;

%}

%token t_TEXT

%%

start:
	label label { tex_header = $1; tex_footer = $2; } pics
    ;

pics:
	/* nothing */
    |	pics pic
    ;

pic:
	'!' { init(); } label nodes { emit($3); }
    ;

nodes:
	/* nothing */
    |	nodes node
    ;

node:
	label ':' text
	{
    	    node *n = new node;
    	    n->link = nodes;
    	    nodes = n;
    	    n->label = $1;
    	    n->header = $3;
    	    n->x = n->y = -1;
    	    n->fields = NULL;
    	}
    	fields ';'
    ;

fields:
    	/* nothing */
    |	fields field
    ;

field:
	'@' number number     { nodes->x = atof($2); nodes->y = atof($3); }
    |	'.' text text 	      { add_field('.', $2, $3); }
    |	'-' text label o_path { add_field('-', $2, $3, $4); }
    ;

o_path:
	/* nothing */ { $$ = NULL; }
    |   label
    ;

label:
    	t_TEXT
    ;

text:
	t_TEXT
    ;

number:
    	t_TEXT
    ;

%%

void init()
{
    nodes = NULL;
    pic_width = pic_height = 0;
    pic_x = pic_y = 10000;
    n_points = 0;
}

TeX_measure *add_measure(void (*f)(FILE *, void *), void *d)
{
    TeX_measure *tm = new TeX_measure;
    tm->link = measures;
    measures = tm;
    tm->func = f;
    tm->data = d;
    return tm;
}

void measure()
{
    FILE *f = fopen("tm-input.tex", "w");
    if(f == NULL)
	fatal("tm-input.tex: %s\n", strerror(errno));
    fprintf(f, "%s", tex_header);
    fprintf(f, "\\immediate\\openout1 tm-output\n");
    for(TeX_measure *tm = measures; tm; tm = tm->link) {
	fprintf(f, "\\setbox0=\\hbox{");
	tm->func(f, tm->data);
	fprintf(f, "}\n");
	fprintf(f, "\\immediate\\write1{\\the\\wd0}\n");
	fprintf(f, "\\dimen0 \\ht0 \\advance\\dimen0 by \\dp0\n");
	fprintf(f, "\\immediate\\write1{\\the\\dimen0}\n");
    }
    fprintf(f, "\\immediate\\closeout1\n");
    fprintf(f, "%s", tex_footer);
    fclose(f);

    if(system("latex tm-input >/dev/null") < 0)
	fatal("running latex: %s", strerror(errno));

    f = fopen("tm-output.tex", "r");
    if(f == NULL)
	fatal("tm-output.tex: %s\n", strerror(errno));
    for(TeX_measure *_tm = measures; _tm; _tm = _tm->link)
	if(fscanf(f, "%lgpt\n%lgpt", &_tm->pt_width, &_tm->pt_height) != 2)
	    fatal("format error in TeX-measure\n");
    fclose(f);

    measures = NULL;
}

void add_field(char t, char *d, char *v, char *p)
{
    field *f = new field, **fp;
    for(fp = &nodes->fields; *fp; fp = &(*fp)->next)
    	;
    *fp = f;
    f->next = NULL;
    f->n = nodes;
    f->type = t;
    f->desc = d;
    f->val = v;
    f->path = p;
    f->child = NULL;
}

node *find_node(char *l)
{
    for(node *n = nodes; n; n = n->link)
    	if(!strcmp(n->label, l))
    	    return n;
    if(strcmp(l, "nil"))
	fatal("%s: no such node\n", l);
    return NULL;
}

double field::height()
{
    return (next? next->tm->pt_height : n->tm->pt_height) - tm->pt_height;
}

double field::ymid()
{
    return n->y*unit - tm->pt_height - height()/2;
}

double field::xright()
{
    return n->x * unit + n->tm->pt_width;
}

double node::anchor_x()
{
    return x * unit;
}

double node::anchor_y()
{
    return y * unit;
}

void include_point(double x, double y)
{
    if(x > pic_width)
	pic_width = x;
    if(y > pic_height)
	pic_height = y;
    if(x < pic_x)
	pic_x = x;
    if(y < pic_y)
	pic_y = y;
}

void validate_nodes()
{
    for(node *n = nodes; n; n = n->link) {
    	if(n->header == NULL)
    	    n->header = n->label;
//    	if(n->x < 0 || n->y < 0)
//    	    fatal("node '%s' has no location\n", n->label);
    	for(field *f = n->fields; f; f = f->next)
    	    if(f->type == '-')
    	    	f->child = find_node(f->val);
    }
}

struct emit_tab_data {
    node *n;
    field *f;

    emit_tab_data(node *_n, field *_f) { n = _n; f = _f; }
};

void emit_tab(FILE *f, void *data)
{
    node *n = ((emit_tab_data *)data)->n;
    field *last = ((emit_tab_data *)data)->f;

    fprintf(f, "\\begin{tabular}[t]{@{}|c|@{}}\\hline\n");
    fprintf(f, "\\textbf{%s}\\\\\\hline%s\n",
	    n->header, n->fields? "\\hline" : "");
    for(field *a = n->fields; a != last; a = a->next) {
	if(a->type == '-')
	    fprintf(f, "%s\\\\\\hline\n", a->desc);
	else
	    fprintf(f, "%s: \\texttt{%s}\\\\\\hline\n", a->desc, a->val);
    }
    fprintf(f, "\\end{tabular}"); // no '\n' here!
}

void find_unit()
{
    unit = 12;
    for(node *n = nodes; n; n = n->link) {
	for(field *f = n->fields; f; f = f->next)
	    if(f->type == '-') {
		unit = f->height();
		return;
	    }
    }
}

static void hbox(FILE *f, void *)
{
    fprintf(f, "\\hbox to 1pt{\\hfill}");
}

void measure_nodes()
{
    TeX_measure *test = add_measure(hbox, NULL);

    for(node *n = nodes; n; n = n->link) {
	for(field *f = n->fields; f; f = f->next)
	    f->tm = add_measure(emit_tab, new emit_tab_data(n, f));
	n->tm = add_measure(emit_tab, new emit_tab_data(n, NULL));
    }
    measure();

    fprintf(stderr, "test: %gpt %gpt\n", test->pt_width, test->pt_height);

    find_unit();

    for(node *_n = nodes; _n; _n = _n->link) {
	include_point(_n->x*unit, _n->y*unit);
	include_point(_n->x*unit+_n->tm->pt_width,
		      _n->y*unit-_n->tm->pt_height);
    }
}

void init_path();
void add_to_path(double x, double y);
void add_arrow();
void split_path();
void stroke_path(FILE *f);

void emit_path(FILE *f, field *a)
{
    bool path_split = false, stop = false;
    init_path();

    double x = a->xright(), y = a->ymid();

    add_to_path(x, y);
    x += a->child? unit : 0.5*unit;
    add_to_path(x, y);

    if(a->path) {
	for(char *cp = a->path; *cp; cp++) {
	    char *cpp = cp;
	    double m = strtod(cp, &cp);
	    if(cpp == cp)
		m = 1;
	    m *= unit;
	    bool point = isupper(*cp);
	    switch(tolower(*cp)) {
	    case 'l':
		x -= m;
		break;
	    case 'r':
		x += m;
		break;
	    case 'u':
		y += m;
		break;
	    case 'd':
		y -= m;
		break;
	    case '*':
		split_path();
		path_split = true;
		break;
	    case '+':
		stop = true;
		break;
	    }
	    if(point) {
		if(x < -100)
		    fprintf(stderr, "%s: %g?\n", a->path, x);
		add_to_path(x, y);
	    }
	}
    }
	
    if(!stop) {
	if(a->child) {
	    if(!path_split) {
		split_path();
		add_to_path(x+unit, y);
	    }
	    add_to_path(a->child->anchor_x(), a->child->anchor_y());
	    add_arrow();
	} else
	    fprintf(f, "\\put(%f,%f)"
		    "{\\makebox(0,0){\\vrule width 0.5pt height 7pt}}\n",
		    x, y);
    }
    stroke_path(f);
}

void emit_pic(FILE *f, void *unused)
{
    fprintf(f, "\\setlength{\\unitlength}{1pt} ");
    fprintf(f, "\\begin{picture}(%f,%f)(%f,%f)\n",
	    pic_width, pic_height, pic_x, pic_y);
    for(node *n = nodes; n; n = n->link) {
	fprintf(f, "\\put(%f,%f){%\n", n->x * unit, n->y * unit);
	emit_tab_data et(n, NULL);
	emit_tab(f, &et);
	fprintf(f, "}\n");
	for(field *a = n->fields; a; a = a->next)
	    if(a->type == '-')
		emit_path(f, a);
	fprintf(f, "\n");
    }
#if 0
    fprintf(f, "\\path(%f,%f)(%f,%f)(%f,%f)(%f,%f)(%f,%f)\n",
	    pic_x, pic_y, pic_x+pic_width, pic_y,
	    pic_x+pic_width, pic_y+pic_height, pic_x, pic_y+pic_height,
	    pic_x, pic_y);
#endif
    fprintf(f, "\\end{picture}\n");
}

void emit_file(const char *fn)
{
    FILE *f = fopen(fn, "w");
    if(f == NULL)
	 fatal("%s: %s\n", fn, strerror(errno));
    emit_pic(f, NULL);
    fclose(f);
}

void emit(const char *fn)
{
    fprintf(stderr, "%s ", fn);
    fflush(stderr);

    validate_nodes();
    measure_nodes();

    emit_file("/dev/null");
    pic_width -= pic_x;
    pic_height -= pic_y;
    emit_file(fn);
    nodes = NULL;
    fprintf(stderr, "%d (%g,%g) (%g,%g)\n", n_points,
	    pic_x, pic_y, pic_width, pic_height);
    n_points = 0;
}

int main(int argc, char **argv)
{
    util_init(argv);
    return (yyparse() == 0)? 0 : 1;
}


/* path magic
 */

struct coord {
    double x, y;
    coord() {}
    coord(double xx, double yy) { x = xx; y = yy; }
};

const int max_path = 200;

coord path[max_path];
int n_path, split_point;
bool arrow;

void init_path()
{
    n_path = 0;
    split_point = -1;
    arrow = false;
}

void add_to_path(double x, double y)
{
    if(n_path >= max_path)
	fatal("more than %d points per path\n", max_path);

    path[n_path].x = x;
    path[n_path].y = y;
    n_path++;
}

void add_arrow()
{
    arrow = true;
}

void split_path()
{
    if(split_point >= 0)
	fatal("path split more than once\n");
    split_point = n_path;
}

coord mid(coord a, coord b)
{
    return coord((a.x+b.x)/2, (a.y+b.y)/2);
}

double abs(coord a)
{
    return sqrt(a.x*a.x + a.y*a.y);
}

const double THRESHOLD = 0.5;

bool flat(coord p1, coord p2, coord p3, coord p4)
{
    coord b; b.x = p4.y - p1.y; b.y = p1.x - p4.x;
    double d1 = fabs((p2.x-p1.x)*b.x + (p2.y-p1.y)*b.y) / abs(b);
    double d2 = fabs((p3.x-p1.x)*b.x + (p2.y-p1.y)*b.y) / abs(b);
    return d1 < THRESHOLD && d2 < THRESHOLD;
}

void bezier(FILE *f, coord p1, coord p2, coord p3, coord p4)
{
    if(flat(p1, p2, p3, p4)) {
	fprintf(f, "(%f,%f)\n", p4.x, p4.y);
	n_points++;
    } else {
	coord l2 = mid(p1, p2);
	coord h = mid(p2, p3);
	coord r3 = mid(p3, p4);
	coord l3 = mid(l2, h);
	coord r2 = mid(h, r3);
	coord l4r1 = mid(l3, r2);
	bezier(f, p1, l2, l3, l4r1);
	bezier(f, l4r1, r2, r3, p4);
    }
}

void bezier(FILE *f, coord p1, coord p2, coord p3)
{
    bezier(f, p1, p2, p2, p3);
}

const double arrow_length = 0.4;
const double arrow_ratio = 3./8.;

void stroke_arrow(FILE *f, coord p, coord d)
{
    d.x -= p.x; d.y -= p.y;
    if(d.x == 0 && d.y == 0)
	return;

    double l = unit * arrow_length / abs(d);
    d.x *= l; d.y *= l;

    fprintf(f, "\\blacken\\path(%f,%f)", p.x, p.y);
    fprintf(f, "(%f,%f)", p.x + d.x - d.y * arrow_ratio,
	    p.y + d.y + d.x * arrow_ratio);
    fprintf(f, "(%f,%f)", p.x + d.x + d.y * arrow_ratio,
	    p.y + d.y - d.x * arrow_ratio);
    fprintf(f, "(%f,%f)\n", p.x, p.y);

//  fprintf(f, "\\put(%f,%f){\\makebox(0,0){$\\bullet$}}\n", p.x, p.y);
//  fprintf(f, "\\put(%f,%f){\\makebox(0,0){$\\Box$}}\n", d.x, d.y);
}

void stroke_path(FILE *f)
{
    if(n_path < 2)
	return;

    if(split_point >= 1) {
	double xoff = path[n_path-1].x - path[n_path-2].x;
	double yoff = path[n_path-1].y - path[n_path-2].y;
	for(int i = n_path-1; i >= split_point; i--) {
	    path[i].x = path[i-1].x+xoff;
	    path[i].y = path[i-1].y+yoff;
	}
    }

    for(int i = 0; i < n_path; i++)
	include_point(path[i].x, path[i].y);

    fprintf(f, "\\path(%f,%f)", path[0].x, path[0].y);

    if(n_path == 2) {
	fprintf(f, "(%f,%f)(%f,%f)", path[0].x, path[0].y,
		                     path[1].x, path[1].y);
    } else if(n_path == 3) {
	bezier(f, path[0], path[1], path[2]);
    } else {
	bezier(f, path[0], path[1], mid(path[1], path[2]));
	int i;
	for(i = 1; i < n_path - 3; i++)
	    bezier(f, mid(path[i], path[i+1]), path[i+1],
		   mid(path[i+1], path[i+2]));
	bezier(f, mid(path[i], path[i+1]), path[i+1], path[i+2]);
    }
    fprintf(f, "\n");

    if(arrow)
	stroke_arrow(f, path[n_path-1], path[n_path-2]);
}
