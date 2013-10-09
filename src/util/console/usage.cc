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
// $Source: /usr/local/cvs/xcdr/src/util/console/usage.cc,v $
// $Revision: 1.3 $
// $Date: 2003/03/21 11:05:06 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: usage.cc,v $
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
#include "usage.h"
#include "tok.h"

#include "common/logging.h"
#include "common/util.h"

#include <iostream>
#include <cstdlib>

XCDR_NAMESPACE_BEGIN

// -------------------------------------------------------------------------
//
// USAGE COMMAND
//
// -------------------------------------------------------------------------

void UsageCommand::execute()
{
	ENTER();
	string path = "/proc/";
	path += Util::itoa(getpid());
	path += "/status";
	ifstream fp(path.c_str(), ifstream::in);
	char buf[256];
	while (!fp.eof())
	{
		fp.getline(buf, 256);
		INFO(buf);
	}
	fp.close();
	LEAVE();
}

string UsageCommand::usage()
{
	ENTER();
	string msg = "usage";
	return msg;
}

void UsageCommand::setParams(const list<string> &params)
{
	ENTER();
	LEAVE();
}

bool UsageCommand::matches(const string &name)
{
	if ((name == "usage") ||
	    (name == "u"))
		return true;
	return false;
}

XCDR_NAMESPACE_END
