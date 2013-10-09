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
// $Source: /usr/local/cvs/xcdr/src/include/common/except.h,v $
// $Revision: 1.3 $
// $Date: 2003/04/06 00:24:44 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: except.h,v $
// Revision 1.3  2003/04/06 00:24:44  tmarsh
// added UnimplementedException
//
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

#ifndef XCDR_EXCEPT_H
#define XCDR_EXCEPT_H

#include "common/common.h"

#include <string>
#include <ostream>

XCDR_NAMESPACE_BEGIN

// ----------------------------------------------------------------------------
// Base Class for Exceptions
// ----------------------------------------------------------------------------

class Exception
{
public:
    Exception(const std::string& err_msg);
    virtual ~Exception() {}
    void print(std::ostream& o) const;
    virtual const std::string &what() const { return msg; }

protected:
    std::string msg;
};

std::ostream& operator << (std::ostream& o, const Exception& e);

// ----------------------------------------------------------------------------
// Development Exceptions
// ----------------------------------------------------------------------------

class UnimplementedException : public Exception
{
public:
	UnimplementedException(const char *file,
			       unsigned int line,
			       const char *function);
};

// ----------------------------------------------------------------------------
// Common Exception classes
// ----------------------------------------------------------------------------

class DirectoryException : public Exception
{
public:
	DirectoryException(const std::string& dir_name)
		: Exception("Can't open directory: " + dir_name) {}

};


class FileException : public Exception
{
public:
	FileException(const std::string& file_name)
		: Exception("Can't open file: " + file_name) {}
};

class FileMovedException : public Exception
{
public:
	FileMovedException(const std::string& file_name)
		: Exception("File moved while streaming: " + file_name) {}
};

class InvalidInputException : public Exception
{
public:
	InvalidInputException(const std::string& err_msg)
		: Exception("Invalid Input: " + err_msg) {}
};

class RemoteFileError : public Exception
{
public:
	RemoteFileError(const std::string& fname)
		: Exception("Remote File Exception: " + fname), filename(fname) {}

	std::string filename;
};

XCDR_NAMESPACE_END

#endif // XCDR_EXCEPT_H
