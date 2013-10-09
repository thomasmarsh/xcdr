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
// $Source: /usr/local/cvs/xcdr/src/util/console/context.cc,v $
// $Revision: 1.4 $
// $Date: 2003/05/14 11:37:18 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: context.cc,v $
// Revision 1.4  2003/05/14 11:37:18  tmarsh
// replacing DigitNode with dtrie
//
// Revision 1.3  2003/04/04 16:06:36  tmarsh
// namespace changes
//
// Revision 1.2  2003/03/21 11:05:06  tmarsh
// broke out command headers
//
// Revision 1.1  2003/03/18 17:54:12  tmarsh
// added
//
//
// =========================================================================

#include "console.h"
#include "context.h"
#include "tok.h"

#include "common/logging.h"
#include "common/util.h"

#include <iostream>
#include <cstdlib>

XCDR_NAMESPACE_BEGIN

inline string getDigitPath(const list<ConsoleContext::DNode> &stack)
{
	ENTER();
	string path("");
	list<ConsoleContext::DNode>::const_iterator i = stack.begin();
	i++;
	while (i != stack.end())
	{
		path += i->digit;
		i++;
	}
	if (path != "")
		path += "/";
	LEAVE();
	return path;
}

string ConsoleContext::getPath()
{
	ENTER();
	string path("/");
	switch (state)
	{
	// root; nothing to do;
	case ROOT:
		return path;

	// handle digit paths
	case TIMEZONE:
		path += "timezone/";
		path += getDigitPath(dnode_stack);
		break;
	case COUNTRY:
		path += "country/";
		path += getDigitPath(dnode_stack);
		break;
	case PROVINCE:
		path += "province/";
		path += getDigitPath(dnode_stack);
		break;
	case CITY:
		path += "city/";
		path += getDigitPath(dnode_stack);
		break;
	case LATA:
		path += "lata/";
		path += getDigitPath(dnode_stack);
		break;
	case OCN:
		path += "ocn/";
		path += getDigitPath(dnode_stack);
		break;
	
	case INTERVAL:
//		path += Util::itoa(interval->i_interval) + "/";
		path += "!!/";
		break;
	case BRACKET:
		path += Util::itoa(bracket->i_bracket) + "/";
		break;
	case PERIOD:
		path += Util::itoa(bracket->i_bracket_index) + "/";
		path += Util::itoa(period->i_period) + "/";
		break;
	case ACTIVITY:
		path += Util::itoa(activity->i_activity) + "/";
		break;
	case TARIFF:
		path += Util::itoa(tariff->i_tariff) + "/";
		break;
	case PARTY:
		path += Util::itoa(party->i_party) + "/";
		break;
	default:
		FATAL("unknown state: " << state);
		break;
	}
	LEAVE();
	return path;
}

// -------------------------------------------------------------------------
//
// SETCONTEXT COMMAND
//
// -------------------------------------------------------------------------

void SetContextCommand::gotoRoot()
{
	ENTER();
	while (context->state != ConsoleContext::ROOT)
		gotoNode("..");
	LEAVE();
}
void SetContextCommand::gotoNode(const string &relpath)
{
	ENTER();
	TRACE("relpath = " << relpath);
	if (relpath == ".")
	{
		// nothing to do
		LEAVE();
		return;
	}

	if (relpath == "<root>")
	{
		gotoRoot();
		LEAVE();
		return;
	}

	map<string, ConsoleContext::DNodePair>::iterator i;
	bool found = false;
	char c = '\0';
	switch (context->state)
	{
	case (ConsoleContext::ROOT):
		TRACE("ConsoleContext::ROOT");
		i = context->dnode_map.begin();
		while (i != context->dnode_map.end())
		{
			if (i->first == relpath)
			{
				context->dnode_stack.push_back(
						DNode('P', i->second.node));
				context->state = i->second.state;
				if ((i->second.node->children) == 0)
					WARN("node has not been initialized yet");
				found = true;
				TRACE("break");
				break;
			}
			i++;
		}
		if (!found)
		{
			if (relpath != "..")
			{
				ERROR("invalid argument; try 'ls'")
			}
		}
		TRACE("break");
		break;

	case (ConsoleContext::TIMEZONE):
	case (ConsoleContext::COUNTRY):
	case (ConsoleContext::PROVINCE):
	case (ConsoleContext::CITY):
	case (ConsoleContext::LATA):
	case (ConsoleContext::OCN):
		TRACE("ConsoleContext::(TIMEZONE|COUNTRY|...|OCN)");
		c = relpath[0];
		if ((c >= '0') && (c <= '9'))
		{
			if (relpath.length() > 1)
			{
				string newp;
				for (unsigned int j = 0; j < relpath.length(); j++)
				{
					newp = relpath[j];
					gotoNode(newp);
				}
				TRACE("break");
				break;
			}
			ConsoleContext::dmap *current =
				context->dnode_stack.back().node;

			char idx = c-'0';

			if ((current->children) == 0)
			{
				ERROR("no children");
			}
			else
			{
				context->dnode_stack.push_back(
						DNode(c, current->children[idx]));
			}
		}
		else if (relpath == "..")
		{
			context->dnode_stack.pop_back();
			if (context->dnode_stack.size() == 0)
				context->state = ConsoleContext::ROOT;
		}
		else
			ERROR("argument must be one of '..', '.', or [0-9]");

		TRACE("break");
		break;
	default:
		TRACE("default");
		ERROR("unhandled state" << context->state);
		TRACE("break");
		break;
	}
	LEAVE();
}

void SetContextCommand::execute()
{
	ENTER();

	if (!path.length())
	{
		ERROR("no path");
		return;
	}
	if (path[0] == '/')
		path = "<root>" + path;

	if (path[path.length()-1] == '/')
		path += ".";

	Tokenizer t(path, "/");

	if (!t.size())
	{
		context->state = ConsoleContext::ROOT;
	}
	if (t.size() > 0)
	{
		Tokenizer::iterator i = t.begin();
		while (i != t.end())
		{
			gotoNode(*i);
			i++;
		}
	}
	LEAVE();
}

string SetContextCommand::usage()
{
	ENTER();
	string msg = "size";
	return msg;
}

void SetContextCommand::setParams(const list<string> &params)
{
	ENTER();
	if (params.size() == 1)
		path = params.front();
	else if (params.size() > 1)
	{
		path = params.front();
		WARN("ignoring extra arguments");
	}
	else
	{
		path = "/";
	}
	LEAVE();
}

bool SetContextCommand::matches(const string &name)
{
	if ((name == "setcontext") ||
	    (name == "sc") ||
	    (name == "cd"))
		return true;
	return false;
}

XCDR_NAMESPACE_END
