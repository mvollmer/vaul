
/*  A Bison parser, made from parser.yy with Bison version GNU Bison version 1.22
  */

#define YYBISON 1  /* Identify Bison output.  */

#define	t_NODE	258
#define	t_ATTR	259
#define	t_ID	260
#define	t_NUM	261
#define	t_PREFIXES	262
#define	t_REVERSE	263
#define	t_STOP	264
#define	t_EXTRA	265
#define	t_CODE	266
#define	t_ARG	267
#define	t_OTHER	268
#define	t_INIT_EXTRA	269
#define	t_DOC	270

#line 35 "parser.yy"

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


#ifndef YYLTYPE
typedef
  struct yyltype
    {
      int timestamp;
      int first_line;
      int first_column;
      int last_line;
      int last_column;
      char *text;
   }
  yyltype;

#define YYLTYPE yyltype
#endif

#ifndef YYSTYPE
#define YYSTYPE int
#endif
#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		74
#define	YYFLAG		-32768
#define	YYNTBASE	26

#define YYTRANSLATE(x) ((unsigned)(x) <= 270 ? yytranslate[x] : 48)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,    22,     2,    24,
    25,    21,     2,    16,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,    20,    17,     2,
    23,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,    18,     2,    19,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     2,     3,     4,     5,
     6,     7,     8,     9,    10,    11,    12,    13,    14,    15
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     5,     6,    14,    15,    18,    31,    32,    35,    36,
    39,    40,    44,    45,    49,    56,    58,    62,    65,    66,
    69,    71,    73,    74,    76,    77,    80,    82,    85,    87,
    89,    91,    93,    95,    97,    99,   101,   103,   105,   107,
   108,   109,   113,   114,   115,   119,   120
};

static const short yyrhs[] = {    27,
    47,    28,     9,     0,     0,     7,     5,    16,     5,    16,
     5,    17,     0,     0,    28,    29,     0,     3,     5,    30,
    47,    18,    31,    45,    43,    47,    19,    17,    47,     0,
     0,    20,     5,     0,     0,    31,    32,     0,     0,     4,
    33,    35,     0,     0,    12,    34,    35,     0,    36,     5,
    39,    40,    17,    47,     0,    32,     0,    18,    31,    19,
     0,     5,    37,     0,     0,    37,    38,     0,    21,     0,
    22,     0,     0,     8,     0,     0,    23,    41,     0,    42,
     0,    41,    42,     0,    24,     0,    25,     0,    18,     0,
    19,     0,    20,     0,    21,     0,    16,     0,    23,     0,
     5,     0,     6,     0,    13,     0,     0,     0,    10,    44,
    11,     0,     0,     0,    14,    46,    11,     0,     0,    15,
     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
   147,   154,   155,   159,   160,   163,   181,   182,   185,   186,
   189,   189,   190,   190,   191,   195,   197,   200,   203,   204,
   207,   207,   209,   210,   213,   214,   217,   219,   222,   223,
   223,   223,   223,   223,   223,   223,   224,   224,   224,   227,
   229,   229,   232,   234,   234,   237,   239
};

static const char * const yytname[] = {   "$","error","$illegal.","t_NODE","t_ATTR",
"t_ID","t_NUM","t_PREFIXES","t_REVERSE","t_STOP","t_EXTRA","t_CODE","t_ARG",
"t_OTHER","t_INIT_EXTRA","t_DOC","','","';'","'{'","'}'","':'","'*'","'&'","'='",
"'('","')'","start","pre","nodes","node","opt_base","decls","decl","@1","@2",
"comp_decl","type","mods","mod","opt_rev","opt_init","tok_seq","tok","opt_code",
"@3","opt_init_code","@4","doc",""
};
#endif

static const short yyr1[] = {     0,
    26,    27,    27,    28,    28,    29,    30,    30,    31,    31,
    33,    32,    34,    32,    32,    35,    35,    36,    37,    37,
    38,    38,    39,    39,    40,    40,    41,    41,    42,    42,
    42,    42,    42,    42,    42,    42,    42,    42,    42,    43,
    44,    43,    45,    46,    45,    47,    47
};

static const short yyr2[] = {     0,
     4,     0,     7,     0,     2,    12,     0,     2,     0,     2,
     0,     3,     0,     3,     6,     1,     3,     2,     0,     2,
     1,     1,     0,     1,     0,     2,     1,     2,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     0,
     0,     3,     0,     0,     3,     0,     1
};

static const short yydefact[] = {     2,
     0,    46,     0,    47,     4,     0,     0,     0,     0,     1,
     5,     0,     7,     0,     0,    46,     3,     8,     0,     9,
    43,    11,    19,    13,    44,    10,     0,    40,     0,    18,
     0,     0,    23,    41,    46,     9,    16,    12,    21,    22,
    20,    14,    45,    24,    25,     0,     0,     0,     0,     0,
    42,     0,    17,    37,    38,    39,    35,    31,    32,    33,
    34,    36,    29,    30,    26,    27,    46,    46,    28,    15,
     6,     0,     0,     0
};

