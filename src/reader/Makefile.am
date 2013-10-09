INCLUDES = -I../include -I/usr/local/include/mysqlcppapi-2.0

XCDR_LIBS = \
	../entity/libxcdrEntity-0.5.la \
	../gateway/sonus/libxcdrGWSonus-0.5.la \
	../xml/libxcdrXML-0.5.la \
	../rdbms/libxcdrRDBMS-0.5.la \
	../common/libxcdrCommon-0.5.la

LIBS = $(XCDR_LIBS) @LIBS@

bin_PROGRAMS = xcdr_reader

xcdr_reader_h = \
	../include/reader/reader.h \
	../include/reader/lfreader.h \
	../include/reader/cache/nodemap.h \
	../include/reader/cache/profile.h

xcdr_reader_cc = \
	reader.cc \
	lfreader.cc

xcdr_reader_SOURCES = $(xcdr_reader_h) $(xcdr_reader_cc)

xcdr_reader$(EEXT): $(XCDR_LIBS)
