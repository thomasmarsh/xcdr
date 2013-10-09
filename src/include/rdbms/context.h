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
// $Source: /usr/local/cvs/xcdr/src/include/rdbms/context.h,v $
// $Revision: 1.2 $
// $Date: 2003/04/04 16:03:58 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: context.h,v $
// Revision 1.2  2003/04/04 16:03:58  tmarsh
// namespace changes
//
// Revision 1.1  2003/03/09 08:35:32  tmarsh
// include file reorganization
//
// Revision 1.1  2003/03/07 13:27:20  tmarsh
// added
//
//
// =========================================================================

#ifndef XCDR_RDBMS_CONTEXT_H
#define XCDR_RDBMS_CONTEXT_H

#include "common/common.h"
#include "rdbms/base.h"

#include <mysqlcppapi/mysqlcppapi.h>

XCDR_NAMESPACE_BEGIN

namespace RDBMS
{
	/**
	 * <p>The RDBMS Context is the primary interface for direct database
	 * interaction.</p>
	 */

	class Context
	{
	public:
		/** @name Constructor and destructor */
		//@{
		Context();
		~Context();
		//@}

		mysqlcppapi::Connection *getConnection();
		void execute(const std::string &sql);
		void begin();
		void rollback();
		void commit();
		inline Query createQuery() { return getConnection()->create_Query(); }
	private:

		mysqlcppapi::Connection *connection;
	};

	std::string quote(const std::string &in);

	Context *GetContext();

	extern Context *_GlobalContext;
}

XCDR_NAMESPACE_END

#endif // XCDR_RDBMS_CONTEXT_H
