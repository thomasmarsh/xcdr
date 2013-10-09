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
// $Source: /usr/local/cvs/xcdr/src/include/alert/alert.h,v $
// $Revision: 1.1 $
// $Date: 2003/03/09 08:35:31 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: alert.h,v $
// Revision 1.1  2003/03/09 08:35:31  tmarsh
// include file reorganization
//
//
// =========================================================================

#ifndef XCDR_ALERT_H
#define XCDR_ALERT_H

#include "xcdr/common/rdbms.h"

namespace Alert
{
	template<unsigned int ID, string NAME>
	struct AlertID
	{
		unsigned int id;
		string name;
		string table;

		AlertID()
		{
			id = ID;
			name = NAME;
			table = "AlertDetail_" + NAME;
		}
	};

	tempate<class REG>
	class AlertBase
	{
	public:
		virtual void insert(RDBMS::Query &sql) const = 0;

		void generate(RDBMS::Context *context)
		{
		}

		static REG alert_id;
	};
}

#endif // XCDR_ALERT_H
