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
// $Source: /usr/local/cvs/xcdr/src/util/console/exec.cc,v $
// $Revision: 1.5 $
// $Date: 2003/04/04 16:06:36 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: exec.cc,v $
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
#include "exec.h"
#include "tok.h"

#include "common/logging.h"

#include "boost/filesystem/operations.hpp"

#include <iostream>
#include <cstdlib>

XCDR_NAMESPACE_BEGIN

// -------------------------------------------------------------------------
//
// RATE COMMAND
//
// -------------------------------------------------------------------------

void ExecCommand::execute()
{
	ifstream fp(path.c_str(), ifstream::in);
	char buf[256];
	while (!fp.eof())
	{
		fp.getline(buf, 256);
		string line(buf);
		context->console->dispatch(line);
	}
	fp.close();
}

string ExecCommand::usage()
{
	string msg = "exec <file>";
	return msg;
}

void ExecCommand::setParams(const list<string> &params)
{
	ENTER();
	if (params.size() != 1)
	{
		string msg = "wrong number of parameters: " + usage();
		throw new SyntaxError(msg);
	}

	path = params.front();

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

bool ExecCommand::matches(const string &name)
{
	if ((name == "exec") ||
	    (name == "run") ||
	    (name == "e"))
		return true;
	return false;
}

XCDR_NAMESPACE_END
