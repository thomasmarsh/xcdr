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
// $Source: /usr/local/cvs/xcdr/src/xml/repository.cc,v $
// $Revision: 1.4 $
// $Date: 2003/04/04 16:06:36 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: repository.cc,v $
// Revision 1.4  2003/04/04 16:06:36  tmarsh
// namespace changes
//
// Revision 1.3  2003/03/09 08:32:58  tmarsh
// include file reorganization
//
// Revision 1.2  2003/03/07 22:28:15  tmarsh
// debugging added
//
// Revision 1.1  2003/03/07 18:07:50  tmarsh
// fixes for repository
//
//
// =========================================================================

#include "xml/repository.h"
#include "common/util.h"
#include "common/logging.h"

#include <xercesc/sax2/Attributes.hpp>

#include <map>
#include <iostream>

XCDR_NAMESPACE_BEGIN

using namespace XMLRepository;

Handler::Handler()
{
	in_repository = false;
	in_group = false;
	in_query = false;
	in_in = false;
	in_param = false;
	in_sql = false;
	in_table = false;
	in_input = false;
	repository = NULL;
}

Handler::~Handler()
{
	if (repository) delete repository;
}

std::string Handler::stateString()
{
	std::string state("");

	if (in_repository) state += "1"; else state += "0";
	if (in_group) state += "1"; else state += "0";
	if (in_query) state += "1"; else state += "0";
	if (in_in) state += "1"; else state += "0";
	if (in_param) state += "1"; else state += "0";
	if (in_sql) state += "1"; else state += "0";
	if (in_table) state += "1"; else state += "0";
	if (in_input) state += "1"; else state += "0";

	return state;
}

