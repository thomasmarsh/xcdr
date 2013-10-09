## top directory

INCLUDES = -I../include -I/usr/local/include/mysqlcppapi-2.0

h_sources = \
	../include/common/activity.h \
	../include/common/cdr.h \
	../include/common/common.h \
	../include/common/csv.h \
	../include/common/datetime.h \
	../include/common/dtrie.h \
	../include/common/etypes.h \
	../include/common/except.h \
	../include/common/filestream.h \
	../include/common/logging.h \
	../include/common/parser.h \
	../include/common/period.h \
	../include/common/pidfile.h \
	../include/common/slist.h \
	../include/common/stats.h \
	../include/common/strtok.h \
	../include/common/threshold.h \
	../include/common/timer.h \
	../include/common/ttree.h \
	../include/common/util.h \
	../include/common/version.h

cc_sources = \
	common.cc \
	csv.cc \
	datetime.cc \
	except.cc \
	filestream.cc \
	logging.cc \
	period.cc \
	pidfile.cc \
	strtok.cc \
	threshold.cc \
	timer.cc \
	util.cc \
	version.cc

build_sources = $(h_sources) $(cc_sources) ../include/common/version.h.in

lib_LTLIBRARIES = libxcdrCommon-0.5.la
libxcdrCommon_0_5_la_SOURCES = $(h_sources) $(cc_sources)
#libxcdrCommon_0_5_la_LIBADD =
libxcdrCommon_0_5_la_LDFLAGS = --version-info $(GENERIC_LIBRARY_VERSION) -release $(GENERIC_RELEASE)
