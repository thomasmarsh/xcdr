INCLUDES = -I../../include -I/usr/local/include/mysqlcppapi-2.0

LIBS = \
	../../xml/libxcdrXML-0.5.la \
	../../rdbms/libxcdrRDBMS-0.5.la \
	../../common/libxcdrCommon-0.5.la \
	@LIBS@

bin_PROGRAMS = xcdr_callgen_sonus

xcdr_callgen_sonus_SOURCES = callgen.cc

h_sources = \
	../../include/gateway/sonus/sonus.h \
	../../include/gateway/sonus/sonus_records.h

cc_sources = sonus.cc

build_sources = $(h_sources) $(cc_sources)

lib_LTLIBRARIES = libxcdrGWSonus-0.5.la
libxcdrGWSonus_0_5_la_SOURCES = $(h_sources) $(cc_sources)
#libxcdrGateway_0_5_la_LIBADD =
libxcdrGWSonus_0_5_la_LDFLAGS = --version-info $(GENERIC_LIBRARY_VERSION) -release $(GENERIC_RELEASE)

