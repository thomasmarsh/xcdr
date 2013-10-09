INCLUDES = -I../../include -I/usr/local/include/mysqlcppapi-2.0

LIBS = \
	../../xml/libxcdrXML-0.5.la \
	../../rdbms/libxcdrRDBMS-0.5.la \
	../../common/libxcdrCommon-0.5.la \
	@LIBS@

noinst_PROGRAMS = xcdr_lerg_loader

lerg_h = \
	common.h \
	lerg.h \
	loc_info.h \
	rmap.h \
	tok.h \
	writer.h

lerg_cc = \
	lerg.cc \
	loc_info.cc \
	main.cc \
	rmap.cc \
	writer.cc

xcdr_lerg_loader_SOURCES = $(lerg_h) $(lerg_cc)
