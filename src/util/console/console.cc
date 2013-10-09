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
// $Source: /usr/local/cvs/xcdr/src/util/console/console.cc,v $
// $Revision: 1.6 $
// $Date: 2003/04/04 16:06:36 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: console.cc,v $
// Revision 1.6  2003/04/04 16:06:36  tmarsh
// namespace changes
//
// Revision 1.5  2003/03/27 19:19:14  tmarsh
// rewrite of rater support, and clean up some memory leaks
//
// Revision 1.4  2003/03/21 11:16:29  tmarsh
// added sql
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
#include "command.h"

#include "load.h"
#include "list.h"
#include "rate.h"
#include "help.h"
#include "exec.h"
#include "size.h"
#include "lookup.h"
#include "sql.h"
#include "context.h"
#include "usage.h"
#include "populate.h"

#include "readline.h"

#include "common/logging.h"

#include <iostream>
#include <cstdlib>

XCDR_NAMESPACE_BEGIN


SyntaxError::SyntaxError(const string &msg) : Exception(msg)
{
}

RaterConsole::RaterConsole()
{
	context.rater = &rater;
	context.initMap();
	context.console = this;

	context.commands.push_back(new LoadCommand(&context));
	context.commands.push_back(new ListCommand(&context));
	context.commands.push_back(new RateCommand(&context));
	context.commands.push_back(new HelpCommand(&context));
	context.commands.push_back(new ExecCommand(&context));
	context.commands.push_back(new SizeCommand(&context));
	context.commands.push_back(new SQLCommand(&context));
	context.commands.push_back(new LookupCommand(&context));
	context.commands.push_back(new SetContextCommand(&context));
	context.commands.push_back(new UsageCommand(&context));
	context.commands.push_back(new PopulateCommand(&context));
}

RaterConsole::~RaterConsole()
{
	list<Command*>::iterator i = context.commands.begin();
	while (i != context.commands.end())
	{
		delete *i;
		i++;
	}
}

void ConsoleContext::initMap()
{
	dnode_map.insert(dnode_map.end(),
		make_pair<string, DNodePair>(
			string("timezone"),
			DNodePair(ConsoleContext::TIMEZONE, rater->timezone_map)));
	dnode_map.insert(dnode_map.end(),
		make_pair<string, DNodePair>(
			string("country"),
			DNodePair(ConsoleContext::COUNTRY, rater->country_map)));
	dnode_map.insert(dnode_map.end(),
		make_pair<string, DNodePair>(
			string("province"),
			DNodePair(ConsoleContext::PROVINCE, rater->province_map)));
	dnode_map.insert(dnode_map.end(),
		make_pair<string, DNodePair>(
			string("city"),
			DNodePair(ConsoleContext::CITY, rater->city_map)));
	dnode_map.insert(dnode_map.end(),
		make_pair<string, DNodePair>(
			string("lata"),
			DNodePair(ConsoleContext::LATA, rater->lata_map)));
	dnode_map.insert(dnode_map.end(),
		make_pair<string, DNodePair>(
			string("ocn"),
			DNodePair(ConsoleContext::OCN, rater->ocn_map)));
}

void RaterConsole::motd()
{
	cout << "XCDR Rater Console v0.5" << endl
	     << "Copyright (c) 2003 Thomas D. Marsh, all rights reserved" << endl
	     << flush;
}

inline string stripComment(const string &in)
{
	string::size_type pos = in.find("#", 0);
	if (pos != string::npos)
	{
		return in.substr(0, pos);
	}
	return in;
}

void RaterConsole::dispatch(const string &line) //, ConsoleContext *ctx)
{
	string nline = stripComment(line);
	if (nline.length() == 0)
		return;
	try
	{
		Command *command = Command::parseCommand(&context, nline);
		if (command)
		{
			command->execute();
		}
	}
	catch (SyntaxError *err)
	{
		ERROR("syntax error: " << (*err));
		delete err;
	}
	catch (Exception *err)
	{
		ERROR("external error: " << *err);
		delete err;
	}
	catch (Exception &err)
	{
		ERROR("external error: " << err);
	}
	catch (std::exception *exc)
	{
		ERROR("external error: " << exc->what());
	}
	catch (std::exception &exc)
	{
		ERROR("external error: " << exc.what());
	}
	catch (...)
	{
		INFO("unknown error");
	}
}

void RaterConsole::mainLoop()
{
	string line;
	Readline in;
	while (!in.eof())
	{
		string prompt = "xcdr:" + context.getPath() + "> ";
		line = in.readline(prompt);
		if (line.length())
			dispatch(line);
	}
}

XCDR_NAMESPACE_END

int main()
{
	Log::suppress_timestamp = true;
//	Log::filter |= Log::Debug;
//	Log::filter |= Log::Trace;
	XCDR::RaterConsole console;
	console.mainLoop();
}
