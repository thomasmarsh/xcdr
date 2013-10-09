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
// $Source: /usr/local/cvs/xcdr/src/include/common/util.h,v $
// $Revision: 1.4 $
// $Date: 2003/04/02 20:34:25 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: util.h,v $
// Revision 1.4  2003/04/02 20:34:25  tmarsh
// added XCDR namespace
//
// Revision 1.3  2003/03/15 21:09:50  tmarsh
// added dtoa
//
// Revision 1.2  2003/03/09 18:40:13  tmarsh
// added toupper/tolower - maybe not working
//
// Revision 1.1  2003/03/09 08:35:31  tmarsh
// include file reorganization
//
// Revision 1.5  2003/03/07 22:28:55  tmarsh
// added libxcdrXML
//
// Revision 1.4  2003/03/06 11:20:02  tmarsh
// updated headers
//
//
// =========================================================================

#ifndef XCDR_UTIL_H
#define XCDR_UTIL_H

#include "common/except.h"
#include "common/logging.h"

#include <string>
#include <list>
#include <algorithm>

#include <cctype>
#include <cstdlib>

extern "C"
{
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
}

XCDR_NAMESPACE_BEGIN

#if 0
#define ENTER(x) {}
#define DEBUG(x) {}
#endif

namespace Util
{
	using namespace std;

	inline const std::string itoa(const int n, const char *fmt = "%d")
	{
		char buf[20];

		snprintf((char *) &buf, 20, fmt, n);

		return std::string(buf);
	}

	inline const std::string itox(const int n, unsigned int l)
	{
		char buf[20];

		std::string format = "%0" + itoa(l) + "X";
		snprintf((char *) &buf, 20, format.c_str(), n);

		return std::string(buf);
	}

	inline const std::string dtoa(const double n)
	{
		char buf[20];

		snprintf((char *) &buf, 20, "%g", n);

		return std::string(buf);
	}

	inline const std::string uitoa(const unsigned int n)
	{
		char buf[20];

		snprintf((char *) &buf, 20, "%d", n);

		return std::string(buf);
	}

	void writePID(const std::string& module, const std::string& home_dir);

	std::string strip(const std::string& rhs);
	std::string stripnl(const std::string& rhs);

	inline std::string toupper(std::string &input)
	{
		std::string output;
		transform(input.begin(),
			  input.end(),
			  output.begin(),
			  (int(*)(int)) toupper);
		return output;
	}

	inline std::string tolower(std::string &input)
	{
		std::string output;
		transform(input.begin(),
			  input.end(),
			  output.begin(),
			  (int(*)(int)) tolower);
		return output;
	}


	inline const std::string pad_zeros(const std::string& input,
				      const unsigned int padded_size = 0)
	{
		std::string rv(input);
		while (rv.length() < padded_size)
		{
			rv = "0" + rv;
		}

		return rv;
	}


	inline const std::string itoahex(const int n,
			            const unsigned int padded_size = 0,
				    bool lower_case = false)
	{
	    char buf[200];
	
	    if (lower_case) { 
	    	snprintf(buf, 200, "%x", n);
	    } else {
	    	snprintf(buf, 200, "%X", n);
	    }
	    std::string rv(buf);
	
	    return pad_zeros(rv,padded_size);
	}
	
	inline const int ahextoi(const std::string& str)
		throw (InvalidInputException)
	{
		std::string temp = strip(str);
	    int val = 0;
	    int success = sscanf(temp.c_str(),"%x",&val);
	    if (success < 0 || success == EOF) {
		throw InvalidInputException("Could not find a hex value in : '" + temp + "'");
	    }
	
	    return val;
	}


	inline int open_file(const std::string& filename, int oflag, mode_t mode = 0666)
	{
		int rv = open(filename.c_str(), oflag, mode);
		int i = 0;
		while (rv < 0 && i < 5)
		{
			rv = open(filename.c_str(), oflag, mode);
			i++;
		}
		if (rv < 0)
		{
			throw FileException("opening file: " + filename);
		}
		return rv;
	}


