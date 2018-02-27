# $Id: Makefile,v 1.2 1996/06/07 19:45:25 mvo Exp $
subdir := libvaulutil

libraries := libvaulutil

libvaulutil-objs := fatal vfatal xmalloc xrealloc asprintf vasprintf \
	            message vmessage run_shell str_error \
	            str_shell shellexp util_init xstrndup xstrdup \
	            fatal_abort vfatal_abort

libvaulutil-headers := vaulutil.h

dist-files := $(addsuffix .c, $(libvaulutil-objs)) $(libvaulutil-headers) \
              Makefile

include ../Makecommon
