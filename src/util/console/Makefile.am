INCLUDES = -I../../include -I/usr/local/include/mysqlcppapi-2.0

XCDR_LIBS = \
	../../rater/libxcdrRater-0.5.la \
	../../xml/libxcdrXML-0.5.la \
	../../rdbms/libxcdrRDBMS-0.5.la \
	../../common/libxcdrCommon-0.5.la

LIBS = $(XCDR_LIBS) \
	-lreadline \
	-ltermcap \
	@LIBS@

bin_PROGRAMS = xcdr_rater_console

console_h = \
	command.h \
	console.h \
	context.h \
	exec.h \
	help.h \
	list.h \
	load.h \
	lookup.h \
	rate.h \
	readline.h \
	size.h \
	sql.h \
	tok.h \
	populate.h \
	usage.h

console_cc = \
	command.cc \
	console.cc \
	context.cc \
	exec.cc \
	list.cc \
	load.cc \
	lookup.cc \
	rate.cc \
	size.cc \
	sql.cc \
	usage.cc \
	help.cc \
	populate.cc \
	convenience.cpp \
	exception.cpp \
	operations_posix_windows.cpp \
	path_posix_windows.cpp

xcdr_rater_console_SOURCES = $(console_h) $(console_cc)

xcdr_rater_console$(EEXT): $(XCDR_LIBS)
