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
// $Source: /usr/local/cvs/xcdr/src/xml/model.cc,v $
// $Revision: 1.8 $
// $Date: 2003/04/04 16:06:36 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: model.cc,v $
// Revision 1.8  2003/04/04 16:06:36  tmarsh
// namespace changes
//
// Revision 1.7  2003/03/09 18:40:32  tmarsh
// fixes for model parsing
//
// Revision 1.6  2003/03/09 08:32:58  tmarsh
// include file reorganization
//
// Revision 1.5  2003/03/07 18:07:50  tmarsh
// fixes for repository
//
// Revision 1.4  2003/03/07 13:36:41  tmarsh
// fixes to compile
//
// Revision 1.3  2003/03/06 11:20:03  tmarsh
// updated headers
//
//
// =========================================================================

#include "xml/model.h"
#include "common/util.h"

#include <xercesc/sax2/Attributes.hpp>

#include <map>
#include <iostream>
#include <cstdlib>

XCDR_NAMESPACE_BEGIN

using namespace XMLModel;

Handler::Handler()
{
	in_model = false;
	in_typedef = false;
	in_profile = false;
	in_entity = false;
	in_attrib = false;
	in_populate = false;
	in_record = false;
	in_a = false;
	in_relation = false;
	in_join = false;
	in_source = false;
	in_destination = false;
	model = NULL;
}

Handler::~Handler()
{
	if (model) delete model;
}

std::string Handler::stateString()
{
	std::string state("");

	if (in_model) state += "1"; else state += "0";
	if (in_typedef) state += "1"; else state += "0";
	if (in_profile) state += "1"; else state += "0";
	if (in_entity) state += "1"; else state += "0";
	if (in_attrib) state += "1"; else state += "0";
	if (in_populate) state += "1"; else state += "0";
	if (in_record) state += "1"; else state += "0";
	if (in_a) state += "1"; else state += "0";
	if (in_relation) state += "1"; else state += "0";
	if (in_join) state += "1"; else state += "0";
	if (in_source) state += "1"; else state += "0";
	if (in_destination) state += "1"; else state += "0";

	return state;
}

void Handler::handleProfile(Model::Profile &p,
			    std::map<std::string, std::string> &attribs)
{
	p.name = attribs["name"];
	p.type = attribs["type"];

	if (attribs.count("externalType"))
	{
		DEBUG("externalType: '" << attribs["externalType"] << "'");
		p.externalType = attribs["externalType"];
//		p.externalType = Util::toupper(p.externalType);
	}

	if (attribs["primaryKey"] == "true")
		p.primaryKey = true;
	else p.primaryKey = false;

	if (attribs["null"] == "false")
		p.null = false;
	else p.null = true;

	if (attribs["required"] == "true")
		p.required = true;
	else p.required = false;

	if (attribs["unsign"] == "true")
		p.unsign = true;
	else p.unsign = false;

	if (attribs["zeroFill"] == "zeroFill")
		p.zeroFill = true;
	else p.zeroFill = false;

	if (attribs["binary"] == "binary")
		p.binary = true;
	else p.binary = false;

	if (attribs.count("width"))
		p.width = atoi(attribs["width"].c_str());
	else p.width = 0;

	if (attribs.count("precision"))
		p.precision = atoi(attribs["precision"].c_str());
	else p.precision = 0;

	if (attribs.count("scale"))
		p.scale = atoi(attribs["scale"].c_str());
	else p.scale = 0;

	if (attribs.count("defaultValue"))
		p.defaultValue = attribs["defaultValue"];

	if (attribs.count("label"))
		p.label = attribs["label"];
}

void Handler::handleAttrib(Model::Attribute &p,
			   std::map<std::string, std::string> &attribs)
{
	p.name = attribs["name"];
	p.type = attribs["type"];

	if (attribs.count("externalType"))
	{
		p.externalType = attribs["externalType"];
	}

	if (attribs.count("primaryKey"))
	{
		if (attribs["primaryKey"] == "true")
			p.primaryKey = true;
		else p.primaryKey = false;
	}

	if (attribs.count("null"))
	{
		if (attribs["null"] == "false")
			p.null = false;
		else p.null = true;
	}

	if (attribs.count("required"))
	{
		if (attribs["required"] == "true")
			p.required = true;
		else p.required = false;
	}

	if (attribs.count("unsign"))
	{
		if (attribs["unsign"] == "true")
			p.unsign = true;
		else p.unsign = false;
	}

	if (attribs["zeroFill"] == "zeroFill")
		p.zeroFill = true;
	else p.zeroFill = false;

	if (attribs["binary"] == "binary")
		p.binary = true;
	else p.binary = false;

	if (attribs.count("width"))
		p.width = atoi(attribs["width"].c_str());
	else p.width = 0;

	if (attribs.count("precision"))
		p.precision = atoi(attribs["precision"].c_str());
	else p.precision = 0;

	if (attribs.count("scale"))
		p.scale = atoi(attribs["scale"].c_str());
	else p.scale = 0;

	if (attribs.count("defaultValue"))
		p.defaultValue = attribs["defaultValue"];

	if (attribs.count("label"))
		p.label = attribs["label"];
}

