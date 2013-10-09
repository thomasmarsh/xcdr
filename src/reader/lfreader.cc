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
// $Source: /usr/local/cvs/xcdr/src/reader/lfreader.cc,v $
// $Revision: 1.12 $
// $Date: 2003/04/04 16:06:36 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: lfreader.cc,v $
// Revision 1.12  2003/04/04 16:06:36  tmarsh
// namespace changes
//
// Revision 1.11  2003/03/27 19:20:02  tmarsh
// re-added repository stuff
//
// Revision 1.10  2003/03/19 00:03:03  tmarsh
// take out broken repository
//
// Revision 1.9  2003/03/09 08:42:15  tmarsh
// include file reorganization
//
// Revision 1.8  2003/03/06 11:20:03  tmarsh
// updated headers
//
//
// =========================================================================

#include "common/logging.h"
#include "common/cdr.h"
#include "reader/lfreader.h"
#include "gateway/sonus/sonus.h"

#include <string>
#include <list>

XCDR_NAMESPACE_BEGIN

void LocalFileReader::shutdown()
{
	ENTER();

	LEAVE();
}

unsigned int LocalFileReader::rawCount()
{
	getContext()->commit();

	std::string query = "rawCount";

	RDBMS::ResultStore results = query_handle->query(query);

	return results[0][0];
}

void LocalFileReader::initialize()
{
	ENTER();
	INFO("Loading reader repository");

	std::string repository_name = "foo";
	std::string group_name = "Reader";

	repository = new RDBMS::Repository(repository_name);
	query_handle = repository->getGroup(group_name);

	INFO("Clearing uncommited entries from previous run");
	INFO("creating FileStream");

	//
	// Set up filestreams and truncate database
	//

	TODO("Use real logic here");
	
	stream = new FileStream("/tmp/callgen-sonus");

	getContext()->begin();
	getContext()->execute("TRUNCATE TABLE ReaderLog");
	getContext()->commit();
	getContext()->begin();
	getContext()->execute("TRUNCATE TABLE CDR_Raw");
	getContext()->commit();

	//
	// Allocate node map
	//

	node_map = new NodeMap(getContext());
	profile_cache = new Reader::ProfileCache(getContext());

	//
	// Get application profile from database
	//

	loadProfile();

	//
	// Counters
	//

	getStats()->addCounter("lines");

	//
	// Setup thresholds
	//

	refresh_threshold = new Threshold(profile.refreshFrequency);
	commit_threshold = new Threshold(profile.commitFrequency);
	poll_threshold = new Threshold(profile.pollFrequency);
	deposit_threshold = new Threshold(profile.depositFrequency,
					  &line_count);

	refresh_threshold->reset();
	commit_threshold->reset();
	poll_threshold->reset();
	deposit_threshold->reset();

	LEAVE();
}

LocalFileReader::~LocalFileReader()
{
	if (node_map)
		delete node_map;

	if (refresh_threshold)
		delete refresh_threshold;

	if (commit_threshold)
		delete commit_threshold;

	if (poll_threshold)
		delete poll_threshold;

	if (deposit_threshold)
		delete deposit_threshold;
}

LocalFileReader::LocalFileReader(unsigned int id)
	: ReaderBase(id)
{
	ENTER();
	INFO("Constructing LocalFileReader");

	last_offset = 0;
	stream = NULL;
	filename = "data";
	counter = 0;
	line_count = 0;

	commit_pending = false;
	waiting = false;
	idle_flag = false;
	stream_idle = false;

	node_map = NULL;
	profile_cache = NULL;

	refresh_threshold = NULL;
	commit_threshold = NULL;
	poll_threshold = NULL;
	deposit_threshold = NULL;

	LEAVE();
}

void LocalFileReader::loadProfile()
{
	ENTER();

	profile = profile_cache->get(getID());

	DEBUG("maxLinesPerPoll: " << profile.maxLinesPerPoll);
	LEAVE();
}


void LocalFileReader::poll()
{
	ENTER();

	unsigned int local_count = 0;
	Threshold threshold(Threshold::T_OR,
			    profile.maxLinesPerPoll, &local_count,
			    profile.readMaxTime);


	while (!threshold.reached())
	{
		std::pair<off_t,std::string> line_data = stream->readline();

		if (line_data.second != "")
		{
			stream_idle = false;
			line_count++;
			local_count++;
			line_buffer.push_back(line_data);
		}
		else
		{
			stream_idle = true;
			break;
		}
	}
	LEAVE();
}


