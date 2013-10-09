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
// $Source: /usr/local/cvs/xcdr/src/include/reader/cache/profile.h,v $
// $Revision: 1.3 $
// $Date: 2003/04/04 16:03:58 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: profile.h,v $
// Revision 1.3  2003/04/04 16:03:58  tmarsh
// namespace changes
//
// Revision 1.2  2003/03/09 08:44:12  tmarsh
// include file reorganization
//
// Revision 1.1  2003/03/09 08:35:32  tmarsh
// include file reorganization
//
// Revision 1.4  2003/03/06 11:20:03  tmarsh
// updated headers
//
//
// =========================================================================

#ifdef XCDR_PROFILE_H
#error "Header contains static declarations and can only be included once."
#else
#define XCDR_PROFILE_H

#include "rdbms/rdbms.h"

XCDR_NAMESPACE_BEGIN

namespace Reader
{
	using namespace mysqlcppapi;

	struct Profile
	{
		unsigned int maxLinesPerPoll;
		unsigned int readMaxTime;
		unsigned int refreshFrequency;
		unsigned int commitFrequency;
		unsigned int pollFrequency;
		unsigned int depositFrequency;
	};

	class ProfileCache : public RDBMS::Cache::Base<unsigned int, Profile>
	{
	public:
		inline ProfileCache(RDBMS::Context *context = NULL,
				    unsigned int min_results = 1,
				    unsigned int max_results = 1,
				    unsigned int max_age = 3600)
		: RDBMS::Cache::Base<unsigned int, Profile>(context,
							    min_results,
							    max_results,
							    max_age) {}

	protected:
		void buildQuery(RDBMS::Query &query,
				const unsigned int &input)
		{
			query << "SELECT * FROM ReaderProfile "
			      << "WHERE i_reader = " << input;
		}

		Profile processResults(RDBMS::ResultStore &results)
		{
			RDBMS::Row row = results[0];

			Profile p;
			p.maxLinesPerPoll = row["maxLinesPerPoll"];
			p.readMaxTime = row["readMaxTime"];
			p.refreshFrequency = row["refreshFrequency"];
			p.commitFrequency = row["commitFrequency"];
			p.pollFrequency = row["pollFrequency"];
			p.depositFrequency = row["depositFrequency"];

			DEBUG("got : " << p.maxLinesPerPoll);
			return p;
		}
	};

}

std::map<unsigned int, RDBMS::Cache::Data< Reader::Profile > >
	RDBMS::Cache::Base<unsigned int, Reader::Profile>::cache;

XCDR_NAMESPACE_END

#endif // XCDR_PROFILE_H
