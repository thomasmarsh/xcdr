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
// $Source: /usr/local/cvs/xcdr/src/include/xml/model.h,v $
// $Revision: 1.3 $
// $Date: 2003/04/04 16:03:58 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: model.h,v $
// Revision 1.3  2003/04/04 16:03:58  tmarsh
// namespace changes
//
// Revision 1.2  2003/03/09 18:38:07  tmarsh
// added some handler methods
//
// Revision 1.1  2003/03/09 08:35:32  tmarsh
// include file reorganization
//
// Revision 1.4  2003/03/07 13:36:41  tmarsh
// fixes to compile
//
// Revision 1.3  2003/03/06 11:20:03  tmarsh
// updated headers
//
//
// =========================================================================

#ifndef XCDR_XML_MODEL_H
#define XCDR_XML_MODEL_H

#include <xercesc/sax2/DefaultHandler.hpp>

#include <list>
#include <string>

#include "rdbms/model.h"

XCDR_NAMESPACE_BEGIN

namespace XMLModel
{
	//
	// An XML handler to process Model files
	//

	using namespace xercesc;

	class Handler : public DefaultHandler
	{
	public:
		Handler();
		~Handler();

		std::string stateString();

		void startElement(
			const XMLCh* const	uri,
			const XMLCh* const	localname,
			const XMLCh* const	qname,
			const Attributes&	attrs);

		void endElement(
			const XMLCh* const	uri,
			const XMLCh* const	localname,
			const XMLCh* const	qname);

		void fatalError(const SAXParseException &);

		void characters(const XMLCh *const chars, const unsigned int length);

		void Handler::handleProfile(Model::Profile &p,
				            std::map<std::string, std::string> &attribs);
		void Handler::handleAttrib(Model::Attribute &a,
				           std::map<std::string, std::string> &attribs);

		Model::Model *getModel() { return model; }

	private:
		bool in_model, in_typedef, in_profile, in_entity,
		     in_attrib, in_populate, in_record, in_a,
		     in_relation, in_join, in_source, in_destination;

		std::string current_typedef;
		std::string current_entity;

		std::list<std::string> pop_buf;
		std::string col_buf;

		Model::Model *model;
	};
}

XCDR_NAMESPACE_END

#endif // XCDR_XML_MODEL_H
