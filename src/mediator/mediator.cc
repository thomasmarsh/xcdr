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
// $Source: /usr/local/cvs/xcdr/src/mediator/mediator.cc,v $
// $Revision: 1.20 $
// $Date: 2003/05/29 09:09:02 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: mediator.cc,v $
// Revision 1.20  2003/05/29 09:09:02  tmarsh
// cleanups
//
// Revision 1.19  2003/04/04 16:06:36  tmarsh
// namespace changes
//
// Revision 1.18  2003/03/09 08:47:27  tmarsh
// include file reorganization
//
// Revision 1.17  2003/03/06 11:20:03  tmarsh
// updated headers
//
//
// =========================================================================

#include "common/activity.h"
#include "gateway/gateway.h"
#include "mediator/mediator.h"


XCDR_NAMESPACE_BEGIN

bool Mediator::haveSonusData()
{
	ENTER();
	getContext()->commit();
	RDBMS::Query query = getContext()->createQuery();

	query << "SELECT COUNT(*) FROM CDR_Raw WHERE commited = 1"
	      << " AND i_node_type = " << 1
	      << " AND (i_cdr_type = " << CDR::TYPE_STOP
	      << "   OR i_cdr_type = " << CDR::TYPE_ATTEMPT << ")";

	RDBMS::ResultStore results = query.store();

	DEBUG(query.preview());
	DEBUG("Returned: " << results[0][0]);
	if (((unsigned int ) results[0][0]) > 0)
	{
		if (!src_file.is_open())
			src_file.open("/tmp/callgen-sonus", std::ifstream::in);
		RETURN(1);
		return true;
	}
	RETURN(0);
	return false;
}

Gateway::CallList Mediator::separateCalls(RDBMS::ResultStore &r)
{
	ENTER();
	Gateway::CallList calls;

	std::string call_id, cdr, last="";
	unsigned int cdr_cache;

	char cache_buf[1024];

	RDBMS::ResultStore::iterator i;
	for (i = r.begin(); i != r.end(); i++)
	{
		call_id = (*i)[0];
		cdr_cache = (*i)[1];
		DEBUG("----------------------------------------------");
		DEBUG("--> call id: " << call_id);
		DEBUG("--> cache id: " << cdr_cache);

		src_file.seekg(cdr_cache);
		src_file.get(cache_buf, 1024);
		cdr = cache_buf;

		DEBUG("--> CDR: " << cdr);

		if (call_id != last)
		{
			calls.push_back(Gateway::Call());
		}
		calls.back().push_back(make_pair(call_id, cdr));
	}
	RETURN(calls.size());
	return calls;
}

// --------------------------------------------------------------------------
//
// METHOD:	Mediator::mediate
// RETURN:	void
// ARGUMENTS:	void
//
// DESCRIPTION: This is the main vehicle for Mediator logic. It retrieves
//		a batch of call records, passes them to a Gateway::Mediator
//		for pre-processing, and then calls localMediation to finish
//		the process. Processed call records are placed in in the
//		table CDR_Mediated.
//
// NOTES:	Some of this logic can be moved into separate methods.
//
// --------------------------------------------------------------------------

