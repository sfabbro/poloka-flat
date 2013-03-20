CLEANFILES = *~
MAINTAINERCLEANFILES = \
        Makefile.in \
        stamp-*

AM_CPPFLAGS = @POLOKA_CORE_CFLAGS@

AM_DEFAULT_SOURCE_EXT = .cc

bin_PROGRAMS = \
	pka-imcorrel pka-imdetrend pka-trendcombine pka-imtrim
# buggy
#	defringe fringefinder makefringe rustine_for_fringe

LDADD = $(top_builddir)/poloka/libpoloka-flat.la
