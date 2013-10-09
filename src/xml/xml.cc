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
// $Source: /usr/local/cvs/xcdr/src/xml/xml.cc,v $
// $Revision: 1.9 $
// $Date: 2003/04/04 16:06:36 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: xml.cc,v $
// Revision 1.9  2003/04/04 16:06:36  tmarsh
// namespace changes
//
// Revision 1.8  2003/03/25 10:30:55  tmarsh
// re-added
//
// Revision 1.7  2003/03/13 13:16:46  tmarsh
// removed
//
// Revision 1.6  2003/03/09 08:33:32  tmarsh
// fixed logging directives
//
// Revision 1.5  2003/03/09 08:32:58  tmarsh
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

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>

#include "xml/xml.h"
#include "common/logging.h"

XCDR_NAMESPACE_BEGIN

SimpleXMLParser::SimpleXMLParser()
{
	parser = NULL;
	model_handler = NULL;

	try
	{
		XMLPlatformUtils::Initialize();
	}
	catch (const XMLException &xmlex)
	{
		char *msg = XMLString::transcode(xmlex.getMessage());
		FATAL(msg);
		XMLString::release(&msg);
		TODO("RETHROW");
	}

	parser = XMLReaderFactory::createXMLReader();
	parser->setFeature(XMLUni::fgSAX2CoreValidation, true);
	parser->setFeature(XMLUni::fgSAX2CoreNameSpaces, true);

	model_handler = new XMLModel::Handler();
	repository_handler = new XMLRepository::Handler();
}

SimpleXMLParser::~SimpleXMLParser()
{
	if (parser) delete parser;
	if (model_handler) delete model_handler;
}

Model::Model *SimpleXMLParser::getModel(const std::string &path)
{
	parser->setContentHandler(model_handler);
	parser->setErrorHandler(model_handler);

	parse(path);

	return model_handler->getModel();
}

XML::Repository *SimpleXMLParser::getRepository(const std::string &path)
{
	parser->setContentHandler(repository_handler);
	parser->setErrorHandler(repository_handler);

	parse(path);

	return repository_handler->getRepository();
}

void SimpleXMLParser::parse(const std::string &path)
{
	try
	{
		parser->parse(path.c_str());
	}
	catch (const XMLException &xmlex)
	{
		char *msg = XMLString::transcode(xmlex.getMessage());
		FATAL(msg);
		XMLString::release(&msg);
		TODO("rethrow XMLException");
	}
	catch (const SAXParseException &saxex)
	{
		char *msg = XMLString::transcode(saxex.getMessage());
		FATAL(msg);
		XMLString::release(&msg);
		TODO("rethrow SAXParseException");
	}
	catch (...)
	{
		FATAL("unexpected exception");
		TODO("rethrow other exception");
	}
}

XCDR_NAMESPACE_END
