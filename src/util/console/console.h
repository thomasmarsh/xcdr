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
// $Source: /usr/local/cvs/xcdr/src/util/console/console.h,v $
// $Revision: 1.5 $
// $Date: 2003/05/14 11:37:18 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: console.h,v $
// Revision 1.5  2003/05/14 11:37:18  tmarsh
// replacing DigitNode with dtrie
//
// Revision 1.4  2003/04/04 16:06:36  tmarsh
// namespace changes
//
// Revision 1.3  2003/03/27 19:19:14  tmarsh
// rewrite of rater support, and clean up some memory leaks
//
// Revision 1.2  2003/03/18 17:53:48  tmarsh
// added contexting; major updates
//
// Revision 1.1  2003/03/15 21:11:04  tmarsh
// added
//
//
// =========================================================================

#ifndef XCDR_UTIL_CONSOLE_H
#define XCDR_UTIL_CONSOLE_H

#define CONSOLE_USAGE
#include "rater/rater.h"
#include "common/except.h"

#include <string>
#include <list>
#include <map>

using namespace std;

XCDR_NAMESPACE_BEGIN

class SyntaxError : public Exception
{
public:
	SyntaxError(const string &msg);
};

class Command;
class RaterConsole;
struct ConsoleContext
{
	enum State
	{
		ROOT = 0,
		TIMEZONE,
		COUNTRY,
		PROVINCE,
		CITY,
		LATA,
		OCN,
		PARTY,
		TARIFF,
		ACTIVITY,
		PERIOD,
		BRACKET,
		INTERVAL
	};
	// Current context state
	State state;

	// Rater and console pointers
	Rater *rater;
	RaterConsole *console;

	typedef dtrie::map<char*, char*, 10, dtrie::decimal_functor> dmap;

	struct DNode
	{
		char digit;
		dmap *node;

		DNode(const DNode &d) { node = d.node; digit = d.digit; }
		DNode() { node = 0; digit = '!'; }
		DNode(char c, dmap *n)
		{
			digit = c; node = n;
		}
		~DNode() { node = 0; }
	};
	struct DNodePair
	{
		State state;
		DigitNode<char*> *node;

		DNodePair(ConsoleContext::State s, DigitNode<char *> *n)
		{
			state = s;
			node = n;
		}

		~DNodePair() { node = 0; }
	};

	// Pointers into the rater
	map<string, DNodePair> dnode_map;
	list<DNode> dnode_stack;
	BillingParty *party;
	Tariff *tariff;
	Activity *activity;
	Period *period;
	Bracket *bracket;
	list<Bracket *> bracket_stack;
	Interval *interval;
	list<Interval *> interval_stack;

	// path and related methods
	list<string> path_stack;

	string getPath();
	ConsoleContext()
	{
		state = ROOT;
		rater = 0;
		console = 0;
		party = 0;
		tariff = 0;
		activity = 0;
		period = 0;
	}

	void initMap();

	list<Command*> commands;
};

class RaterConsole
{
public:
	RaterConsole();
	~RaterConsole();
	void mainLoop();
	void motd();
	void dispatch(const string &line);
private:
	Rater rater;
	ConsoleContext context;

};

XCDR_NAMESPACE_END

#endif // XCDR_UTIL_CONSOLE_H
