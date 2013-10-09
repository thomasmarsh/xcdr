INCLUDES = -I../src/include -I/usr/local/include/mysqlcppapi-2.0

LIBS = \
	../src/rater/libxcdrRater-0.5.la \
	../src/xml/libxcdrXML-0.5.la \
	../src/rdbms/libxcdrRDBMS-0.5.la \
	../src/common/libxcdrCommon-0.5.la \
	@LIBS@

noinst_PROGRAMS = \
	test_dtrie \
	test_period \
	test_stream \
	test_rater \
	test_threshold \
	test_ttree \
	test_timing \
	test_xml

test_rater_SOURCES = test_rater.cc
test_dtrie_SOURCES = test_dtrie.cc
test_period_SOURCES = test_period.cc
test_stream_SOURCES = test_stream.cc
test_threshold_SOURCES = test_threshold.cc
test_ttree_SOURCES = test_ttree.cc
test_timing_SOURCES = test_timing.cc
test_xml_SOURCES = test_xml.cc
