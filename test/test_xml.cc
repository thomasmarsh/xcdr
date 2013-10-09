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
// $Source: /usr/local/cvs/xcdr/test/test_xml.cc,v $
// $Revision: 1.12 $
// $Date: 2003/04/04 16:07:10 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: test_xml.cc,v $
// Revision 1.12  2003/04/04 16:07:10  tmarsh
// namespace changes
//
// Revision 1.11  2003/03/27 19:21:09  tmarsh
// updates for parser
//
// Revision 1.10  2003/03/09 18:42:29  tmarsh
// test table
//
// Revision 1.9  2003/03/09 08:56:48  tmarsh
// include file reorganization
//
// Revision 1.8  2003/03/07 22:27:55  tmarsh
// added try/catch and corrected groupname
//
// Revision 1.7  2003/03/07 18:09:01  tmarsh
// updates for new rdbms and repository
//
// Revision 1.6  2003/03/07 13:46:48  tmarsh
// updated for changes in model structure
//
// Revision 1.5  2003/03/06 11:20:05  tmarsh
// updated headers
//
//
// =========================================================================


#include "xml/xml.h"
#include "rdbms/rdbms.h"
#include <iostream>

using namespace std;
using namespace XCDR;

void test()
{

	SimpleXMLParser xml;

	Model::Model *model = xml.getModel("model.xml");

	std::map<std::string,Model::Entity>::iterator i;
	i = model->entities.begin();

	while (i != model->entities.end())
	{
		std::cout << "entity: name="
			  << (*i).second.name << " type="
			  << (*i).second.type << std::endl;
		i++;
	}

	std::string test = "foo";
	RDBMS::Repository repository(test);

	std::string g = "Mediator";
	RDBMS::QueryGroup *group = repository.getGroup(g);

	std::list<std::string> keys;
	keys.push_back("1234");
	RDBMS::Parameters p;
	std::string x("keys");
	p.add(x, keys);

	std::string n("GetOffsets");
	group->query(n, p);

	RDBMS::Table t(*model, "CDR_Raw");

	t.create();
}



int main()
{
	Log::filter = Log::Trace | Log::Debug | Log::Info | Log::Warn |
		     Log::Error | Log::Fatal | Log::Exception | Log::Todo;

	try
	{
		test();
	}
	catch (mysqlcppapi::ex_base &exc)
	{
		FATAL(exc.what());
		cerr << exc.what() << endl << flush;
		exit(1);
	}
	catch (Exception &exc)
	{
		EXCEPTION("FATAL", exc);
		cerr << exc << endl << flush;
		exit(1);
	}
	catch (...)
	{
		FATAL("unknown exception");
		cerr << "other exception" << endl << flush;
		exit(1);
	}

	exit(0);
}
