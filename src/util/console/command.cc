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
// $Source: /usr/local/cvs/xcdr/src/util/console/command.cc,v $
// $Revision: 1.4 $
// $Date: 2003/04/04 16:06:36 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: command.cc,v $
// Revision 1.4  2003/04/04 16:06:36  tmarsh
// namespace changes
//
// Revision 1.3  2003/03/27 19:19:14  tmarsh
// rewrite of rater support, and clean up some memory leaks
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
#include "tok.h"

#include "common/logging.h"

#include <iostream>
#include <cstdlib>

XCDR_NAMESPACE_BEGIN

// -------------------------------------------------------------------------
//
// COMMAND PARSER
//
// -------------------------------------------------------------------------

Command *Command::parseCommand(ConsoleContext *ctx, const string &line)
{
	enum State
	{
		COMMAND = 0,
		PARAMETER
	};

	State state = COMMAND;

	Tokenizer t(line, " \t");
	string name;
	list<string> params;
	Tokenizer::iterator i = t.begin();
	while (i != t.end())
	{
		TRACE("i=" << *i);
		if (i->length())
		{
			if ((*i) == "#")
				break;

			switch (state)
			{
			case COMMAND:
				name = *i;
				state = PARAMETER;
				break;
			case PARAMETER:
				params.push_back(*i);
				break;
			default:
				FATAL("unknown state: " << state);
				break;
			}
		}
		i++;
	}

	if (name == "")
		return 0;

	TRACE(name);
	TRACE(ctx);
//	TRACE(ctx->commands.size());
	ConsoleContext *cc = ctx->commands.front()->context;
	list<Command*>::const_iterator j = cc->commands.begin();
	TRACE(3);
	while (j != cc->commands.end())
	{
		if ((*j)->matches(name))
		{
			TRACE(5);
			(*j)->setParams(params);
			return (*j);
		}
		TRACE(6);
		j++;
	}
	TRACE(7);

	throw new SyntaxError("unknown command: try \"help\"");
	return 0;
}


// -------------------------------------------------------------------------
//
// ABSTRACT COMMAND
//
// -------------------------------------------------------------------------

Command::Command(ConsoleContext *_context)
{
	context = _context;
}


XCDR_NAMESPACE_END
