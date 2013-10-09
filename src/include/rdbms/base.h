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
// $Source: /usr/local/cvs/xcdr/src/include/rdbms/base.h,v $
// $Revision: 1.2 $
// $Date: 2003/04/04 16:03:58 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: base.h,v $
// Revision 1.2  2003/04/04 16:03:58  tmarsh
// namespace changes
//
// Revision 1.1  2003/03/09 08:35:32  tmarsh
// include file reorganization
//
// Revision 1.2  2003/03/07 13:27:07  tmarsh
// fixes to compile
//
// Revision 1.1  2003/03/07 11:26:44  tmarsh
// added
//
// Revision 1.5  2003/03/06 11:20:02  tmarsh
// updated headers
//
//
// =========================================================================

#ifndef XCDR_RDBMS_H
#define XCDR_RDBMS_H


#include "common/common.h"
#include "common/logging.h"
#include "common/except.h"

#include <mysqlcppapi/mysqlcppapi.h>
#include <ctime>


using XCDR::Exception;

XCDR_NAMESPACE_BEGIN

namespace RDBMS
{
	// XXX: TODO: Template this for mysqlcppapi and later Oracle
	typedef mysqlcppapi::Connection		Connection;
	typedef mysqlcppapi::Row		Row;
	typedef mysqlcppapi::Query		Query;
	typedef mysqlcppapi::Result_Store	ResultStore;
	typedef mysqlcppapi::Result_NoData	ResultNoData;

	class RDBMSException : public Exception
	{
	public:
		RDBMSException(const std::string &msg)
			: Exception(msg) {}
	};

	class NoResults : public RDBMSException
	{
	public:
		NoResults(const std::string &msg)
			: RDBMSException(msg) {}
	};

	class TooManyResults : public RDBMSException
	{
	public:
		TooManyResults(const std::string &msg)
			: RDBMSException(msg) {}
	};

	class TooFewResults : public RDBMSException
	{
	public:
		TooFewResults(const std::string &msg)
			: RDBMSException(msg) {}
	};

	// UTILITY

	std::string quote(const std::string &in);
}

XCDR_NAMESPACE_END

#endif // XCDR_RDBMS_H
