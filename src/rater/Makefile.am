## top directory

INCLUDES = -I../include -I/usr/local/include/mysqlcppapi-2.0

rater_incdir = ../include/rater

h_sources = \
	$(rater_incdir)/activity.h \
	$(rater_incdir)/bracket.h \
	$(rater_incdir)/digit.h \
	$(rater_incdir)/except.h \
	$(rater_incdir)/ident.h \
	$(rater_incdir)/interval.h \
	$(rater_incdir)/party.h \
	$(rater_incdir)/period.h \
	$(rater_incdir)/query.h \
	$(rater_incdir)/rate.h \
	$(rater_incdir)/rater.h \
	$(rater_incdir)/response.h \
	$(rater_incdir)/tariff.h

cc_sources = \
	charge.cc \
	except.cc \
	ident.cc \
	populate.cc \
	rater.cc

build_sources = $(h_sources) $(cc_sources)

lib_LTLIBRARIES = libxcdrRater-0.5.la
libxcdrRater_0_5_la_SOURCES = $(h_sources) $(cc_sources)
#libxcdrRDBMS_0_5_la_LIBADD =
libxcdrRater_0_5_la_LDFLAGS = --version-info $(GENERIC_LIBRARY_VERSION) -release $(GENERIC_RELEASE)