void Mediator::mediate()
{
	ENTER();
	std::list<std::string> to_delete;
	std::list<std::string> to_insert;

	//
	// Get all keys for completed calls (those that have
	// stops, or attempts)
	//

	RDBMS::Query query_keys = getContext()->createQuery();

	TODO("have the GW::Mediator provide the select statement");

	query_keys << "SELECT DISTINCT call_id FROM CDR_Raw"
	           << " WHERE commited = 1"
		   << " AND (i_cdr_type = " << CDR::TYPE_STOP
		   << "   OR i_cdr_type = " << CDR::TYPE_ATTEMPT << ")"
	           << " AND i_node_type = " << 1
		   << " LIMIT 500";

	DEBUG("Query keys: " << query_keys.preview());
	std::string keys = "(";

	RDBMS::ResultStore key_r = query_keys.store();

	RDBMS::ResultStore::iterator k = key_r.begin();
	while (k != key_r.end())
	{
		keys += "\"" + (*k)[0] + "\"";
		k++;
		keys += ((k != key_r.end()) ? ", " : "");
	}
	keys += ")";


	//
	// Pull all call_ids and CDR text for all keys (we will
	// sort all this information out later)
	// 

	RDBMS::Query query = getContext()->createQuery();

	query << "SELECT call_id, cdr_offset FROM CDR_Raw"
	      << " WHERE call_id IN " << keys
	      << " ORDER BY call_id, cdr_offset, cdr_time DESC";

	DEBUG("Query: " << query.preview());
	RDBMS::ResultStore results = query.store();


	//
	// Break out the raw listing into a CallList which
	// is a list of Call's. A Call being a list of call_id's
	// and cdr data.
	//

	Gateway::CallList calls = separateCalls(results);


	//
	// Ask the gateway mediator to mediate the gateway
	// specific bits (start/stop time, CLI/CLD, bytes
	// sent/received, etc..)
	//

	CDR::Mediated mrec;
	Gateway::CallList::iterator i = calls.begin();
	while (i != calls.end())
	{
		mrec = sonus_mediator.mediate(*i);

		localMediation(mrec);

		to_insert.push_back(mrec.toInsert());
		to_delete.push_back((*i).front().first);
		i++;
	}


	//
	// Build the insert statement for the mediated CDR's
	//

	std::string ins = "INSERT INTO CDR_Mediated VALUES ";
	std::list<std::string>::iterator j = to_insert.begin();
	while (j != to_insert.end())
	{
		ins += (*j);
		j++;
		ins += ((j != to_insert.end()) ? ", " : "");
	}


	//
	// Build the delete statement to clear processed call_ids.
	//

	std::string del = "DELETE FROM CDR_Raw WHERE";
	del += " commited = 1 AND call_id in (";
	j = to_delete.begin();
	while (j != to_delete.end())
	{
		del += "\"" + (*j) + "\"";
		j++;
		del += ((j != to_delete.end()) ? ", " : "");
	}
	del += ")";

	//
	// Execute the INSERT and DELETE statements and commit.
	//


	getContext()->begin();
	try
	{
		getContext()->execute(ins);
		getContext()->execute(del);

		getContext()->commit();

		INFO("Commited " << to_insert.size() << " calls");
		(*getStats())["calls"].update(to_insert.size());
	}
	catch (mysqlcppapi::ex_base &exc)
	{
		ERROR(exc.what());
		getContext()->rollback();
	}
	catch (Exception &exc)
	{
		ERROR(exc);
		getContext()->rollback();
	}
	catch (...)
	{
		ERROR("an error occured...");
		getContext()->rollback();
	}
	LEAVE();
}

void Mediator::wait() const
{
	sleep(1);
}

void Mediator::localMediation(CDR::Mediated &m)
{
	// Set call id
	
	TODO("set call id");

	m.i_call = generateActivity(1, 1, count);
	count++;

	// set i_ingress/i_egress
	
	TODO("i_ingress/i_egress");
	TODO("local timezone");

	TODO("set i_customer");
}


void Mediator::initialize()
{
	TODO("remove truncate");
	getContext()->execute("TRUNCATE TABLE CDR_Mediated");
	getContext()->commit();

	getStats()->addCounter("calls");
}

void Mediator::execute()
{
	if (haveSonusData())
	{
		if (isIdle())
		{
			INFO("calls available");
			setIdle(false);
		}
		mediate();
	}
	else
	{
		if (!isIdle())
		{
			INFO("mediator is idle");
			setIdle(true);
		}
	}
}

XCDR_NAMESPACE_END

int main()
{
	int retval = 0;
	try
	{
		XCDR::Mediator m(0);
		m.run();
	}
	catch (mysqlcppapi::ex_base &exc)
	{
		FATAL(exc.what());
		std::cerr << exc.what() << std::endl << std::flush;
		retval = 1;
	}
	catch (XCDR::Exception &exc)
	{
		EXCEPTION("FATAL", exc);
		std::cerr << exc << std::endl << std::flush;
		retval = 1;
	}
	catch (XCDR::Exception *exc)
	{
		EXCEPTION("FATAL", (*exc));
		std::cerr << (*exc) << std::endl << std::flush;
		retval = 1;
	}
	catch (...)
	{
		FATAL("unknown exception");
		std::cerr << "other exception" << std::endl << std::flush;
		retval = 1;
	}

	exit(retval);
}