void Handler::startElement(const XMLCh *const uri,
			   const XMLCh *const localname,
			   const XMLCh *const qname,
			   const Attributes &attrs)
{
	char *msg = XMLString::transcode(qname);
	std::string name(msg);
	DEBUG("XML START: " << name);

	std::map<std::string, std::string> attribs;
	typedef std::pair<std::string, std::string> nameval;

	unsigned int len = attrs.getLength();
	for (unsigned int i = 0; i < len; i++)
	{
		char *str = XMLString::transcode(attrs.getLocalName(i));
		char *val = XMLString::transcode(attrs.getValue(i));

		std::string s(str), v(val);
		nameval nv;
		nv.first = s;
		nv.second = v;
		DEBUG("XML ATTRIB: " << nv.first << "=" << nv.second);
		attribs.insert(attribs.end(), nv);

		XMLString::release(&str);
		XMLString::release(&val);
	}


	TODO("move each case to separate method");
	if (name == "repository")
	{
		if (repository) delete repository;
		repository = new XML::Repository;

		repository->model = attribs["model"];
	
		in_repository = true;
	}
	else if (name == "group")
	{
		assert(in_repository);
		// push name onto stack

		group_stack.push_back(attribs["name"]);

		XML::Group g;

		g.name = group_stack.front();
		std::list<std::string>::iterator i;
		i = group_stack.begin();
		i++;
		while (i != group_stack.end())
		{
			g.name += "/";
			g.name += (*i);
			i++;
		}
		
		current_group = g.name;

		repository->groups.insert(repository->groups.end(), std::make_pair<std::string, XML::Group>(g.name, g));

		in_group = true;
	}
	else if (name == "query")
	{
		assert(in_group && in_repository);
		
		XML::Query q;

		q.name = attribs["name"];
		q.type = attribs["type"];
		q.table = attribs["table"];

		current_query = q.name;

		repository->groups[current_group].queries.insert(
				repository->groups[current_group].queries.end(),
				std::make_pair<std::string, XML::Query>(q.name, q));

		in_query = true;
	}
	else if (name == "in")
	{
		assert(in_group && in_repository && in_query);

		in_in = true;
	}
	else if (name == "param")
	{
		assert(in_group && in_repository && in_query && in_in);

		repository->groups[current_group].queries[current_query].params.push_back(attribs["name"]);
		in_param = true;
	}
	else if (name == "sql")
	{
		assert(in_group && in_repository && in_query);
		in_sql = true;
	}
	else if (name == "table")
	{
		assert(in_group && in_repository && in_query && in_sql);

		unsigned n = repository->groups[current_group].queries[current_query].tables.size();

		std::string tname = repository->groups[current_group].queries[current_query].table;

		if (attribs.count("name"))
		{
			tname = attribs["name"];
		}
		bool found = false;
		unsigned int idx = 0;
		unsigned int j;
		for (j = 0; j < n; j++)
		{
			if (repository->groups[current_group].queries[current_query].tables[j] == tname)
			{
				found = true;
				idx = j;
			}
		}
		if (found)
		{
			repository->groups[current_group].queries[current_query].build_table.push_back(idx);
		}
		else
		{
			repository->groups[current_group].queries[current_query].tables.push_back(tname);
			repository->groups[current_group].queries[current_query].build_table.push_back(j);
		}

		repository->groups[current_group].queries[current_query].build_order.push_back(XML::BUILD_TABLE);

		in_table = true;
	}
	else if (name == "input")
	{
		assert(in_group && in_repository && in_query && in_sql);

		unsigned n = repository->groups[current_group].queries[current_query].inputs.size();

		std::string tname = repository->groups[current_group].queries[current_query].name;

		if (attribs.count("name"))
		{
			tname = attribs["name"];
		}
		bool found = false;
		unsigned int idx = 0;
		unsigned int j;
		for (j = 0; j < n; j++)
		{
			if (repository->groups[current_group].queries[current_query].inputs[j] == tname)
			{
				found = true;
				idx = j;
			}
		}
		if (found)
		{
			repository->groups[current_group].queries[current_query].build_param.push_back(idx);
		}
		else
		{
			repository->groups[current_group].queries[current_query].inputs.push_back(tname);
			repository->groups[current_group].queries[current_query].build_param.push_back(j);
		}

		repository->groups[current_group].queries[current_query].build_order.push_back(XML::BUILD_PARAM);
		in_input = true;
	}
	else
		std::cerr << "UNKNOWN: " << name << std::endl;

	XMLString::release(&msg);
}

void Handler::endElement(const XMLCh *const uri,
			 const XMLCh *const localname,
			 const XMLCh *const qname)
{
	char *msg = XMLString::transcode(qname);
	std::string name(msg);

	DEBUG("XML END: " << name);
	if (name == "repository") in_repository = false;
	else if (name == "group")
	{
		group_stack.pop_back();
		in_group = false;
	}
	else if (name == "query") in_query = false;
	else if (name == "in") in_in = false;
	else if (name == "param") in_param = false;
	else if (name == "sql") in_sql = false;
	else if (name == "table") in_table = false;
	else if (name == "input") in_input = false;
	else
		std::cerr << "UNKNOWN: " << name << std::endl;

	XMLString::release(&msg);
}

void Handler::characters(const XMLCh *const chars, const unsigned int length)
{
	if (in_group && in_repository && in_query && in_sql)
	{
		char *msg = XMLString::transcode(chars);
		std::string sql(msg);

		std::string stripped = " ";
		stripped += Util::stripnl(sql);
		stripped += " ";

		DEBUG("XML PCDATA: " << sql);
		repository->groups[current_group]
			.queries[current_query].sql.push_back(sql);
		repository->groups[current_group]
			.queries[current_query]
				.build_order.push_back(XML::BUILD_SQL);

		XMLString::release(&msg);
	}
}

void Handler::fatalError(const SAXParseException &excep)
{
	char *msg = XMLString::transcode(excep.getMessage());

	std::cout << "Fatal Error: " << msg
		  << "at line: " << excep.getLineNumber()
		  << std::endl;
}

XCDR_NAMESPACE_END
