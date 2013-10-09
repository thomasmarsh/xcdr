## top directory

INCLUDES = -I../include -I/usr/local/include/mysqlcppapi-2.0

h_sources = \
	../include/rdbms/base.h \
	../include/rdbms/context.h \
	../include/rdbms/cache.h \
	../include/rdbms/model.h \
	../include/rdbms/repository.h \
	../include/rdbms/query.h \
	../include/rdbms/rdbms.h \
	../include/rdbms/sql.h \
	../include/rdbms/table.h

cc_sources = \
	base.cc \
	context.cc \
	model.cc \
	params.cc \
	query.cc \
	table.cc

build_sources = $(h_sources) $(cc_sources)

lib_LTLIBRARIES = libxcdrRDBMS-0.5.la
libxcdrRDBMS_0_5_la_SOURCES = $(h_sources) $(cc_sources)
#libxcdrRDBMS_0_5_la_LIBADD =
libxcdrRDBMS_0_5_la_LDFLAGS = --version-info $(GENERIC_LIBRARY_VERSION) -release $(GENERIC_RELEASE)
