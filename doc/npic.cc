
/*  A Bison parser, made from npic.yy with Bison version GNU Bison version 1.22
  */

#define YYBISON 1  /* Identify Bison output.  */

#define	t_TEXT	258

#line 35 "npic.yy"


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



#define	YYFINAL		34
#define	YYFLAG		-32768
#define	YYNTBASE	10

#define YYTRANSLATE(x) ((unsigned)(x) <= 258 ? yytranslate[x] : 24)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     4,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     9,     8,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     5,     6,     2,
     2,     2,     2,     7,     2,     2,     2,     2,     2,     2,
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
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     2,     3
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     1,     6,     7,    10,    11,    16,    17,    20,    21,
    28,    29,    32,    36,    40,    45,    46,    48,    50,    52
};

static const short yyrhs[] = {    -1,
    21,    21,    11,    12,     0,     0,    12,    13,     0,     0,
     4,    14,    21,    15,     0,     0,    15,    16,     0,     0,
    21,     5,    22,    17,    18,     6,     0,     0,    18,    19,
     0,     7,    23,    23,     0,     8,    22,    22,     0,     9,
    22,    21,    20,     0,     0,    21,     0,     3,     0,     3,
     0,     3,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
   128,   130,   132,   134,   137,   138,   141,   143,   146,   157,
   160,   162,   165,   167,   168,   171,   173,   176,   180,   184
};

static const char * const yytname[] = {   "$","error","$illegal.","t_TEXT","'!'",
"':'","';'","'@'","'.'","'-'","start","@1","pics","pic","@2","nodes","node",
"@3","fields","field","o_path","label","text","number",""
};
#endif

static const short yyr1[] = {     0,
    11,    10,    12,    12,    14,    13,    15,    15,    17,    16,
    18,    18,    19,    19,    19,    20,    20,    21,    22,    23
};

static const short yyr2[] = {     0,
     0,     4,     0,     2,     0,     4,     0,     2,     0,     6,
     0,     2,     3,     3,     4,     0,     1,     1,     1,     1
};

static const short yydefact[] = {     0,
    18,     0,     1,     3,     2,     5,     4,     0,     7,     6,
     8,     0,     0,    19,     9,    11,     0,    10,     0,     0,
     0,    12,    20,     0,     0,     0,    13,    14,    16,    15,
    17,     0,     0,     0
};

static const short yydefgoto[] = {    32,
     4,     5,     7,     8,    10,    11,    16,    17,    22,    30,
     2,    15,    24
};

static const short yypact[] = {     1,
-32768,     1,-32768,-32768,    -3,-32768,-32768,     1,-32768,     1,
-32768,     0,    10,-32768,-32768,-32768,     3,-32768,    11,    10,
    10,-32768,-32768,    11,    10,     1,-32768,-32768,     1,-32768,
-32768,    15,    16,-32768
};

static const short yypgoto[] = {-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
    -2,   -18,    -7
};


#define	YYLAST		27


static const short yytable[] = {     3,
     6,    25,    26,     1,    13,     9,    28,    12,    18,    19,
    20,    21,    14,    23,    33,    34,    27,     0,     0,     0,
     0,     0,     0,    29,     0,     0,    31
};

static const short yycheck[] = {     2,
     4,    20,    21,     3,     5,     8,    25,    10,     6,     7,
     8,     9,     3,     3,     0,     0,    24,    -1,    -1,    -1,
    -1,    -1,    -1,    26,    -1,    -1,    29
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
#line 129 "npic.yy"
{ tex_header = yyvsp[-1]; tex_footer = yyvsp[0]; ;
    break;}
case 5:
#line 138 "npic.yy"
{ init(); ;
    break;}
case 6:
#line 138 "npic.yy"
{ emit(yyvsp[-1]); ;
    break;}
case 9:
#line 148 "npic.yy"
{
    	    node *n = new node;
    	    n->link = nodes;
    	    nodes = n;
    	    n->label = yyvsp[-2];
    	    n->header = yyvsp[0];
    	    n->x = n->y = -1;
    	    n->fields = NULL;
    	;
    break;}
case 13:
#line 166 "npic.yy"
{ nodes->x = atof(yyvsp[-1]); nodes->y = atof(yyvsp[0]); ;
    break;}
case 14:
#line 167 "npic.yy"
{ add_field('.', yyvsp[-1], yyvsp[0]); ;
    break;}
case 15:
#line 168 "npic.yy"
{ add_field('-', yyvsp[-2], yyvsp[-1], yyvsp[0]); ;
    break;}
case 16:
#line 172 "npic.yy"
{ yyval = NULL; ;
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
#line 188 "npic.yy"


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
