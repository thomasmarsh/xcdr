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
// $Source: /usr/local/cvs/xcdr/src/include/xml/xml.h,v $
// $Revision: 1.4 $
// $Date: 2003/04/04 16:03:58 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: xml.h,v $
// Revision 1.4  2003/04/04 16:03:58  tmarsh
// namespace changes
//
// Revision 1.3  2003/03/27 19:18:46  tmarsh
// re-added old parser
//
// Revision 1.2  2003/03/14 17:57:33  tmarsh
// big xml changes
//
// Revision 1.1  2003/03/09 08:35:32  tmarsh
// include file reorganization
//
// Revision 1.4  2003/03/07 18:07:50  tmarsh
// fixes for repository
//
// Revision 1.3  2003/03/07 13:36:41  tmarsh
// fixes to compile
//
// Revision 1.2  2003/03/06 11:20:04  tmarsh
// updated headers
//
//
// =========================================================================

#ifndef XCDR_XML_XML_H
#define XCDR_XML_XML_H

#include "xml/model.h"
#include "xml/repository.h"

#include <xercesc/sax2/SAX2XMLReader.hpp>

XCDR_NAMESPACE_BEGIN

using namespace xercesc;

class SimpleXMLParser
{
public:
	SimpleXMLParser();
	~SimpleXMLParser();

	Model::Model *getModel(const std::string &path);
	XML::Repository *getRepository(const std::string &path);

	void parse(const std::string &path);
private:
	SAX2XMLReader		*parser;
	XMLModel::Handler	*model_handler;
	XMLRepository::Handler	*repository_handler;
};

XCDR_NAMESPACE_END

#endif // XCDR_XML_XML_H
