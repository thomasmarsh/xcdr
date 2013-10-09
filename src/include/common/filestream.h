// ==========================================================================
//
// XCDR, Version 1.0
//
// Copyright (c) 2003 Thomas D. Marsh. All rights reserved.
//
// ==========================================================================
//
// Redistribution and use is subject to agreement with the author(s). See
// the doc/LICENSE file for more details.
//
// THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
// ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
// USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
// OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
// SUCH DAMAGE.
//
// =========================================================================
//
// $Source: /usr/local/cvs/xcdr/src/include/common/filestream.h,v $
// $Revision: 1.2 $
// $Date: 2003/04/02 20:34:25 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: filestream.h,v $
// Revision 1.2  2003/04/02 20:34:25  tmarsh
// added XCDR namespace
//
// Revision 1.1  2003/03/09 08:35:31  tmarsh
// include file reorganization
//
// Revision 1.2  2003/03/06 11:20:02  tmarsh
// updated headers
//
//
// =========================================================================


#ifndef XCDR_FILESTREAM_H
#define XCDR_FILESTREAM_H

#include "common/common.h"
#include "common/except.h"
#include "common/timer.h"


#include <iostream>
#include <string>
#include <cstdlib>

extern "C" {
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
}

// the maximum line lenth is 512 bytes
// the read buffer is 128 kb

#define DEFAULT_LINELEN		512

#define DEFAULT_READLEN		131072

XCDR_NAMESPACE_BEGIN


class FileStream
{
public:
    //
    // Exceptions
    //

    class IOError : public Exception
    {
    public:
	IOError(const std::string& file_name);
    };

    class EOFException : public Exception
    {
    public:
	EOFException();
    };


    class FileTruncation : public Exception
    {
    public:
	FileTruncation();
    };


    //
    // File descriptor states
    //

    typedef enum FileState
    {
	READ, APPEND, WRITE, CLOSED
    };


    //
    // Constructors/destructors
    //

    FileStream(std::string file_name, off_t intial_pos=0,
	       unsigned int rlen=DEFAULT_READLEN,
	       unsigned int llen=DEFAULT_LINELEN);


    virtual ~FileStream();


    //
    // FileStream functionality
    //
 
    void open(const FileState mode=READ) throw (IOError);
    void close();
    void move(const std::string& new_fname);

    time_t readLastModification();
    off_t readSize(bool force_stat = false);
    off_t getRecordedSize();


    // find() returns a pointer to the beginning of the next
    // newline (which should be terminated before use).
    // It scans from last offset to the first valid
    // character and returns that location. It zeros any
    // invalids encountered.

    char *find(char* start=0);


    // term() scans from start along the buffer until
    // it finds an invalid character, then zeroing out the
    // location and returning its position, effectively
    // terminating the string

    char *term(char* start=0);


    // reports if find() is able to locate a validly
    // terminated record which is considered a line.
    // A client invokes this before calling readline()
    // in order to guarantee availability from the file.

    bool hasData();


    // readline returns a complete line or an empty
    // string if there is no data available

    std::pair<off_t, std::string> readline();


    // accessors
    off_t getOffset() const;


    std::string getFilename() const { return filename; }

    inline bool isOpen() const { return (fd > 0); }
    inline bool isIdle() const { return idle_flag; }

protected:
    // read timer; if reading at eof, we should only
    // do another OS read when elapsed time is greater than
    // one minute (this is NFS' configured cache write time)

    struct stat stat(bool force_stat = false)
	    throw (FileException,FileMovedException, RemoteFileError);

    struct stat stat_cache;
    bool eof_wait;
    Timer read_timer;
    bool timer_initialized;
    bool idle_flag;

    off_t recorded_size;

    off_t getEOF() throw (FileTruncation);

    bool hasLine();

    bool hasInsertRoom();
    bool hasForwardRoom();

    std::size_t _read();

    unsigned int swap();

    void handleError();
    void handleTruncation();

    bool has_eol();

    std::string filename;
    int fd;
    off_t current_pos, end_pos;
    FileState file_state;

    // the signature as of instantiation (used to determine if a rollover has occurred)
    std::string sig_cache;

    bool incomplete;

    char *buf;
    unsigned int read_pos;

    // this is the offset into the buffer
    unsigned int offset;

    // this is the total number of bytes that have been read from the file
    off_t total_bytes;

    // the number of empty bytes at the end of the buffer
    unsigned int empty_bytes;

    unsigned int end;

    std::size_t bytes;

    unsigned int read_len;
    unsigned int line_len;
};

XCDR_NAMESPACE_END

#endif // XCDR_FILESTREAM_H
