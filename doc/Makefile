# $Id: Makefile,v 1.8 1996/06/07 19:39:59 mvo Exp $
subdir := doc

programs := npic

npic-objs := npic npic-lex
npic-libs := vaulutil m

include-dirs = . $(srcdir) $(topdir)/libvaulutil

ref-src  := vaul.tex classes.tex nodes.tex node-desc.tex vaul.sty
stud-src := stud.tex vaul.sty

%.ps: %.dvi
	dvips -o $@ $<

generated := npic.cc npic.tab.h npic-lex.cc
dont-install := npic

dist-files := $(addsuffix .cc, $(npic-objs)) $(generated) \
              npic.yy npic-lex.ll vaul.pics stud.pics \
              Makefile $(ref-src) $(stud-src) vaul.dvi stud.dvi

include ../Makecommon

dvi: vaul.dvi stud.dvi
ps: vaul.ps stud.ps

stud.dvi: $(stud-src) pics/stud-stamp
	pic -t $< >stud.tmp; TEX=$(LATEX) texi2dvi stud.tmp; rm stud.tmp

vaul.dvi: $(ref-src) pics/vaul-stamp
	TEX=$(LATEX) texi2dvi vaul.tex

pics/%-stamp: %.pics npic
	if test ! -d pics; then mkdir pics; fi
	npic <$<
	touch $@

tree := $(addprefix ../libvaul/, \
	   tree.t decls.t types.t subprogs.t objects.t expr.t stats.t \
           misc.t blocks.t)

node-desc.tex: $(tree)
	cd ../libvaul && ../ctree/ctree -doc <$< ../doc/node-desc.tex
