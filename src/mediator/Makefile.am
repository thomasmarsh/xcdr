INCLUDES = -I../include -I/usr/local/include/mysqlcppapi-2.0

XCDR_LIBS = \
	../entity/libxcdrEntity-0.5.la \
	../gateway/sonus/libxcdrGWSonus-0.5.la \
	../xml/libxcdrXML-0.5.la \
	../rdbms/libxcdrRDBMS-0.5.la \
	../common/libxcdrCommon-0.5.la

LIBS = $(XCDR_LIBS) @LIBS@

bin_PROGRAMS = xcdr_mediator

xcdr_mediator_h = \
	../include/mediator/mediator.h 

xcdr_mediator_cc = \
	mediator.cc

xcdr_mediator_SOURCES = $(xcdr_mediator_h) $(xcdr_mediator_cc)

xcdr_mediator$(EEXT): $(XCDR_LIBS)
