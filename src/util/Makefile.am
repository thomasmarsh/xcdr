SUBDIRS = lerg rmap console

INCLUDES = -I../include -I/usr/local/include/mysqlcppapi-2.0

LIBS = \
	../xml/libxcdrXML-0.5.la \
	../rdbms/libxcdrRDBMS-0.5.la \
	../common/libxcdrCommon-0.5.la \
	@LIBS@

noinst_PROGRAMS = seek

seek_cc = \
	seek.cc

seek_SOURCES = $(seek_cc)
