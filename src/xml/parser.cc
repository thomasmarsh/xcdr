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
// $Source: /usr/local/cvs/xcdr/src/xml/parser.cc,v $
// $Revision: 1.3 $
// $Date: 2003/04/04 16:06:36 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: parser.cc,v $
// Revision 1.3  2003/04/04 16:06:36  tmarsh
// namespace changes
//
// Revision 1.2  2003/03/13 13:16:22  tmarsh
// fixes in includes
//
// Revision 1.1  2003/03/13 07:57:46  tmarsh
// added
//
//
// =========================================================================

#include "xml/parser.h"
#include "common/logging.h"

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>

using namespace xercesc;

XCDR_NAMESPACE_BEGIN

XMLParser::XMLParser()
{
	ENTER();
	try
	{
		TODO("move XMLPlatformUtils::Initialize() to some static place");
		XMLPlatformUtils::Initialize();
	}
	catch (const XMLException &xmlexc)
	{
		char *msg = XMLString::transcode(xmlexc.getMessage());
		FATAL(msg);
		XMLString::release(&msg);
		TODO("rethrow");
		exit(1);
	}
	parser = XMLReaderFactory::createXMLReader();
	parser->setFeature(XMLUni::fgSAX2CoreValidation, true);
	parser->setFeature(XMLUni::fgSAX2CoreNameSpaces, true);
	LEAVE();
}

XMLParser::~XMLParser()
{
	ENTER();
	delete parser;
	LEAVE();
}

void XMLParser::parse(const std::string &path, RootElement *root)
{
	ENTER();
	parser->setContentHandler(root);
	parser->setErrorHandler(root);
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
		exit(1);
        }
        catch (const SAXParseException &saxexc)
        {
                char *msg = XMLString::transcode(saxexc.getMessage());
                FATAL(msg);
                XMLString::release(&msg);
                TODO("rethrow SAXParseException");
		exit(1);
        }
        catch (...)
        {
                FATAL("unexpected exception");
                TODO("rethrow other exception");
		exit(1);
        }
	LEAVE();
}

XCDR_NAMESPACE_END
