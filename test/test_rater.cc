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
// $Source: /usr/local/cvs/xcdr/test/test_rater.cc,v $
// $Revision: 1.1 $
// $Date: 2003/05/05 17:20:52 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: test_rater.cc,v $
// Revision 1.1  2003/05/05 17:20:52  tmarsh
// added test_rater
//
//
// =========================================================================

#include "rater/rater.h"
#include "rater/except.h"
#include "common/logging.h"
#include "common/util.h"

#include <iostream>

using namespace XCDR;

std::string gen_num()
{
	std::string cli("1");
	int i = 0;
	while (i < 10)
	{
		cli += Util::itoa(random() % 10);
		i++;
	}
	return cli;
}

Query *testQuery()
{
	Query *query = new Query;

	query->cli = strdup(gen_num().c_str());
	query->cld = strdup(gen_num().c_str());
	query->payphone = false;
	query->duration = random() % 600;
	Identifier *ident = new Identifier;
	ident->i_type = Identifier::REPORTING_GW;
	ident->value = strdup("RAMBLER");
	query->identifiers.push_back(ident);
	return query;
}

Response *testResponse()
{
	return new Response;
}

double success;
double total;
std::string error;

inline void doRating(Rater *rater, Query *query, Response *response)
{
	total += 1;
	try
	{
		rater->rate(query, response);
		error = "success";
		success += 1;
	}
	catch (RatingFailure *exc)
	{
//		FATAL(*exc);
//		INFO("Exception dump:\n" << exc->dump());
		error = exc->what();
		delete exc;
	}
	catch (...)
	{
		FATAL("unhandled exception");
		error = "unhandled";
	}
	std::cout << response->i_customer << ":"
		  << response->i_tariff << ":"
		  << response->i_activity << ":"
		  << response->i_rate << ":"
		  << response->i_bracket << ":"
		  << query->cli << ":"
		  << query->cld << ":"
		  << query->duration << ":"
		  << response->charged_time << ":"
		  << response->charged_amount << ":"
		  << error
		  << std::endl << std::flush;
}


int main()
{
//	Log::filter |= Log::Debug | Log::Trace;
	success = 0;
	total = 0;
	srand(time(NULL));
	Rater r;
	INFO("Rater size: " << r.size());
	INFO("Populating...");
	r.populate();
	INFO("Rater size: " << r.size());
	INFO("Begin rating");
	for (int i = 0; i < 100000; i++)
	{
		Query *query = testQuery();
		Response *response = testResponse();
		doRating(&r, query, response);
		delete query;
		delete response;
	}
	INFO("End rating");
	return 0;
}