void Handler::startElement(const XMLCh *const uri,
			   const XMLCh *const localname,
			   const XMLCh *const qname,
			   const Attributes &attrs)
{
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
		attribs.insert(attribs.end(), nv);

		XMLString::release(&str);
		XMLString::release(&val);
	}

	char *msg = XMLString::transcode(qname);
	std::string name(msg);

	TODO("move each case to separate method");
	if (name == "model")
	{
		if (model) delete model;
		model = new Model::Model;

		model->version = attribs["version"];
		model->adaptor = attribs["adaptor"];
	
		in_model = true;
	}
	else if (name == "typedef")
	{
		assert(in_model);
		in_typedef = true;
	}
	else if (name == "profile")
	{
		assert(in_typedef);
		
		Model::Profile p;

		p.width = 0;
		p.precision = 0;
		p.scale = 0;

		handleProfile(p, attribs);

		current_typedef = p.name;
		model->typedefs.insert(
				model->typedefs.end(),
				std::make_pair<std::string,
					  Model::Profile>(p.name, p));

		in_profile = true;
	}
	else if (name == "entity")
	{
		assert(in_model);

		Model::Entity e;

		e.name = attribs["name"];
		e.type = attribs["type"];
		e.comment = attribs["comment"];

		current_entity = e.name;
		model->entities.insert(
				model->entities.end(),
				std::make_pair<std::string,
					  Model::Entity>(e.name, e));
		in_entity = true;
	}
	else if (name == "attrib")
	{
		assert(in_entity);
		Model::Attribute a;

		a.width = 0;
		a.precision = 0;
		a.scale = 0;
		if (attribs.count("profile"))
		{
			Model::Profile p = model->typedefs[attribs["profile"]];

			a.type = p.type;
			a.externalType = p.externalType;
			a.primaryKey = p.primaryKey;
			a.null = p.null;
			a.required = p.required;
			a.unsign = p.unsign;
			a.zeroFill = p.zeroFill;
			a.binary = p.binary;
			a.width = p.width;
			a.precision = p.precision;
			a.scale = p.scale;
			a.defaultValue = p.defaultValue;
			a.label = p.label;

		}

		handleAttrib(a, attribs);

		model->entities[current_entity].attributes.push_back(a);

		in_attrib = true;
	}
	else if (name == "populate")
	{
		assert(in_entity);
		in_populate = true;
	}
	else if (name == "record")
	{
		assert(in_populate);
		in_record = true;
	}
	else if (name == "a")
	{
		assert(in_record);
		in_a = true;
	}
	else if (name == "relation")
	{
		in_relation = true;
	}
	else if (name == "join")
	{
		in_join = true;
	}
	else if (name == "source")
	{
		in_source = true;
	}
	else if (name == "destination")
	{
		in_destination = true;
	}
	else
		std::cerr << "UNKNOWN: " << name << std::endl;

//	std::cout << "START: " << msg << " " << stateString() << std::endl;

	XMLString::release(&msg);
}

void Handler::endElement(const XMLCh *const uri,
			 const XMLCh *const localname,
			 const XMLCh *const qname)
{
	char *msg = XMLString::transcode(qname);
	std::string name(msg);

	if (name == "model")
	{
		in_model = false;
	}
	else if (name == "typedef")
	{
		in_typedef = false;
	}
	else if (name == "profile")
	{
		in_profile = false;
	}
	else if (name == "entity")
	{
		in_entity = false;
	}
	else if (name == "attrib")
	{
		in_attrib = false;
	}
	else if (name == "populate")
	{
		in_populate = false;
	}
	else if (name == "record")
	{
		model->entities[current_entity].populate.push_back(pop_buf);
		pop_buf.erase(pop_buf.begin(), pop_buf.end());
		in_record = false;
	}
	else if (name == "a")
	{
		pop_buf.push_back(col_buf);
		col_buf = "";
		in_a = false;
	}
	else if (name == "relation")
	{
		in_relation = false;
	}
	else if (name == "join")
	{
		in_join = false;
	}
	else if (name == "source")
	{
		in_source = false;
	}
	else if (name == "destination")
	{
		in_destination = false;
	}
	else
		std::cerr << "UNKNOWN: " << name << std::endl;

//	std::cout << "END: " << msg << " " << stateString() << std::endl;

	XMLString::release(&msg);
}

void Handler::characters(const XMLCh *const chars, const unsigned int length)
{
	if (in_a)
	{
		assert(in_record);
		assert(in_populate);
		assert(in_entity);
		assert(in_model);

		char *msg = XMLString::transcode(chars);
		std::string schars(msg);
		col_buf += schars;
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
