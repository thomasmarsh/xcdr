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
// $Source: /usr/local/cvs/xcdr/src/common/filestream.cc,v $
// $Revision: 1.5 $
// $Date: 2003/04/02 20:35:51 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: filestream.cc,v $
// Revision 1.5  2003/04/02 20:35:51  tmarsh
// added XCDR namespacing; eradicated all using namespace std
//
// Revision 1.4  2003/03/09 08:26:12  tmarsh
// include file reorganization
//
// Revision 1.3  2003/03/06 11:20:02  tmarsh
// updated headers
//
//
// =========================================================================

#include "common/filestream.h"
#include "common/logging.h"
#include "common/util.h"

#include <errno.h>

//#define NFS_CACHE_DELAY	10
#define NFS_CACHE_DELAY	5

#undef DEBUG
#define DEBUG(x)

XCDR_NAMESPACE_BEGIN

void FileStream::move(const std::string& new_source)
{
    ENTER();
    filename = new_source;

    timer_initialized = false;
    if (isOpen())
    {
        off_t pos = lseek(fd, 0, SEEK_CUR);
	::close(fd);
	fd = 0;

        mode_t flags = O_SYNC | O_RDONLY;

        DEBUG("about to open file: " << filename);
        fd = ::open(filename.c_str(), flags, 00644);
        DEBUG("opened, seeking to " << offset);
        lseek(fd, pos, SEEK_SET);
    }

    LEAVE();
}

FileStream::FileStream(std::string fn, off_t initial_pos,
		       unsigned int rlen, unsigned int llen)
{
    DEBUG("FileStream::FileStream(\""
		    << fn << "\", "
		    << Util::itoa((int) initial_pos)
		    << ")");

    filename = fn;
    
    timer_initialized = false;
    stat_cache.st_ino = 0;
    stat();
    recorded_size = 0;

    offset = 0;
    total_bytes = initial_pos;
    empty_bytes = rlen;
    bytes = 0;
    fd = 0;

    incomplete = false;

    read_len = rlen;
    line_len = llen;

    DEBUG("Constructing a filestream with rlen: '" << rlen << "'");

    // allocate buffer and set beginning and ending pointers
    buf = new char[rlen + 1];
    read_pos = 0;
    end = read_len;

    // clear buffer
    DEBUG("!!!!!!!!!!!! CLEAR BUF !!!!!!!!!!!! ");
    memset(buf, 0, rlen);

    // open the file
    try
    {
    	open();
    }
    catch (Exception e)
    {
	DEBUG(std::string("Dying in FileStream constructor (file='") << fn << "') " << e);
	exit(-1);
    }
}


FileStream::~FileStream()
{
    if (isOpen()) close();

    if (buf != NULL) delete(buf);
}


off_t FileStream::readSize(bool force_stat)
{
    recorded_size = stat(force_stat).st_size;
    return recorded_size;
}

off_t FileStream::getRecordedSize()
{
    return recorded_size;
}


FileStream::IOError::IOError(const std::string& file_name)
  : Exception( "IOError: " + file_name + ", " + strerror(errno) )
{}


FileStream::EOFException::EOFException()
  : Exception("eof") {}


FileStream::FileTruncation::FileTruncation()
  : Exception("file truncated") {}



time_t FileStream::readLastModification()
{
    return stat().st_ctime;
}


struct stat FileStream::stat(bool force_stat)
	throw (FileException, FileMovedException, RemoteFileError)
{
    ENTER();
    // this invokes a delay of up to NFS_CACHE_DELAY seconds.
    // the trick is to set this low enough that you aren't waiting
    // too long sitting idle, but at the same time not be hitting
    // the nfs file systems so hard with constant requests.
    //
    // This could be fixed by synchronizing the time on the two
    // machines with something like NTP, and ensuring that NFS_CACHE_DELAY
    // and the NFS server on the NT and solaris boxes are all
    // configured with a similar caching.
    //
    // a better connection between the machines (on the order gigabit)
    // would eliminate the need for any timing.

    if ((!timer_initialized)
	 || (read_timer.elapsed() > NFS_CACHE_DELAY)
	 || (force_stat))
    {
	ino_t old_inode = stat_cache.st_ino;

	try 
	{
		stat_cache = Util::stat(filename);
	}
	catch (FileException& e)
	{
		close();
		if (old_inode != 0)
		{
			throw FileMovedException(filename);
		}
		else
		{
			throw e;
		}
	}
	catch (RemoteFileError& e)
	{
		DEBUG("Caught RemoteFileError in FileStream:;stat, throwing it again: " << e);
		throw e;
	}

	if ((old_inode != 0) && (old_inode != stat_cache.st_ino))
	{
		close();
		throw FileMovedException(filename);
	}

	timer_initialized = true;
	read_timer.reset();
    }
    DEBUG("stat result: <" << stat_cache.st_mtime << ":" << stat_cache.st_size);

    LEAVE();

    return stat_cache;
}


//
// open
//

