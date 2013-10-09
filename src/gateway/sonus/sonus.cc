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
// $Source: /usr/local/cvs/xcdr/src/gateway/sonus/sonus.cc,v $
// $Revision: 1.6 $
// $Date: 2003/04/04 16:01:53 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: sonus.cc,v $
// Revision 1.6  2003/04/04 16:01:53  tmarsh
// namespace changes
//
// Revision 1.5  2003/03/09 08:50:05  tmarsh
// include file reorganization
//
// Revision 1.4  2003/03/06 11:20:03  tmarsh
// updated headers
//
//
// =========================================================================

#include "gateway/sonus/sonus.h"

XCDR_NAMESPACE_BEGIN

Sonus::Parser::Parser(const std::string &line)
	: Parser::CSVParser(line)
{
	if (cache[0] == "STOP")
	{
		cdr_type = CDR::TYPE_STOP;
	}
	else if (cache[0] == "START")
	{
		cdr_type = CDR::TYPE_START;
	}
	else if (cache[0] == "INTERMEDIATE")
	{
		cdr_type = CDR::TYPE_INTERMEDIATE;
	}
	else if (cache[0] == "REBOOT")
	{
		cdr_type = CDR::TYPE_REBOOT;
	}
	else if (cache[0] == "ATTEMPT")
	{
		cdr_type = CDR::TYPE_ATTEMPT;
	}
	else
	{
		cdr_type = CDR::TYPE_UNKNOWN;
		TODO(cache[0]);
	}
}

std::string Sonus::Parser::callID()
{
	switch (cdr_type)
	{
	case CDR::TYPE_START:
		return cache[Start::call_id];
	case CDR::TYPE_ATTEMPT:
		return cache[Attempt::call_id];
	case CDR::TYPE_STOP:
		return cache[Stop::call_id];

	case CDR::TYPE_UNKNOWN:
	default:
		return "0";
	}
}

std::string Sonus::Parser::timestamp()
{
	try
	{
		if (cdr_type == CDR::TYPE_UNKNOWN)
		{
			return "01/01/2000 10:07:00";
		}

		if (cdr_type == CDR::TYPE_STOP)
		{
			return DateTime(cache[Stop::disconnect_date],
				        cache[Stop::disconnect_time]).toString();
		}
	}
	catch (...)
	{
		return "01/01/2000 10:07:00";
	}
	return "01/01/2000 10:07:00";
}



Sonus::Mediator::Mediator()
	: Gateway::Mediator()
{
}


DateTime Sonus::Mediator::getDateTime(Parser &p, unsigned int d, unsigned int t)
{
	try
	{
		return DateTime(p[d], p[t]);
	}
	catch(...)
	{
		DEBUG("---------- bad date -------------");
		DEBUG(" DATE: " << p[d]);
		DEBUG(" TIME: " << p[t]);
		DEBUG("---------------------------------");
	}
	return DateTime("01/01/1976", "00:00:00");
}

//
// Helper macros for datetime processing
//

#define stopStartDate(p)	getDateTime(p, Stop::start_date, \
					       Stop::start_time)
#define stopStopDate(p)		getDateTime(p, Stop::disconnect_date, \
					       Stop::disconnect_time)
#define attemptStartDate(p)	getDateTime(p, Attempt::start_date, \
					       Attempt::start_time)
#define attemptStopDate(p)	getDateTime(p, Attempt::start_date, \
					       Attempt::disconnect_time)

CDR::Mediated Sonus::Mediator::mediate(Gateway::Call &recs)
{
	ENTER();
	CDR::Mediated m;
	std::list<Parser> p;

	unsigned int attempt_count = 0;
	unsigned int stop_count = 0;
	unsigned int start_count = 0;

	Gateway::Call::iterator j = recs.begin();
	while (j != recs.end())
	{
		p.push_back(Parser((*j).second));
		j++;
	}

	std::list<Parser>::iterator i = p.begin();

	while (i != p.end())
	{
		if ((*i).cdrType() == CDR::TYPE_START)
		{
			start_count++;
		}

		if ((*i).cdrType() == CDR::TYPE_STOP)
		{
			stop_count++;

			m.call_id = (*i)[Stop::call_id];
			m.i_type_node = 1;
			m.i_ingress = 0;
			m.i_egress = 0;
			m.start_time = stopStartDate(*i);
			m.stop_time =  stopStopDate(*i);
			m.start_time_local = m.start_time;
			m.stop_time_local = m.stop_time;
			m.setup_seconds = atoi((*i)[Stop::elapsed_setup_response].c_str()) / 100;
			m.elapsed_seconds = atoi((*i)[Stop::service_duration].c_str()) / 100;
			m.bytes_sent = atoi((*i)[Stop::bytes_sent].c_str());
			m.bytes_rcvd = atoi((*i)[Stop::bytes_rcvd].c_str());
			m.cli = (*i)[Stop::calling_number];
			if (m.cli == "")
			{
				m.cli = (*i)[Stop::billing_number];
			}
			m.cld = (*i)[Stop::called_number];
			m.billable = true;
		}
		if ((*i).cdrType() == CDR::TYPE_ATTEMPT)
		{
			attempt_count++;

			m.call_id = (*i)[Attempt::call_id];
			m.i_type_node = 1;
			m.i_ingress = 0;
			m.i_egress = 0;
			m.start_time = attemptStartDate(*i);
			m.stop_time =  attemptStopDate(*i);
			m.start_time_local = m.start_time;
			m.stop_time_local = m.stop_time;
			m.setup_seconds = atoi((*i)[Attempt::elapsed_time_6].c_str()) / 100;
			m.elapsed_seconds = 0;
			m.bytes_sent = 0;
			m.bytes_rcvd = 0;
			m.cli = (*i)[Attempt::calling_number];
			if (m.cli == "")
			{
				m.cli = (*i)[Attempt::billing_number];
			}
			m.cld = (*i)[Attempt::called_number];
			m.billable = false;
		}
		i++;
	}
	LEAVE();
	return m;
}

XCDR_NAMESPACE_END
