CLEANFILES = *~
MAINTAINERCLEANFILES = \
        Makefile.in \
        stamp-*

AM_CPPFLAGS = @POLOKA_CORE_CFLAGS@

AM_DEFAULT_SOURCE_EXT = .cc

bin_PROGRAMS = \
	correlation flatfield medianstack trim \
	defringe fringefinder makefringe

LDADD = $(top_builddir)/poloka/libpoloka-flat.la