void FileStream::open(FileState mode) throw (IOError)
{
    ENTER();

    if (fd > 0)
    {
        if (mode == file_state)
	{
            // nothing to do
	    LEAVE();
            return;
	}
        else
            close();
    }

           
    // File syncronization
    //
    // see 'man -s 2 open' and read sections on O_DSYNC,
    // O_RSYNC, O_SYNC, and O_NONBLOCK/O_NDELAY.

    mode_t flags = O_SYNC;
    
    switch (mode)
    {
    case READ:
        flags |= O_RDONLY;
        break;

    case APPEND:
        flags |= (O_WRONLY | O_CREAT | O_APPEND);
        break;

    case WRITE:
        flags |= (O_WRONLY | O_TRUNC | O_CREAT);
        break;

    default:
        break;
    }

    DEBUG("about to open file: " << filename);
    fd = ::open(filename.c_str(), flags, 00644);
    if (fd <= 0)
    {
	IOError err("open: " + filename);

	RETURN(err);
        throw err;
    }

    DEBUG("opened, seeking to " << total_bytes);
    off_t x = lseek(fd,total_bytes,SEEK_SET);

    if (x < 0)
    {
	IOError err("lseek: " + filename);

	RETURN(err);
        throw err;
    }

    DEBUG("seeked to " << x);
    LEAVE();
}


void FileStream::close()
{
    DEBUG("close");

    if (fd > 0)
	::close(fd);
    fd = 0;
}


off_t FileStream::getOffset() const
{
    return (((total_bytes + empty_bytes) - (read_len - offset)) + 2);
}


off_t FileStream::getEOF() throw (FileTruncation)
{
    DEBUG("getEOF");

    off_t current_pos = getOffset();
    end_pos = lseek(fd, (off_t) 0, SEEK_END);

    lseek(fd, current_pos, SEEK_SET);

    if (current_pos > end_pos)
        throw FileTruncation();

    return end_pos;
}


void FileStream::handleError()
{
    ENTER();

    DEBUG(filename << ": " << strerror(errno));
    switch (errno)
    {
#ifdef SOLARIS2
    case EDEADLK:
    case EINTR:
#endif // SOLARIS2
    case EAGAIN:
        DEBUG("no data");
        break;

    // reopen cases
    case EBADF:
        DEBUG("reopen");

        close();
        open(READ);
        break;


    // fatal cases
    case EFAULT:
    default:
	RETURN("IOError");
	throw IOError("handleError switch");
        break;
    }

    LEAVE();
    return;
}


// Any character outside of printable ascii
// range is considered the end of a potential
// line. (This includes newlines and carriage
// returns.)

bool isTerminator(char c)
{
    return ((c != '\0') && ((c == '\r') || (c == '\n')));
}


char* FileStream::find(char* pos)
{
    ENTER();

    DEBUG("FileStream::find 1");

    if (!pos)
        pos = (buf + offset);

    DEBUG("- buf = " << (unsigned int) buf);
    DEBUG("- pos = " << (unsigned int) pos);
    DEBUG("- offset = " << offset);

    if (pos >= (buf + end))
    {
	DEBUG("past end of buf");
	offset = 0;

	LEAVE();
	RETURN(0);
	return 0;
    }

    DEBUG("FileStream::find 2");

    char *initial = pos;
    while ((*pos == '\0') || isTerminator(*pos))
    {
	// exceded line size
	if ((pos-initial) > (signed) line_len)
	{
	    DEBUG("big line");
	    //offset = 0;

	    RETURN(0);
	    return 0;
	}

	// exceded max position
	DEBUG("pos: " << (unsigned int)pos << ", buf + end: " << (unsigned int)(buf+end));
	if ((pos + 1) > (buf+end))
	{
	    DEBUG("end of buf");
	    offset = 0;

	    RETURN(0);
	    return 0;
	}

	DEBUG("FileStream::find 3");
	*pos = '\0';
	pos++;
    }

    DEBUG("FileStream::find 4");

    offset = (pos-buf);
    DEBUG("- offset = " << offset);

    RETURN((unsigned int) &pos);
    return pos;
}


char* FileStream::term(char* pos)
{
    ENTER();

    char* xs = 0;

    if (!pos)
    {
	pos = find();

        if (!pos)
	{
	    RETURN(0);
	    return 0;
	}
    }

    xs = pos + 1;
    DEBUG("Start+1 = " << (unsigned int)xs);

    if (xs >= (buf+end))
    {
	RETURN(0);
	return 0;
    }

    //DEBUG("got here");

    int my_i = 0;
    while (!isTerminator(*xs))
    {
	// exceded line size
//	if ((xs-pos) > (signed) line_len)
//	{
//	    DEBUG("Broke1 : xs - pos =" << (unsigned int) (xs-pos));
//	    RETURN(0);
//	    return 0;
//	}

	// exceded max position
	if ((xs+1) > (buf+end))
	{
           DEBUG("Broke2");
	   RETURN(0);
	   return 0;
	}

	if (*xs == '\0')
	{
	    DEBUG("end of input (" << my_i << " spread)");
	    RETURN(0);
	    return 0;
	}

	xs++;
	my_i++;
    }

    RETURN((unsigned int) &xs);
    return xs;
}