static const short yydefgoto[] = {    72,
     2,     7,    11,    16,    21,    26,    29,    31,    38,    27,
    30,    41,    45,    50,    65,    66,    35,    46,    28,    32,
     5
};

static const short yypact[] = {     2,
    24,    -4,    14,-32768,-32768,    27,    -2,    18,    30,-32768,
-32768,    31,    17,    23,    36,    -4,-32768,-32768,    25,-32768,
     0,-32768,-32768,-32768,-32768,-32768,    37,    34,    21,     6,
    21,    35,    39,-32768,    -4,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,    22,    38,    29,    19,    -3,    33,
-32768,    40,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,    -3,-32768,    -4,    -4,-32768,-32768,
-32768,    53,    54,-32768
};

static const short yypgoto[] = {-32768,
-32768,-32768,-32768,-32768,    20,   -23,-32768,-32768,    28,-32768,
-32768,-32768,-32768,-32768,-32768,   -10,-32768,-32768,-32768,-32768,
   -16
};


#define	YYLAST		59


static const short yytable[] = {    19,
     9,    54,    55,    22,    23,    37,    10,    37,     1,    56,
     4,    24,    57,    25,    58,    59,    60,    61,    47,    62,
    63,    64,    22,    23,    22,    23,    39,    40,     3,     6,
    24,     8,    24,    12,    13,    14,    15,    53,    36,    17,
    18,    33,    20,    34,    49,    43,    44,    52,    51,    67,
    70,    71,    73,    74,    69,    48,    68,     0,    42
};

static const short yycheck[] = {    16,
     3,     5,     6,     4,     5,    29,     9,    31,     7,    13,
    15,    12,    16,    14,    18,    19,    20,    21,    35,    23,
    24,    25,     4,     5,     4,     5,    21,    22,     5,    16,
    12,     5,    12,    16,     5,     5,    20,    19,    18,    17,
     5,     5,    18,    10,    23,    11,     8,    19,    11,    17,
    67,    68,     0,     0,    65,    36,    17,    -1,    31
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "bison.simple"

/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

#ifndef alloca
#ifdef __GNUC__
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi)
#include <alloca.h>
#else /* not sparc */
#if defined (MSDOS) && !defined (__TURBOC__)
#include <malloc.h>
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
#include <malloc.h>
 #pragma alloca
#else /* not MSDOS, __TURBOC__, or _AIX */
#ifdef __hpux
#ifdef __cplusplus
extern "C" {
void *alloca (unsigned int);
};
#else /* not __cplusplus */
void *alloca ();
#endif /* not __cplusplus */
#endif /* __hpux */
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc.  */
#endif /* not GNU C.  */
#endif /* alloca not defined.  */

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	return(0)
#define YYABORT 	return(1)
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    { yychar = (token), yylval = (value);			\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { yyerror ("syntax error: cannot back up"); YYERROR; }	\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

#ifndef YYPURE
#define YYLEX		yylex()
#endif

#ifdef YYPURE
#ifdef YYLSP_NEEDED
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, &yylloc, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval, &yylloc)
#endif
#else /* not YYLSP_NEEDED */
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval)
#endif
#endif /* not YYLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

int	yychar;			/*  the lookahead symbol		*/
YYSTYPE	yylval;			/*  the semantic value of the		*/
				/*  lookahead symbol			*/

#ifdef YYLSP_NEEDED
YYLTYPE yylloc;			/*  location data for the lookahead	*/
				/*  symbol				*/
#endif

int yynerrs;			/*  number of parse errors so far       */
#endif  /* not YYPURE */

#if YYDEBUG != 0
int yydebug;			/*  nonzero means print parse trace	*/
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  YYINITDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
int yyparse (void);
#endif

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#define __yy_memcpy(FROM,TO,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else				/* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (from, to, count)
     char *from;
     char *to;
     int count;
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#else /* __cplusplus */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (char *from, char *to, int count)
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif
#endif

#line 192 "bison.simple"

/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
#define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
#else
#define YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#endif

int
yyparse(YYPARSE_PARAM)
     YYPARSE_PARAM_DECL
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YYSTYPE *yyvsp;
  int yyerrstatus;	/*  number of tokens to shift before error messages enabled */
  int yychar1 = 0;		/*  lookahead token as an internal (translated) token number */

  short	yyssa[YYINITDEPTH];	/*  the state stack			*/
  YYSTYPE yyvsa[YYINITDEPTH];	/*  the semantic value stack		*/

  short *yyss = yyssa;		/*  refer to the stacks thru separate pointers */
  YYSTYPE *yyvs = yyvsa;	/*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
  YYLTYPE yylsa[YYINITDEPTH];	/*  the location stack			*/
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;

#ifdef YYPURE
  int yychar;
  YYSTYPE yylval;
  int yynerrs;
#ifdef YYLSP_NEEDED
  YYLTYPE yylloc;
#endif
#endif

  YYSTYPE yyval;		/*  the variable used to return		*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int yylen;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Starting parse\n");
#endif

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YYLSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
yynewstate:

  *++yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YYSTYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YYLSP_NEEDED
      YYLTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YYLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.  */
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yyls1, size * sizeof (*yylsp),
		 &yystacksize);
