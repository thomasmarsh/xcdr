## top directory

INCLUDES = -I../include -I/usr/local/include/mysqlcppapi-2.0

h_sources = \
	../include/entity/entity.h

cc_sources = \
	entity.cc

build_sources = $(h_sources) $(cc_sources)

lib_LTLIBRARIES = libxcdrEntity-0.5.la
libxcdrEntity_0_5_la_SOURCES = $(h_sources) $(cc_sources)
#libxcdrCommon_0_5_la_LIBADD =
libxcdrEntity_0_5_la_LDFLAGS = --version-info $(GENERIC_LIBRARY_VERSION) -release $(GENERIC_RELEASE)
