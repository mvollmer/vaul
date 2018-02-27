# $Id: Makefile,v 1.12 1996/06/19 21:08:04 mvo Exp $

all:

subdir := libvaul

libraries := libvaul
programs  := vau v2c

ctree := ../ctree/ctree

# library

treeimps-n := 1
treeimps := $(addprefix treeimp-, $(treeimps-n))

libvaul-objs := error lib lock parser lexer dunit dfile binfile \
             $(treeimps) tree decls types mempool ref xmap \
	     message printer expr subprogs stats blocks attr \
	     version list

libvaul-headers := $(addprefix vaul/, \
             binfile.h design_file.h errors.h library.h mempool.h \
             printer.h tree.h xmap.h common.h design_unit.h lexer.h \
             lock.h parser.h ref.h vaul.h debug.h my_dynarray.h list.h)

tree := tree.t decls.t types.t subprogs.t objects.t expr.t stats.t \
        misc.t blocks.t aggr.t attr.t

$(addsuffix .cc,$(treeimps)): $(tree)
	$(ctree) -cc <$< 'treeimp-%d.cc' $(firstword $(treeimps-n))

vaul/tree.h: $(tree)
	$(ctree) -h <$< vaul/tree.h

tree.m: $(tree)
	$(ctree) -m <$< tree.m

# programs

vau-objs := vau smart_lib
vau-libs := vaul vaulutil

v2c-objs := v2c smart_lib
v2c-libs := vaul vaulutil

# general

include-dirs = . $(srcdir) $(topdir)/libvaulutil

generated := $(addsuffix .cc, $(treeimps)) \
             tree.m vaul/tree.h parser.cc parser.tab.h lexer.cc version.cc

dist-files := Makefile FlexLexer.h $(shell find vlib -name '*.vhd*' -print) \
              $(addsuffix .cc, $(libvaul-objs)) $(libvaul-headers) \
              parser.yy lexer.ll $(tree) vau.cc v2c.cc $(generated) \
              TODO VHDL93.bnf version.cc.in smart_lib.h smart_lib.cc \
              vhdl.yacc vhdl.lex

include ../Makecommon
