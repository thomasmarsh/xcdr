## top directory

SUBDIRS = sonus

h_sources = ../include/gateway/gateway.h

cc_sources = #dco.cc mvam.cc mvam_cases.cc sonus.cc

build_sources = $(h_sources) $(cc_sources)

lib_LTLIBRARIES = libxcdrGateway-0.5.la
libxcdrGateway_0_5_la_SOURCES = $(h_sources) $(cc_sources)
#libxcdrGateway_0_5_la_LIBADD =
libxcdrGateway_0_5_la_LDFLAGS = --version-info $(GENERIC_LIBRARY_VERSION) -release $(GENERIC_RELEASE)

