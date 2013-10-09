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
// $Source: /usr/local/cvs/xcdr/src/include/xml/element.h,v $
// $Revision: 1.5 $
// $Date: 2003/04/04 16:03:58 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: element.h,v $
// Revision 1.5  2003/04/04 16:03:58  tmarsh
// namespace changes
//
// Revision 1.4  2003/03/21 15:13:27  tmarsh
// virtual dtor
//
// Revision 1.3  2003/03/14 17:57:33  tmarsh
// big xml changes
//
// Revision 1.2  2003/03/13 07:53:25  tmarsh
// break out Element and RootElement
//
// Revision 1.1  2003/03/12 20:45:07  tmarsh
// added
//
//
// =========================================================================

#ifndef XCDR_XML_ELEMENT_H
#define XCDR_XML_ELEMENT_H

#include "common/common.h"

#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/sax2/Attributes.hpp>

#include <string>
#include <map>
#include <list>


XCDR_NAMESPACE_BEGIN

class RootElement;

class Element
{
public:
	friend class RootElement;

	virtual Element *create() { assert(1==0); return NULL; }
	virtual void initialize() = 0;

	virtual ~Element() {}

	const std::string &getName() { return elem_name; }

protected:
	typedef std::map<std::string,std::string> AttributeMap;

	std::string elem_name;
	std::list<std::string> required_arguments;
	std::list<std::string> optional_arguments;

	std::list<std::string> creates;

	Element *parent;
	bool wantCharacters;

	void validate();
	void validateParams(const AttributeMap &attrs);

	void setName(const std::string &_name);
	void setHandleChars(bool b);
	virtual void addContained(const std::string &name, Element *prototype);
	void addRequired(const std::string &name);
	void addOptional(const std::string &name);

	// handler functions

	virtual void characters(const std::string &chars);
	virtual void startChild(Element *child);
	virtual void endChild(Element *child);

	virtual void bind(AttributeMap &attrs) = 0;

	Element *getParent();
};

using namespace xercesc;

class RootElement : public Element, public DefaultHandler
{
private:
	Element *current;
	std::list<Element*> stack;

	std::map<std::string,Element*> proto;
protected:
	void addContained(const std::string &name, Element *prototype);

	void handleStart(const std::string &name, AttributeMap &attrs);

	std::string getPath();

	void handleStop(const std::string &name);
public:
	virtual ~RootElement() {}
	void startElement(
		const XMLCh* const	uri,
		const XMLCh* const	localname,
		const XMLCh* const	qname,
		const Attributes&	attrs);

	void endElement(
		const XMLCh* const	uri,
		const XMLCh* const	localname,
		const XMLCh* const	qname);

	void characters(
		const XMLCh *const	chars,
		const unsigned int	length);

	void fatalError(const SAXParseException &excep);
};

XCDR_NAMESPACE_END


#endif // XCDR_XML_ELEMENT_H
