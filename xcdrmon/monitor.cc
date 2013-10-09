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
// $Source: /usr/local/cvs/xcdr/xcdrmon/monitor.cc,v $
// $Revision: 1.2 $
// $Date: 2003/03/06 11:20:05 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: monitor.cc,v $
// Revision 1.2  2003/03/06 11:20:05  tmarsh
// updated headers
//
//
// =========================================================================

#include "monitor.h"
#include "../src/common/rdbms.h"

RDBMS::Context context;

class CDRTypeMap : public RDBMS::Cache::Base<unsigned int, string>
{
public:
	CDRTypeMap(RDBMS::Context *context = NULL,
		   unsigned int min_results = 1,
		   unsigned int max_results = 1,
		   unsigned int max_age = 3600)
		: RDBMS::Cache::Base<unsigned int, string>(context,
							   min_results,
							   max_results,
							   max_age) {}

	void buildQuery(RDBMS::Query &query, const unsigned int &input) const
	{
		query << "SELECT name FROM CDR_Types WHERE i_type = "
		      << input;
	}

	string processResults(RDBMS::ResultStore &results) const
	{
		return results[0][0];
	}
};


CDRTypeMap typemap(&context);

unsigned int Monitor::getCountCDR_Mediated()
{
	context.commit();
	RDBMS::Query query = context.createQuery();

	query << "SELECT COUNT(*) FROM CDR_Mediated";

	RDBMS::ResultStore results = query.store();

	return results[0][0];
}

unsigned int Monitor::getCountCDR_Raw()
{
	context.commit();
	RDBMS::Query query = context.createQuery();

	query << "SELECT COUNT(*) FROM CDR_Raw";

	RDBMS::ResultStore results = query.store();

	return results[0][0];
}

// pair<string, unsigned int> StringUint;


map<unsigned int, unsigned int> Monitor::getSpreadCDR_Raw()
{
	context.commit();
	RDBMS::Query query = context.createQuery();

	query << "SELECT COUNT(*), i_cdr_type FROM CDR_Raw GROUP BY i_cdr_type";

	RDBMS::ResultStore results = query.store();

	map<unsigned int, unsigned int> spread;

	RDBMS::ResultStore::iterator i = results.begin();

	while (i != results.end())
	{
		spread.insert(make_pair((*i)[1], (*i)[0]));
//		spread[(*i)[1]] = (*i)[0];
		i++;
	}
	for (unsigned int j = 0; j < 8; j++)
	{
		if (spread.count(j) == 0)
		{
			spread.insert(make_pair(j, 0));
		}
	}

	return spread;
}
