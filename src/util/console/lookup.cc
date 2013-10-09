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
// $Source: /usr/local/cvs/xcdr/src/util/console/lookup.cc,v $
// $Revision: 1.6 $
// $Date: 2003/05/14 11:37:18 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: lookup.cc,v $
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
#include "lookup.h"
#include "tok.h"

#include "common/logging.h"

#include <iostream>
#include <cstdlib>

XCDR_NAMESPACE_BEGIN

// -------------------------------------------------------------------------
//
// RATE COMMAND
//
// -------------------------------------------------------------------------

string nstr(char *s)
{
	return string(s ? s : "");
}

#define LOOKUP(map, dest) nstr((*map)[const_cast<char*>(dest)])

void LookupCommand::execute()
{
	INFO("query results:"
		<< "\nTime Zone   = \""
		<< LOOKUP(context->rater->timezone_map, destination.c_str())
		<< "\"\nCountry     = \""
		<< LOOKUP(context->rater->country_map, destination.c_str())
		<< "\"\nProvince    = \""
		<< LOOKUP(context->rater->province_map, destination.c_str())
		<< "\"\nCity        = \""
		<< LOOKUP(context->rater->city_map, destination.c_str())
		<< "\"\nLATA        = \""
		<< LOOKUP(context->rater->lata_map, destination.c_str())
		<< "\"\nOCN         = \""
		<< LOOKUP(context->rater->ocn_map, destination.c_str())
		<< "\"");
}

string LookupCommand::usage()
{
	string msg = "lookup <dest>";
	return msg;
}

void LookupCommand::setParams(const list<string> &params)
{
	ENTER();
	if (params.size() != 1)
	{
		string msg = "wrong number of parameters: " + usage();
		throw new SyntaxError(msg);
	}

	destination = params.front();
	LEAVE();
}

bool LookupCommand::matches(const string &name)
{
	if ((name == "lookup") ||
	    (name == "lu"))
		return true;
	return false;
}

XCDR_NAMESPACE_END
