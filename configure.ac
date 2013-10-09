# Process this file with autoconf to produce a configure script.

AC_INIT(xcdr, 0.5, thomas.marsh@seznam.cz)

AC_CONFIG_AUX_DIR(scripts)

# AM_INIT_AUTOMAKE(xcdr, 0.5)
AC_CONFIG_SRCDIR([config.h.in])
AM_CONFIG_HEADER([config.h])

AC_PREFIX_DEFAULT(/home/xcdr)

# Keep junk separate:
# AC_CONFIG_AUX_DIR(scripts)

# Tell it where to find extra macros:
# AM_ACLOCAL_INCLUDE(macros)

# -----------------------------------------------
# Package name and version number (user defined)
# -----------------------------------------------
GENERIC_NAME=xcdr
GENERIC_LIBRARY_NAME=xcdr

# release versioning
GENERIC_MAJOR_VERSION=0
GENERIC_MINOR_VERSION=5
GENERIC_MICRO_VERSION=1

# shared library versioning
GENERIC_LIBRARY_VERSION=2:0:0
#                     | | |
#              +------+ | +---+
#              |        |     |
#           current:revision:age
#              |        |     |
#              |        |     +- increment if interfaces have been added
#              |        |        set to zero if interfaces have been removed
#              |        |        or changed
#              |        +- increment if source code has changed
#              |           set to zero if current is incremented
#              +- increment if interfaces have been added, removed or changed

# -----------------------------------------------
# Package name and version number (DO NOT MODIFY)
# -----------------------------------------------

AC_SUBST(GENERIC_LIBRARY_VERSION)

PACKAGE=$GENERIC_NAME
AC_SUBST(GENERIC_NAME)
AC_SUBST(GENERIC_LIBRARY_NAME)

GENERIC_VERSION=$GENERIC_MAJOR_VERSION.$GENERIC_MINOR_VERSION.$GENERIC_MICRO_VERSION
GENERIC_RELEASE=$GENERIC_MAJOR_VERSION.$GENERIC_MINOR_VERSION
AC_SUBST(GENERIC_RELEASE)
AC_SUBST(GENERIC_VERSION)

VERSION=$GENERIC_VERSION



AM_INIT_AUTOMAKE($PACKAGE, $VERSION)
# , no-define)

# -----------------------------------------------
# Checks for programs.
# -----------------------------------------------

AC_PROG_CC
AC_PROG_CXX
AM_PROG_LIBTOOL
AM_SANITY_CHECK

#  What's this for?
#  AC_PROG_MAKE_SET

#  Initialize maintainer mode:
AM_MAINTAINER_MODE
#  Add support for dmalloc:
AM_WITH_DMALLOC()

# -----------------------------------------------
# Checks for libraries.
# -----------------------------------------------

#  Ask user for path to libmysqlclient stuff:
#  Maybe one day there will be an AM_PATH_MYSQLCLIENT() macro.

#  XXX: TODO: configure should fail if missing these
AC_LANG_CPLUSPLUS
AC_C_CONST
AC_HEADER_STDC
AC_CHECK_HEADERS([fcntl.h limits.h stdlib.h sys/time.h unistd.h values.h])
AC_TYPE_MODE_T
AC_TYPE_OFF_T
AC_TYPE_SIZE_T
AC_HEADER_TIME
AC_STRUCT_TM
AC_FUNC_MKTIME
AC_FUNC_STAT
AC_FUNC_STRFTIME
AC_CHECK_FUNCS([gettimeofday memset strerror strtol])

# Other library stuff:
AC_CHECK_LIB(intl, main)
AC_CHECK_LIB(socket, main)
AC_CHECK_LIB(nsl, main)
AC_CHECK_LIB(crypt, crypt)
AC_CHECK_LIB(z, main)

# -----------------------------------------------
# MYSQLCLIENT
# -----------------------------------------------

AC_ARG_WITH(mysql,
    [  --with-mysql=DIR		Use mysql having this prefix.],
    [MYSQL_PREFIX=$with_mysql],
    [MYSQL_PREFIX="/usr/local"])

AC_SUBST(MYSQL_PREFIX)

echo "MySQL prefix is set to: $MYSQL_PREFIX"
MYSQL_LIBS="-L${MYSQL_PREFIX}/lib/mysql -lmysqlclient"
MYSQL_CFLAGS="-I${MYSQL_PREFIX}/mysql/include"
AC_SUBST(MYSQL_LIBS)
AC_SUBST(MYSQL_CFLAGS)

CFLAGS="$CFLAGS $MYSQL_CFLAGS"
LIBS="$LIBS $MYSQL_LIBS"


AC_CHECK_LIB([mysqlclient], [mysql_store_result])

dnl MYSQL_LIBS="-L/usr/local/lib -lmysqlcppapi-1.9 -lmysqlclient"
dnl MYSQL_CFLAGS="-I/usr/local/include -I/usr/local/include/mysqlcppapi-2.0"
dnl AC_SUBST(MYSQL_LIBS)
dnl AC_SUBST(MYSQL_CFLAGS)

dnl CFLAGS="$CFLAGS $MYSQL_CFLAGS"
dnl LIBS="$LIBS $MYSQL_LIBS"


xcdr_libfail=""
xcdr_fail="no"

if test "$ac_cv_lib_mysqlclient_mysql_store_result" != "yes"; then
	echo
	echo "*** Warning: the MySQL client library could not be found!"
	echo "*** Please get a copy from http://www.mysql.com/."
	echo
	xcdr_fail="yes"
	xcdr_libfail="$xcdr_libfail -lmysqlclient"
	exit 1
