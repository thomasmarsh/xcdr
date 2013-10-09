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
// $Source: /usr/local/cvs/xcdr/src/rdbms/query.cc,v $
// $Revision: 1.7 $
// $Date: 2003/04/04 16:06:36 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: query.cc,v $
// Revision 1.7  2003/04/04 16:06:36  tmarsh
// namespace changes
//
// Revision 1.6  2003/03/09 08:28:22  tmarsh
// include file reorganization
//
// Revision 1.5  2003/03/09 08:27:17  tmarsh
// added paramsNull symbol
//
// Revision 1.4  2003/03/07 22:28:39  tmarsh
// more debugging
//
// Revision 1.3  2003/03/07 18:09:01  tmarsh
// updates for new rdbms and repository
//
// Revision 1.2  2003/03/07 13:27:07  tmarsh
// fixes to compile
//
// Revision 1.1  2003/03/07 11:26:44  tmarsh
// added
//
//
// =========================================================================

#include "rdbms/query.h"
#include <iostream>

XCDR_NAMESPACE_BEGIN

using namespace RDBMS;

// -------------------------------------------------------------------------
//
// QUERY GROUP
// 
// -------------------------------------------------------------------------


Parameters QueryGroup::paramsNull;

Repository::Repository(std::string &n)
{
	DEBUG("INIT REPOSITORY");
	name = n;
	initialize();
}

void QueryGroup::build(InputPair &input)
{
	DEBUG("building query: " << input.first);
	std::string statement;

	if (group.queries.count(input.first) == 0)
	{
		FATAL("No such query!");
	}
	XML::Query query = group.queries[input.first];

	std::list<unsigned int>::iterator
		i = query.build_order.begin(),
		t = query.build_table.begin(),
		p = query.build_param.begin();
	
	std::list<std::string>::iterator
		s = query.sql.begin();

	DEBUG("Beginning build:");
	while (i != query.build_order.end())
	{
		switch (*i)
		{
		case XML::BUILD_SQL:
			DEBUG(" - BUILD_SQL");
			statement += (*s);
			s++;
			break;
		case XML::BUILD_TABLE:
			DEBUG(" - BUILD_TABLE");
			statement += query.tables[*t];
			t++;
			break;
		case XML::BUILD_PARAM:
			DEBUG(" - BUILD_PARAM");
			statement += input.second[query.inputs[*p]];
			p++;
			break;
		}
		i++;

	}
	DEBUG("End Build.");

	query_cache.insert(query_cache.end(),
			std::make_pair<InputPair,
				     std::string>(input, statement));
}

XCDR_NAMESPACE_END