	inline struct stat stat(const std::string& filename) throw (FileException, RemoteFileError)
	{
	    ENTER();
	    int fd;
	    
	    try
	    {
		fd = open_file(filename, O_RDONLY);
	    }
	    catch (FileException& e)
	    {
		DEBUG("errno: " << errno);
		DEBUG("std::string errno: " << strerror(errno));
		if(errno == ETIMEDOUT)
		{
		    DEBUG("Throwing RemoteFileError because of OPEN");
		    throw RemoteFileError(filename);
		}
		//FileException err(filename + ": " + strerror(errno));
		DEBUG("Throwing FileException because of OPEN error: "
		      << errno << " " << strerror(errno));
		FileException err(filename);
		RETURN(err);
		throw err;
    	}

	    struct stat st;
	    int ret = fstat(fd, &st);

	    close(fd);

   	 if (ret != 0)
	    {
		DEBUG("errno: " << errno);
		DEBUG("std::string errno: " << strerror(errno));
		if (errno == ETIMEDOUT)
		{
			DEBUG("Throwing RemoteFileError because of FSTAT");
			throw RemoteFileError(filename);
		}
		//FileException err(filename + ": " + strerror(errno));
		DEBUG("Throwing FileException because of STAT failure");
		FileException err(filename);
		RETURN(err);
		throw err;
	    }
	
	    LEAVE();
	    return st;
	}
	
	inline void copy_file(const std::string& lhs, const std::string& rhs)
	{
		char buffer[16 * 1024];
		int in_file, out_file, read_size;
	
		in_file = open_file(lhs.c_str(), O_RDONLY);
		out_file = open_file(rhs, O_WRONLY | O_TRUNC | O_CREAT, 0666);
	
		while (1)
		{
			read_size = read(in_file, buffer, sizeof(buffer));
			if (read_size == 0) break;
	
			if (read_size < 0)
			{
				throw FileException("copying file: " + lhs);
			}
	
			write(out_file, buffer, (unsigned int) read_size);
		}
	
		close(in_file);
		close(out_file);
	
		return;
	}
	
	inline void cat_files(std::list<std::string>& files_to_cat, const std::string& out_file)
	{
		char buffer[16 * 1024];
		int in_fd, out_fd, read_size;

		out_fd = open_file(out_file, O_WRONLY | O_TRUNC | O_CREAT, 0666);
		std::list<std::string>::iterator i = files_to_cat.begin();
		while(i != files_to_cat.end())
		{
			in_fd = open_file((*i), O_RDONLY);
			while(1)
			{
				read_size = read(in_fd, buffer, sizeof(buffer));
				if (read_size == 0) break;
				if (read_size < 0)
				{
					throw FileException("catting file: " + (*i)
							    + " --> " + out_file);
				}
				::write(out_fd, buffer, (unsigned int) read_size);
			}
			i++;
	
			close(in_fd);
		}
		close(out_fd);
		return;
	}

	inline void move_file(const std::string& lhs, const std::string& rhs)
	{
		::rename(lhs.c_str(), rhs.c_str());
	}
	
	inline void remove_file(const std::string& lhs)
	{
		::unlink(lhs.c_str());
	}
	
	
	inline bool isDir(const std::string& filename)
	{
		ENTER();
	
		struct stat st;
		try
		{
			DEBUG("isdir 1");
			st = stat(filename);
			DEBUG("isdir 2");
		}
		catch (...)
		{
			DEBUG("isdir 3");
			RETURN(false);
			return false;
		}
		DEBUG("isdir 4");
	
		RETURN(S_ISDIR(st.st_mode));
		return S_ISDIR(st.st_mode);
	}
	
	
	inline bool isFile(const std::string& filename)
	{
		ENTER();
	
		struct stat st;
		try
		{
			st = stat(filename);
		}
		catch (Exception& e)
		{
			RETURN(false);
			return false;
		}
		catch (...)
		{
			RETURN(false);
			return false;
		}
	
		RETURN(S_ISREG(st.st_mode));
		return S_ISREG(st.st_mode);
	}
	
	inline std::string getenv(const std::string &var)
	{
		std::string value = ::getenv(var.c_str());
		TODO("Check the return value...");
		return value;
	}

	struct LookupTables
	{
		std::string qtel;
		std::string lerg;
		std::string supplementary;
		std::string international;
	};
}

XCDR_NAMESPACE_END

#endif // XCDR_UTIL_H
