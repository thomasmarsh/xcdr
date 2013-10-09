## top directory

INCLUDES = -I../include -I/usr/local/include/mysqlcppapi-2.0

h_sources = \
	../include/xml/model.h \
	../include/xml/repository.h \
	../include/xml/element.h \
	../include/xml/parser.h

cc_sources = \
	model.cc \
	repository.cc \
	element.cc \
	parser.cc \
	xml.cc

build_sources = $(h_sources) $(cc_sources)

lib_LTLIBRARIES = libxcdrXML-0.5.la
libxcdrXML_0_5_la_SOURCES = $(h_sources) $(cc_sources)
#libxcdrXML_0_5_la_LIBADD =
libxcdrXML_0_5_la_LDFLAGS = --version-info $(GENERIC_LIBRARY_VERSION) -release $(GENERIC_RELEASE)