fi

AC_CHECK_LIB(mysqlcppapi-1.9, main)

if test "$ac_cv_lib_mysqlcppapi_1_9_main" != "yes"; then
	echo
	echo "*** Warning: the MySQL C++ API(1.9) library could not be found!"
	echo "*** Please get a copy from http://www.mysql.com/."
	echo
	xcdr_fail="yes"
	xcdr_libfail="$xcdr_libfail -lmysqlcppapi-1.9"
	exit 1
fi

AC_CHECK_LIB(pcre, pcre_exec)

if test "$ac_cv_lib_pcre_pcre_exec" != "yes"; then
	echo
	echo "*** Warning: the PCRE library could not be found!"
	echo "*** Please get a copy from http://www.pcre.org/."
	echo
	xcdr_fail="yes"
	xcdr_libfail="$xcdr_libfail -lpcre"
	exit 1
fi

AC_CHECK_LIB(icui18n, main)
AC_CHECK_LIB(icuuc, main)
AC_CHECK_LIB(icudata, main)

icufail="no";

if test "$ac_cv_lib_icui18n_main" != "yes"; then
	icufail="yes"
fi
if test "$ac_cv_lib_icuuc_main" != "yes"; then
	icufail="yes"
fi
if test "$ac_cv_lib_icudata_main" != "yes"; then
	icufail="yes"
fi

if test "$icufail" == "yes"; then
	echo
	echo "*** Warning: the IBM ICU library could not be found!"
	echo "*** Please get a copy from http://http://oss.software.ibm.com/icu/"
	echo
	xcdr_fail="yes"
	xcdr_libfail="$xcdr_libfail -licuuc -licudata"
	exit 1
fi

AC_CHECK_LIB(xml4c, main)

if test "$ac_cv_lib_xml4c_main" != "yes"; then
	echo
	echo "*** Warning: the XML4c library could not be found!"
	echo "*** Please get a copy from http://www.alphaworks.ibm.com/tech/xml4c"
	echo
	xcdr_fail="yes"
	xcdr_libfail="$xcdr_libfail -lxml4c"
	exit 1
fi


if test $xcdr_fail != "no"; then
	echo 
	echo "*** Exitting due to dependency failure."
	echo "*** (Link lines required: \"$xcdr_libfail\")"
	echo
	exit 1
fi

AC_CHECK_LIB(termcap, main)
AC_CHECK_LIB(readline, main)

dnl ----------------------------------------------------------------------------
dnl
dnl   CONSTANTS DEFINED FOR src/include/version.h
dnl   (some of these are probably not very portable...)
dnl
dnl ----------------------------------------------------------------------------

XCDR_VERSION_MAJOR="$GENERIC_MAJOR_VERSION"
XCDR_VERSION_MINOR="$GENERIC_MINOR_VERSION"
XCDR_VERSION_PATCHLEVEL="$GENERIC_MICRO_VERSION"
XCDR_VERSION_STRING="$XCDR_VERSION_MAJOR.$XCDR_VERSION_MINOR.$XCDR_VERSION_PATCHLEVEL"

XCDR_BUILD_COMPILER=`g++ --version > /tmp/g++version 2>&1; cat /tmp/g++version | grep -i g++; rm /tmp/g++version`
XCDR_BUILD_FLAGS=`echo $CXXFLAGS | sed -e 's/"/\\\\"/g'`
XCDR_BUILD_HOST=`uname -n`
XCDR_BUILD_OS=`uname -o`
XCDR_BUILD_KERNEL_REL=`uname -r`
XCDR_BUILD_KERNEL_VER=`uname -v`
XCDR_BUILD_ARCH=`/bin/arch`
XCDR_BUILD_PROCESSOR=`uname -p`

AC_SUBST(XCDR_VERSION_MAJOR)
AC_SUBST(XCDR_VERSION_MINOR)
AC_SUBST(XCDR_VERSION_PATCHLEVEL)
AC_SUBST(XCDR_VERSION_STRING)
AC_SUBST(XCDR_BUILD_COMPILER)
AC_SUBST(XCDR_BUILD_FLAGS)
AC_SUBST(XCDR_BUILD_HOST)
AC_SUBST(XCDR_BUILD_OS)
AC_SUBST(XCDR_BUILD_KERNEL_REL)
AC_SUBST(XCDR_BUILD_KERNEL_VER)
AC_SUBST(XCDR_BUILD_ARCH)
AC_SUBST(XCDR_BUILD_PROCESSOR)


dnl ----------------------------------------------------------------------------
dnl
dnl Generate makefiles and config script
dnl
dnl ----------------------------------------------------------------------------

AC_CONFIG_FILES([Makefile \
                 src/Makefile \
	         src/common/Makefile \
	         src/xml/Makefile \
	         src/rdbms/Makefile \
	         src/gateway/Makefile \
	         src/gateway/sonus/Makefile \
	         src/entity/Makefile \
	         src/rater/Makefile \
	         src/reader/Makefile \
	         src/mediator/Makefile \
	         src/engine/Makefile \
		 src/util/Makefile \
		 src/util/lerg/Makefile \
		 src/util/rmap/Makefile \
		 src/util/console/Makefile \
		 src/include/common/version.h \
	         test/Makefile \
		 xcdr.spec])
AC_OUTPUT
