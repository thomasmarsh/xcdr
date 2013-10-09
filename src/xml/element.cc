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
// $Source: /usr/local/cvs/xcdr/src/xml/element.cc,v $
// $Revision: 1.4 $
// $Date: 2003/04/04 16:06:36 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: element.cc,v $
// Revision 1.4  2003/04/04 16:06:36  tmarsh
// namespace changes
//
// Revision 1.3  2003/03/13 13:15:53  tmarsh
// fixes
//
// Revision 1.2  2003/03/13 07:53:48  tmarsh
// break out Element and RootElement
//
// Revision 1.1  2003/03/12 20:45:49  tmarsh
// added
//
//
// =========================================================================

#include "xml/element.h"
#include "common/logging.h"

XCDR_NAMESPACE_BEGIN

Element *Element::getParent()
{
	ENTER();
	LEAVE();
	return parent;
}

void Element::setName(const std::string &_name)
{
	ENTER();
	elem_name = _name;
	LEAVE();
}

void Element::setHandleChars(bool b)
{
	ENTER();
	wantCharacters = b;
	LEAVE();
}

void Element::addContained(const std::string &name, Element *prototype)
{
	ENTER();
	parent->addContained(name, prototype);
	creates.push_back(name);
	LEAVE();
}

void Element::addRequired(const std::string &name)
{
	ENTER();
	required_arguments.push_back(name);
	LEAVE();
}

void Element::addOptional(const std::string &name)
{
	ENTER();
	optional_arguments.push_back(name);
	LEAVE();
}

void Element::validate()
{
	ENTER();
	assert(elem_name != "");
	LEAVE();
}

void Element::characters(const std::string &chars)
{
	ENTER();
	LEAVE();
}

void Element::startChild(Element *element)
{
}

void Element::endChild(Element *element)
{
}

void Element::validateParams(const AttributeMap &attrs)
{
	ENTER();
	AttributeMap::const_iterator i = attrs.begin();
	while (i != attrs.end())
	{
		i++;
	}
	LEAVE();
}


// handler functions

void RootElement::addContained(const std::string &name, Element *prototype)
{
	ENTER();
	prototype->parent = this;
	prototype->initialize();
	DEBUG("addContained: " << name << " -> " << prototype->getName());
	if (!proto.count(name))
	{
		proto[name] = prototype;
		creates.push_back(name);
		DEBUG("add prototype: " << name << " -> " << proto[name]->getName());
	}
	else
	{
		delete prototype;
	}
	LEAVE();
}

void RootElement::handleStart(const std::string &name, AttributeMap &attrs)
{
	ENTER();
	DEBUG("stack size: " << stack.size());
	if (stack.size() > 0)
	{
		bool found = false;
		std::list<std::string>::iterator i = current->creates.begin();
		while (i != current->creates.end())
		{
			if ((*i) == name)
			{
				found = true;
				break;
			}
			i++;
		}
		if (!found)
		{
			FATAL("Cannot create in this context: " << name
						<< "; path=" << getPath());
			LEAVE();
			exit(1);
		}

		DEBUG("Create new element: " << name);
		if (!proto.count(name))
		{
			FATAL("No such element in factory: " << name);
			LEAVE();
			exit(1);
		}
		current = proto[name]->create();
		current->parent = stack.back();
		current->initialize();
		if (current->elem_name != name)
		{
			FATAL("current->elem_name != expected");
			FATAL("\t\"" << current->elem_name << "\" != \"" << name << "\"");
			FATAL(proto[name]->elem_name);
			exit(1);
		}
		stack.push_back(current);
	}
	else
	{
		current = this;
		parent = this;
		current->initialize();
		stack.push_back(current);
	}
	DEBUG("new path: " << getPath());

	current->validate();
	current->validateParams(attrs);
	current->bind(attrs);

	// send a startChild signal for newly construct child
	current->parent->startChild(current);

	LEAVE();
}

std::string RootElement::getPath()
{
	ENTER();
	std::string path = "";
	std::list<Element*>::iterator i = stack.begin();
	while (i != stack.end())
	{
		path += "/" + (*i)->elem_name;
		i++;
	}
	LEAVE();
	return path;
}

void RootElement::handleStop(const std::string &name)
{
	ENTER();
	DEBUG("got stop: " << name);
	DEBUG("stack size: " << stack.size());
	current->parent->endChild(current);
	DEBUG("popping: " << current->elem_name);
	stack.pop_back();

	if (stack.size() > 0)
		current = stack.back();
	else
		current = NULL;
	DEBUG("new path: " << getPath());
	LEAVE();
}

void RootElement::startElement(
	const XMLCh* const	uri,
	const XMLCh* const	localname,
	const XMLCh* const	qname,
	const Attributes&	attrs)
{
	ENTER();
	AttributeMap attribs;
	typedef std::pair<std::string,std::string> nameval;

	char *_name = XMLString::transcode(qname);
	std::string name(_name);
	XMLString::release(&_name);

	DEBUG("NAME: " << name);

	unsigned int len = attrs.getLength();
	for (unsigned int i = 0; i < len; i++)
	{
		char *str = XMLString::transcode(attrs.getLocalName(i));
		char *val = XMLString::transcode(attrs.getValue(i));

		attribs.insert(attribs.end(),
				std::make_pair<std::string,std::string>(std::string(str), std::string(val)));

		DEBUG("\tATTR: \"" << str << "\" -> \"" << val << "\"");

		XMLString::release(&str);
		XMLString::release(&val);
	}


	handleStart(name, attribs);
	LEAVE();
}

void RootElement::endElement(
	const XMLCh* const	uri,
	const XMLCh* const	localname,
	const XMLCh* const	qname)
{
	ENTER();
	char *_name = XMLString::transcode(qname);
	std::string name(_name);
	XMLString::release(&_name);

	handleStop(name);
	LEAVE();
}

void RootElement::characters(
	const XMLCh *const	chars,
	const unsigned int	length)
{
	ENTER();
	if (current->wantCharacters)
	{
		char *_text = XMLString::transcode(chars);
		std::string text(_text);
		XMLString::release(&_text);

		current->characters(text);
	}
	LEAVE();
}

void RootElement::fatalError(const SAXParseException &excep)
{
	ENTER();
	char *msg = XMLString::transcode(excep.getMessage());

	FATAL("Fatal Error: " << msg << "at line: " << excep.getLineNumber());
	TODO("throw exception");

	XMLString::release(&msg);
	LEAVE();
}

XCDR_NAMESPACE_END