#else
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yystacksize);
#endif

      yyss = yyss1; yyvs = yyvs1;
#ifdef YYLSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  yyerror("parser stack overflow");
	  return 2;
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
      yyss = (short *) alloca (yystacksize * sizeof (*yyssp));
      __yy_memcpy ((char *)yyss1, (char *)yyss, size * sizeof (*yyssp));
      yyvs = (YYSTYPE *) alloca (yystacksize * sizeof (*yyvsp));
      __yy_memcpy ((char *)yyvs1, (char *)yyvs, size * sizeof (*yyvsp));
#ifdef YYLSP_NEEDED
      yyls = (YYLTYPE *) alloca (yystacksize * sizeof (*yylsp));
      __yy_memcpy ((char *)yyls1, (char *)yyls, size * sizeof (*yylsp));
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YYLSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  goto yybackup;
 yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Reading a token: ");
#endif
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(yychar);

#if YYDEBUG != 0
      if (yydebug)
	{
	  fprintf (stderr, "Next token is %d (%s", yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  goto yynewstate;

/* Do the default action for the current state.  */
yydefault:

  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
yyreduce:
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YYDEBUG != 0
  if (yydebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


  switch (yyn) {

case 1:
#line 148 "parser.yy"
{
	if(do_doc) emit_doc_string(yyvsp[-2]);
        if(do_header) emit_header();
    ;
    break;}
case 3:
#line 156 "parser.yy"
{ npx = yyvsp[-5]; mpx = yyvsp[-3]; spx = yyvsp[-1]; ;
    break;}
case 6:
#line 169 "parser.yy"
{
	node *b = (yyvsp[-9]? node::find(yyvsp[-9]) : NULL);
	node *n = new node(b, yyvsp[-10], decl::consume(), yyvsp[-4], yyvsp[-5], yyvsp[-8], yyvsp[-3], yyvsp[0]);
	if(b == NULL) {
	    if(tree_base_node == NULL)
		tree_base_node = n;
	    else
		fatal("only one base node allowed.\n");
	}
    ;
    break;}
case 7:
#line 181 "parser.yy"
{ yyval = NULL; ;
    break;}
case 8:
#line 182 "parser.yy"
{ yyval = yyvsp[0];	 ;
    break;}
case 11:
#line 189 "parser.yy"
{ dtype = dt_attr; ;
    break;}
case 12:
#line 189 "parser.yy"
{ dtype = dt_child; ;
    break;}
case 13:
#line 190 "parser.yy"
{ dtype = dt_arg; ;
    break;}
case 14:
#line 190 "parser.yy"
{ dtype = dt_child; ;
    break;}
case 15:
#line 192 "parser.yy"
{	new decl(yyvsp[-5], yyvsp[-4], dtype, yyvsp[-3] != 0, yyvsp[-2], yyvsp[0]); ;
    break;}
case 18:
#line 200 "parser.yy"
{ yyval = aprintf("%s %s", yyvsp[-1], yyvsp[0]); ;
    break;}
case 19:
#line 203 "parser.yy"
{ yyval = ""; ;
    break;}
case 20:
#line 204 "parser.yy"
{ yyval = aprintf("%s%s", yyvsp[-1], yyvsp[0]); ;
    break;}
case 23:
#line 209 "parser.yy"
{ yyval = NULL; ;
    break;}
case 24:
#line 210 "parser.yy"
{ yyval = ""; ;
    break;}
case 25:
#line 213 "parser.yy"
{ yyval = NULL; ;
    break;}
case 26:
#line 214 "parser.yy"
{ yyval = yyvsp[0]; ;
    break;}
case 27:
#line 218 "parser.yy"
{ yyval = yyvsp[0]; ;
    break;}
case 28:
#line 219 "parser.yy"
{ yyval = aprintf("%s %s", yyvsp[-1], yyvsp[0]); ;
    break;}
case 40:
#line 228 "parser.yy"
{ yyval = ""; ;
    break;}
case 41:
#line 229 "parser.yy"
{ enter_code_block = 1; ;
    break;}
case 42:
#line 229 "parser.yy"
{ yyval = yyvsp[0]; ;
    break;}
case 43:
#line 233 "parser.yy"
{ yyval = ""; ;
    break;}
case 44:
#line 234 "parser.yy"
{ enter_code_block = 1; ;
    break;}
case 45:
#line 234 "parser.yy"
{ yyval = yyvsp[0]; ;
    break;}
case 46:
#line 238 "parser.yy"
{ yyval = NULL; ;
    break;}
case 47:
#line 239 "parser.yy"
{ yyval = yyvsp[0]; ;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */
#line 487 "bison.simple"

  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YYLSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = yylloc.first_line;
      yylsp->first_column = yylloc.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;

yyerrlab:   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (sizeof(yytname) / sizeof(char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (sizeof(yytname) / sizeof(char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      yyerror(msg);
	      free(msg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror("parse error");
    }

  goto yyerrlab1;
yyerrlab1:   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);
#endif

      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;

yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) goto yydefault;
#endif

yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

yyerrhandle:

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;
}
#line 242 "parser.yy"

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
