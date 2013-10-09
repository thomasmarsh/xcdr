INCLUDES = -I../include -I/usr/local/include/mysqlcppapi-2.0

XCDR_LIBS = \
	../entity/libxcdrEntity-0.5.la \
	../rater/libxcdrRater-0.5.la \
	../gateway/sonus/libxcdrGWSonus-0.5.la \
	../xml/libxcdrXML-0.5.la \
	../rdbms/libxcdrRDBMS-0.5.la \
	../common/libxcdrCommon-0.5.la

LIBS = $(XCDR_LIBS) @LIBS@

bin_PROGRAMS = xcdr_engine

xcdr_engine_h = \
	../include/engine/engine.h

xcdr_engine_cc = \
	engine.cc

xcdr_engine_SOURCES = $(xcdr_engine_h) $(xcdr_engine_cc)

xcdr_engine$(EEXT): $(XCDR_LIBS)
