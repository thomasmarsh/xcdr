## Makefile for xcdr_reader created by pkgdata
## from ICU Version 2.4

NAME=xcdr_reader
CNAME=xcdr_reader
TARGETDIR=.
TEMP_DIR=.
srcdir=$(TEMP_DIR)
MODE=common
MAKEFILE=./xcdr_reader_common.mak
ENTRYPOINT=xcdr_reader
include /usr/local/lib/icu/2.4/Makefile.inc



TEMP_PATH=
TARG_PATH=
## List files [1] containing data files to process (note: - means stdin)
LISTFILES= pkglist.txt


## Data Files [1]
DATAFILES= root.txt


## Data File Paths [1]
DATAFILEPATHS= ./root.txt


# File to make:
TARGET=./xcdr_reader.dat

TARGETNAME=xcdr_reader.dat
# List file for gencmn:
CMNLIST=./xcdr_reader_common.lst

all: $(TARGET)

$(TARGET): $(CMNLIST) $(DATAFILEPATHS)
	$(INVOKE) $(GENCMN) -n $(CNAME) -c -d $(TARGETDIR) 0 $(CMNLIST)

$(CMNLIST): $(LISTFILES)
	cat $(LISTFILES) > $(CMNLIST)

CLEANFILES= $(CMNLIST) $(TARGET)

clean:
	-$(RMV) $(CLEANFILES) $(MAKEFILE)

install: $(TARGET)
	$(INSTALL_DATA) $(TARGET) $(INSTALLTO)/$(TARGETNAME)

