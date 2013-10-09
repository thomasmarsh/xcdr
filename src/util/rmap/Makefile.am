INCLUDES = -I../../include -I/usr/local/include/mysqlcppapi-2.0

XCDR_LIBS = \
	../../xml/libxcdrXML-0.5.la \
	../../rdbms/libxcdrRDBMS-0.5.la \
	../../common/libxcdrCommon-0.5.la

LIBS = $(XCDR_LIBS) @LIBS@

noinst_PROGRAMS = xcdr_rmap

rmap_h = \
	lerg.h \
	loc_info.h \
	rmap.h \
	tok.h

rmap_cc = \
	lerg.cc \
	loc_info.cc \
	main.cc

xcdr_rmap_SOURCES = $(rmap_h) $(rmap_cc)

xcdr_rmap$(EEXT): $(XCDR_LIBS)
