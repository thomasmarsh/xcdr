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
// $Source: /usr/local/cvs/xcdr/src/util/console/rate.cc,v $
// $Revision: 1.4 $
// $Date: 2003/04/04 16:06:36 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: rate.cc,v $
// Revision 1.4  2003/04/04 16:06:36  tmarsh
// namespace changes
//
// Revision 1.3  2003/03/21 11:05:06  tmarsh
// broke out command headers
//
// Revision 1.2  2003/03/18 17:53:48  tmarsh
// added contexting; major updates
//
// Revision 1.1  2003/03/15 21:11:26  tmarsh
// added
//
//
// =========================================================================

#include "console.h"
#include "rate.h"
#include "tok.h"

#include "rater/except.h"
#include "common/logging.h"
#include "common/util.h"

#include <iostream>
#include <cstdlib>

XCDR_NAMESPACE_BEGIN

#define NSTR(x) ((x) ? (x) : string("<null>"))

#if 0
inline std::string icu2str(UnicodeString &in)
{
	static char out[200];
	out[in.extract(0, 99, out)] = 0;
	return string(out);
}
#endif

#if 0
inline void dump_ident_list(std::string &msg, IdentifierList &l)
{
	if (!l.count()) return;

	IdentifierList::iterator i = l.begin();
	while (i != l.end())
	{
		Identifier *ident = *i;
		msg += "\t\t(" + Util::itoa(ident->i_type) + ", \"";
		msg += NSTR(ident->value) + "\")\n";
		i++;
	}
}
#endif

inline void dump_dest_info(std::string &msg, DestinationInfo &d)
{
	msg += "\t\tis_nanp    = " + Util::itoa(d.is_nanp) + "\n";
	msg += "\t\tvalid      = " + Util::itoa(d.valid) + "\n";
	msg += "\t\tcountry    = " + NSTR(d.country) + "\n";
	msg += "\t\tprovince   = " + NSTR(d.province) + "\n";
	msg += "\t\tcity       = " + NSTR(d.city) + "\n";
	msg += "\t\ttimezone   = " + NSTR(d.timezone) + "\n";
	msg += "\t\tlata       = " + NSTR(d.lata) + "\n";
	msg += "\t\tocn        = " + NSTR(d.ocn) + "\n";
}

std::string dumpResponse(Response *response)
{
	ENTER();
	std::string msg;
	msg += "\nResponse:\n";
	msg += "\tprice      = " + Util::dtoa(response->charged_amount) + "\n";
	msg += "\ttime       = " + Util::itoa(response->charged_time) + "\n";
	msg += "\ttime_del   = 0\n";
	msg += "\tcli_info   = {\n";
	dump_dest_info(msg, response->cli_info);
	msg += "\t}\n\tcld_info = {\n";
	dump_dest_info(msg, response->cld_info);
	msg += "\t}\n";
	msg += "\ti_customer = " + Util::itoa(response->i_customer) + "\n";
	msg += "\ti_vendor   = " + Util::itoa(response->i_vendor) + "\n";
	msg += "\ti_tariff   = " + Util::itoa(response->i_tariff) + "\n";
	msg += "\ti_activity = " + Util::itoa(response->i_activity) + "\n";
	msg += "\ti_period   = " + Util::itoa(response->i_period) + "\n";
	msg += "\ti_rate     = " + Util::itoa(response->i_rate) + "\n";
	msg += "\ti_bracket  = " + Util::itoa(response->i_bracket) + "\n";
	msg += "\ti_bracketx = " + Util::itoa(response->i_bracket_index) + "\n";
	msg += "\ti_interval = " + Util::itoa(response->i_interval) + "\n";
	LEAVE();
	return msg;
}


// -------------------------------------------------------------------------
//
// RATE COMMAND
//
// -------------------------------------------------------------------------

char *getDigits(const string &label)
{
	char buf[256];
	cout << label << ": " << flush;
	cin.getline(buf, 256);
	return strdup(buf);
}

bool getBool(const string &label)
{
	char buf[256];
	cout << label << ": " << flush;
	cin.getline(buf, 256);
	if ((buf[0] == 'Y') || (buf[0] == 'y'))
		return true;
	return false;
}

void buildIdentList(Query *query)
{
	bool done = false;
	char buf[256];
	while (!done)
	{
		cout << "adding idents ('.' to stop)" << endl;
		cout << "i_type: ";
		cin.getline(buf, 256);
		if (buf[0] == '.')
			break;
		Identifier *ident = new Identifier;
		ident->i_type = (Identifier::Type) Util::atoi(buf);
		ident->value = getDigits("value");
		query->identifiers.push_back(ident);
	}
}

void interactiveQueryBuild(Query *query)
{
	query->cli = getDigits("CLI");
	query->cld = getDigits("CLD");
	query->payphone = getBool("Payphone");
	query->prison = getBool("Prison");
	query->duration = Util::atoi(getDigits("Duration"));
	buildIdentList(query);
}

void printResponse(Response *response)
{
	cout << dumpResponse(response);
}

void RateCommand::execute()
{
	ENTER();
	Query *query = new Query;
	Response *response = new Response;

	interactiveQueryBuild(query);

	try
	{
		context->rater->rate(query, response);
	}
	catch (RatingFailure *exc)
	{
		FATAL("RatingFailure: " << exc->what());
		delete exc;
		delete query;
		delete response;
		return;
	}

	printResponse(response);

	delete query;
	delete response;
	LEAVE();
}

string RateCommand::usage()
{
	ENTER();
	string msg = "rate ? ? ?";
	return msg;
}

void RateCommand::setParams(const list<string> &params)
{
	ENTER();
	TODO("implement");
	LEAVE();
}

bool RateCommand::matches(const string &name)
{
	if ((name == "rate") ||
	    (name == "r"))
		return true;
	return false;
}

XCDR_NAMESPACE_END
