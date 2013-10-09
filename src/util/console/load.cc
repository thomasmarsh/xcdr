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
// $Source: /usr/local/cvs/xcdr/src/util/console/load.cc,v $
// $Revision: 1.6 $
// $Date: 2003/05/14 11:37:18 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: load.cc,v $
// Revision 1.6  2003/05/14 11:37:18  tmarsh
// replacing DigitNode with dtrie
//
// Revision 1.5  2003/04/04 16:06:36  tmarsh
// namespace changes
//
// Revision 1.4  2003/03/27 19:19:14  tmarsh
// rewrite of rater support, and clean up some memory leaks
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
#include "load.h"
#include "tok.h"

#include "common/logging.h"

#include "boost/filesystem/operations.hpp"

XCDR_NAMESPACE_BEGIN


#define PUSH(map, dest, value) map->insert(dest, const_cast<char*>(value))

void LoadCommand::execute()
{
	ENTER();
	ifstream fp(path.c_str(), ifstream::in);

	char buf[256];
	unsigned int count = 0;
	while (!fp.eof())
	{
		fp.getline(buf, 256);
		Tokenizer t(buf, "\t");
		switch (type)
		{
		case LOAD_TIMEZONE:
			if (t.size() == 2)
			{
				PUSH(context->rater->timezone_map,
				     strdup(t[1].c_str()),
				     strdup(t[0].c_str()));
				count++;
			}
			break;
		case LOAD_COUNTRY:
			if (t.size() == 2)
			{
				PUSH(context->rater->country_map,
				     strdup(t[1].c_str()),
				     strdup(t[0].c_str()));
				count++;
			}
			break;
		case LOAD_PROVINCE:
			if (t.size() == 2)
			{
				PUSH(context->rater->province_map,
				     strdup(t[1].c_str()),
				     strdup(t[0].c_str()));
				count++;
			}
			break;
		case LOAD_CITY:
			if (t.size() == 2)
			{
				PUSH(context->rater->city_map,
				     strdup(t[1].c_str()),
				     strdup(t[0].c_str()));
				count++;
			}
			break;
		case LOAD_LATA:
			if (t.size() == 2)
			{
				PUSH(context->rater->lata_map,
				     strdup(t[1].c_str()),
				     strdup(t[0].c_str()));
				count++;
			}
			break;
		case LOAD_OCN:
			if (t.size() == 2)
			{
				PUSH(context->rater->ocn_map,
				     strdup(t[1].c_str()),
				     strdup(t[0].c_str()));
				count++;
			}
			break;
		default:
			ERROR("unknown load type: " << type);
			break;
		}
	}
	INFO("loaded '" << path << "' (" << count << " records)");
	LEAVE();
}

string LoadCommand::usage()
{
	ENTER();
	string msg = "load <type> <file>";
	LEAVE();
	return msg;
}

void LoadCommand::setParams(const list<string> &params)
{
	ENTER();
	if (params.size() != 2)
	{
		string msg = "wrong number of parameters: " + usage();
		throw new SyntaxError(msg);
	}

	string stype = params.front();
	string file = params.back();
	LoadCommand::LoadType load_type;

	if (stype == "timezone")	load_type = LoadCommand::LOAD_TIMEZONE;
	else if (stype == "country")	load_type = LoadCommand::LOAD_COUNTRY;
	else if (stype == "province")	load_type = LoadCommand::LOAD_PROVINCE;
	else if (stype == "city")	load_type = LoadCommand::LOAD_CITY;
	else if (stype == "lata")	load_type = LoadCommand::LOAD_LATA;
	else if (stype == "ocn")	load_type = LoadCommand::LOAD_OCN;
	else if (stype == "party")	load_type = LoadCommand::LOAD_PARTY;
	else if (stype == "tariff")	load_type = LoadCommand::LOAD_TARIFF;
	else if (stype == "activity")	load_type = LoadCommand::LOAD_ACTIVITY;
	else if (stype == "period")	load_type = LoadCommand::LOAD_PERIOD;
	else if (stype == "bracket")	load_type = LoadCommand::LOAD_BRACKET;
	else if (stype == "interval")	load_type = LoadCommand::LOAD_INTERVAL;
	else
	{
		string msg = usage() + "; valid types are: \n";
		msg += "\ttimezone\n";
		msg += "\tcountry\n";
		msg += "\tprovince\n";
		msg += "\tcity\n";
		msg += "\tlata\n";
		msg += "\tocn\n";
		msg += "\tparty\n";
		msg += "\ttariff\n";
		msg += "\tactivity\n";
		msg += "\tperiod\n";
		msg += "\tbracket\n";
		msg += "\tinterval";
		throw new SyntaxError(msg);
	}

	type = load_type;
	path = params.back();

	namespace fs = boost::filesystem;

	fs::path fpath = path.c_str();
	if ((!fs::exists(fpath)) ||
	    (fs::is_directory(fpath)) ||
	    (fs::is_empty(fpath)))
	{
		string msg = "incorrect file type, or non-existent: " + path;
		throw new SyntaxError(msg);
	}

	LEAVE();
}

bool LoadCommand::matches(const string &name)
{
	if ((name == "load") ||
	    (name == "ld"))
		return true;
	return false;
}

XCDR_NAMESPACE_END