bool FileStream::hasData()
{
    //ENTER("FileStream::hasData");

    if (hasLine())
    {
	RETURN(true);
	return true;
    }

    size_t count=0;

    if (!isOpen()) { return false; }
    if (!hasLine())
    {
	unsigned int leftover = 0;
        if (!hasInsertRoom())
	{
 	    leftover = swap();
	}

	count = _read();

	if (count == 0)
	{
	    DEBUG("no read data");
	    //DEBUG(find());
	    RETURN(false);
	    return false;
	}
	else
	{
	    DEBUG("read " << (int) count << " bytes");
	    total_bytes += count;
	    empty_bytes = read_len - (count + leftover + 2);
	}
    }

    DEBUG("******Here is buf: " << buf);
    if (hasLine())
    {
	RETURN(true);
	return true;
    }
    DEBUG("No hasline!\n");

    RETURN(false);
    return false;
}


std::pair<off_t, std::string> FileStream::readline()
{
    ENTER();
    DEBUG("FILENAME=" << filename);

    std::pair<off_t, std::string> rv;

    static int lineno = 0;
    lineno++;
    DEBUG("line = " << lineno);

    if (!hasData())
    {
        RETURN("std::pair<off_t,std::string>(0, \"\")");
	idle_flag = true;
	return std::pair<off_t,std::string>(0,"");
    }

    char *s = find();
    char *xs = term(s);
    assert(s && xs);

    *xs = '\0';

    rv.first = getOffset();

    bytes = (unsigned int) (xs-buf+read_pos);
    offset = (unsigned int) (xs-buf);

    char lbuf[line_len+1];

    memset((char *) &lbuf, 0, line_len);

    int len = snprintf((char *) &lbuf, line_len+1, "%s", s);

    if (len > (signed) line_len)
    {
	DEBUG("long line: " << len);
    }

    rv.second = lbuf;

    RETURN(rv.second);
    return rv;
}


unsigned int FileStream::swap()
{
    ENTER();

    char* pos = find();

    DEBUG("pos: " << (unsigned int)(pos));

    bytes = 0;

    if ((pos) && (pos != buf))
    {
	unsigned int i = 0;

	while (*(pos+i) != '\0')
	{
	    if (pos >= (buf+read_pos)) break;

	    DEBUG("setting buf[" << i << "] = '" << *(pos+i) << "' (" << (int) *(pos+i) << ")");

            *(buf+i) = *(pos+i);

	    i++;
	    bytes++;
	}

	memset((buf + i), 0, (read_len - i));

	read_pos = bytes;
    }
    else // (!pos)
    {
	DEBUG("------------ CLEAR BUF ---------------- ");
	pos = buf;
	memset(buf, 0, read_len);

	read_pos = 0;
    } 
    offset = 0;

    DEBUG("bytes end: " << (unsigned int)(bytes))
    LEAVE();
    return(bytes);
}


bool FileStream::hasForwardRoom()
{
    TRACE(">");
    return ((line_len + ((int) buf)) < (unsigned) offset);
}


bool FileStream::hasInsertRoom()
{
    ENTER();

    DEBUG("Calculating hasroom.  end: " << end << ", read_pos: " << read_pos << ", line_len: " << line_len);
    bool hasroom = (end - read_pos > line_len);

    RETURN(hasroom);
    return hasroom;
}


bool FileStream::hasLine()
{
    ENTER();

    char* pos = find();

    if (pos)
    {
	DEBUG("- have start: " << (int) pos - (int) buf);
    }
    else
    {
	DEBUG("- no start: " << (int) pos);
    }

    char* xs = term(pos);

    if (xs)
    {
	DEBUG("- have end: " << (int) xs - (int) buf);
    }
    else
    {
	DEBUG("- no end: " << (int) xs);
    }

    RETURN((pos && xs));
    return (pos && xs);

//    return (pos && term(pos));
}


size_t FileStream::_read()
{
    ENTER();

    assert (hasInsertRoom());
    bool force_stat = false;
    if (recorded_size > total_bytes)
    {
	force_stat = true;
    }

    ssize_t count = 0;

    if (readSize(force_stat) > total_bytes) {
    	size_t rlen = (end-read_pos-2);

    	DEBUG("calling read(" << fd << ", " << (unsigned int) buf+read_pos << ", " << rlen << ")");

    	count = ::read(fd, (buf+read_pos), rlen);

    	if (count == 0)
    	{
		idle_flag = true;
		RETURN(0);
        	return 0;
    	}

    	DEBUG("******Here is pos: " << *(buf+read_pos));

    	if (count == -1)
    	{
		DEBUG("negative bytes");
        	try
        	{
	    		DEBUG("error");
            		handleError();
        	}
        	catch (IOError& error)
        	{
            		DEBUG("IOError: " << error);
        	}
        	count = 0;
    	}

    	//memset(buf+read_pos+count+1, 0, end-(read_pos+count)+1);
    	memset(buf+read_pos+count+1, 0, end-(read_pos+count));

    	DEBUG("adding " << count << " bytes");

    	bytes += (size_t) count;

    	read_pos += count+1;
    }

    RETURN(count);
    return (size_t) count;
}

XCDR_NAMESPACE_END
