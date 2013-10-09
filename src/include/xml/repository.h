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
// $Source: /usr/local/cvs/xcdr/src/include/xml/repository.h,v $
// $Revision: 1.3 $
// $Date: 2003/04/04 16:03:58 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: repository.h,v $
// Revision 1.3  2003/04/04 16:03:58  tmarsh
// namespace changes
//
// Revision 1.2  2003/03/09 18:37:52  tmarsh
// removed debugging
//
// Revision 1.1  2003/03/09 08:35:32  tmarsh
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

#ifndef XCDR_XML_REPOSITORY_H
#define XCDR_XML_REPOSITORY_H

#include <xercesc/sax2/DefaultHandler.hpp>

#include <list>
#include <string>

#include "common/logging.h"
#include "rdbms/repository.h"

XCDR_NAMESPACE_BEGIN

namespace XMLRepository
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

		XML::Repository *getRepository()
		{
			return repository;
		}

	private:
		bool in_repository, in_group, in_query, in_in,
		     in_param, in_sql, in_table, in_input;

		std::string current_group;
		std::string current_query;

		std::list<std::string> group_stack;

		XML::Repository *repository;
	};
}

XCDR_NAMESPACE_END

#endif // XCDR_XML_REPOSITORY_H
