CLEANFILES = *~
MAINTAINERCLEANFILES = \
	Makefile.in \
	stamp-*

lib_LTLIBRARIES = libpoloka-flat.la

src_includedir = $(includedir)/poloka
src_include_HEADERS = \
	fitsimagearray.h \
	fitsset.h \
	fringeutils.h \
	superflat.h

libpoloka_flat_la_SOURCES = \
	$(src_include_HEADERS) \
	fitsimagearray.cc \
	fitsset.cc \
	fringeutils.cc \
	superflat.cc

libpoloka_flat_la_CPPFLAGS = @POLOKA_CORE_CFLAGS@

libpoloka_flat_la_LIBADD = @POLOKA_CORE_LIBS@
