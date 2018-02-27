# $Id: Makefile,v 1.5 1996/06/19 21:05:40 mvo Exp $
subdir := ctree

all:

programs := ctree sample

ctree-objs := parser lexer
ctree-libs := vaulutil

sample-objs := sample
sample-libs :=

tree-deps := sample.tree ctree runner.h runner.cc

sample.cc: $(tree-deps)
	ctree -cc $@ <sample.tree

sample.m: $(tree-deps)
	ctree -m $@ <sample.tree

sample.h: $(tree-deps)
	ctree -h $@ <sample.tree

defs = -DRUNNER_DECLARATION=\"$(abs_topdir)/ctree/runner.h\" \
       -DRUNNER_IMPLEMENTATION=\"$(abs_topdir)/ctree/runner.cc\"

include-dirs = . $(srcdir) $(topdir)/libvaulutil

generated := parser.cc parser.tab.h lexer.cc sample.cc sample.h sample.m
dist-files := $(addsuffix .cc, $(ctree-objs)) $(generated) \
              parser.yy lexer.ll runner.cc runner.h Makefile sample.tree

include ../Makecommon