unsigned int LocalFileReader::deposit()
{
	ENTER();
	// deposit line_buffer to the database
	
	std::string sql("INSERT INTO CDR_Raw VALUES\n\t");

	int count = 0;

	std::pair<off_t,std::string> line;
	while (line_buffer.size() > 0)
	{
		line = line_buffer.front();
		Sonus::Parser parser(line.second);
			   //2003-02-03 10:07:00\", " // cdr_time

		unsigned int i_node = 0;
		unsigned int i_type = 0;

		if (parser.cdrType() == CDR::TYPE_STOP)
		{
			NodeMapData data = node_map->get(parser[Sonus::Stop::gateway_name]);
			i_node = data.i_node;
			i_type = data.i_type;
		}
		if (parser.cdrType() == CDR::TYPE_START)
		{
			NodeMapData data = node_map->get(parser[Sonus::Start::gateway_name]);
			i_node = data.i_node;
			i_type = data.i_type;
			if (i_type == 0)
			{
				ERROR(parser[Sonus::Start::gateway_name] <<
						"got type 0 on start");
			}
		}
		if (parser.cdrType() == CDR::TYPE_ATTEMPT)
		{
			NodeMapData data = node_map->get(parser[Sonus::Attempt::gateway_name]);
			i_node = data.i_node;
			i_type = data.i_type;
		}


		sql += "(" + Util::uitoa(getID()) + ", "
			   + Util::uitoa(i_node) + ", " // i_node
			   + Util::uitoa(i_type) + ", " // i_node
			   + Util::uitoa(parser.cdrType()) + ", "
			   + "NULL, "   // r_timestamp=auto
			   + "0, "	// commited=false
			   + "\"" + parser.callID() + "\", " // call_id
			   + "\"" + parser.timestamp() + "\", "
			   + Util::uitoa(line.first)
			   + ")" + ((line_buffer.size() > 1) ? ",\n\t" : "");

		last_offset = line.first;
		line_buffer.pop_front();
		counter++;
		count++;
	}

	getContext()->execute(sql);
	getContext()->commit();

	INFO("deposited with last_offset="
			<< last_offset << " and line_count="
			<< count);
	(*getStats())["lines"].update(count);
	RETURN(count);
	return count;
}

void LocalFileReader::commit()
{
	ENTER();
	DEBUG("commiting records");

	getContext()->begin();

	try
	{
		std::string sql = "INSERT INTO ReaderLog VALUES ("
			   + Util::uitoa(getID()) + ", "
			   + Util::uitoa(i_dir) + ", \""
			   + filename + "\", "
			   + Util::uitoa(last_offset) + ", 0)";

		getContext()->execute(sql);

		sql = "UPDATE CDR_Raw SET commited = 1 WHERE commited = 0 AND i_reader = ";
		sql += Util::uitoa(getID());

		getContext()->execute(sql);
	}
	catch (...)
	{
		getContext()->rollback();
	}

	getContext()->commit();

	LEAVE();
}

void LocalFileReader::refresh()
{
	ENTER();
	// look for new files..
	LEAVE();
}

void LocalFileReader::wait() const
{
	ENTER();
	usleep(10000);
	LEAVE();
}

void LocalFileReader::execute()
{
	ENTER();
	if ((!line_buffer.size()) &&
	    (idle_flag) &&
	    (!commit_pending))
	{
		if (!isIdle())
		{
			setIdle(true);
			INFO("Reader is idle.");
		}
	}
	else
	{
		setIdle(false);
	}
	if (stream_idle)
	{
		if (!idle_flag)
		{
			idle_flag = true;
			INFO("stream idle");
			poll_threshold->reset();
		}
	}
	else
	{
		if (idle_flag)
		{
			idle_flag = false;
		}
	}
	if (rawCount() <= 3000)
	{
		if (waiting)
			waiting = false;

		if (idle_flag && poll_threshold->reached())
		{
			poll();
			poll_threshold->reset();
		}
		else
		{
			poll();
		}
	}
	else
	{
		if (!waiting)
		{
			INFO("reader is waiting for the mediator to catch up");
			waiting = true;
		}
	}

	if (line_buffer.size() &&
	    deposit_threshold->reached())
	{
		deposit();
		commit_pending = true;
	}

	if (refresh_threshold->reached()) refresh();

	if (commit_pending && commit_threshold->reached())
	{
		commit();
		commit_pending = false;
	}

	LEAVE();
}

XCDR_NAMESPACE_END

int main(int argc, char** argv)
{
//	Log::filter |= Log::Debug;
//	Log::filter |= Log::Trace;

	int retval = 0;
	try
	{
		TODO("get this id from the command line");
		XCDR::LocalFileReader reader(0);
		reader.run();
	}
	catch (mysqlcppapi::ex_base &exc)
	{
		std::cerr << "TOPLEVEL MYSQL: " << exc.what() << std::endl << std::flush;
		retval = 1;
	}
	catch (XCDR::Exception &exc)
	{
		std::cerr << "TOPLEVEL XCDR: " << exc << std::endl << std::flush;
		retval = 1;
	}
	catch (XCDR::Exception *exc)
	{
		std::cerr << "TOPLEVEL XCDR: " << (*exc) << std::endl << std::flush;
		delete exc;
		retval = 1;
	}
	catch (std::ofstream::failure &e)
	{
		std::cerr << "fstream exception: " << e.what() << std::endl << std::flush;
		retval = 1;
	}
	catch (...)
	{
		std::cerr << "other exception" << std::endl << std::flush;
		retval = 1;
	}
	exit(retval);
}
