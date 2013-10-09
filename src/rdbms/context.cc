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
// $Source: /usr/local/cvs/xcdr/src/rdbms/context.cc,v $
// $Revision: 1.3 $
// $Date: 2003/04/04 16:06:36 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: context.cc,v $
// Revision 1.3  2003/04/04 16:06:36  tmarsh
// namespace changes
//
// Revision 1.2  2003/03/09 08:28:22  tmarsh
// include file reorganization
//
// Revision 1.1  2003/03/07 13:27:34  tmarsh
// added
//
// Revision 1.1  2003/03/07 11:26:44  tmarsh
// added
//
// Revision 1.4  2003/03/06 11:20:02  tmarsh
// updated headers
//
//
// =========================================================================

#include "rdbms/context.h"

XCDR_NAMESPACE_BEGIN

RDBMS::Context *RDBMS::_GlobalContext = NULL;

RDBMS::Context *RDBMS::GetContext()
{
	if (RDBMS::_GlobalContext == NULL)
	{
		RDBMS::_GlobalContext = new RDBMS::Context();
	}
	return RDBMS::_GlobalContext;
}
using namespace RDBMS;


Context::Context()
{
	connection = NULL;
}

Connection *Context::getConnection()
{
	ENTER();
	if (!connection)
	{
		DEBUG("allocating new connection");
		connection = new Connection();
		connection->set_Host("localhost");
		connection->set_User("xcdr");
		connection->set_Password("xcdr");

		connection->connect();
		connection->select_database("xcdr");
		connection->execute("SET AUTOCOMMIT=0");
	}
	LEAVE();
	return connection;
}


Context::~Context()
{
	if (connection)
		delete connection;
}

void Context::begin()
{
	DEBUG("BEGIN");
	execute("BEGIN");
}

void Context::rollback()
{
	DEBUG("ROLLBACK");
	execute("ROLLBACK");
}

void Context::commit()
{
	DEBUG("COMMIT");
	execute("COMMIT");
}

void Context::execute(const std::string &sql)
{

	if (!connection)
		connection = getConnection();
	try
	{
		DEBUG("executeSQL");
		ResultNoData r = connection->execute(sql);
		DEBUG("SQL EXECUTE:      " << (r.get_succeeded() ?
						"succeeded" : "failed"));
		DEBUG("   Insert id:     " << r.get_insert_id());
		DEBUG("   Rows affected: " << r.get_rows_affected());
		DEBUG("   Info:          " << r.get_info());
	}
	catch (mysqlcppapi::ex_BadQuery &er)
	{
		ERROR("BadQuery: " << er.what());
		DEBUG("Original SQL was: " << sql);
		throw (er);
	}
	catch (mysqlcppapi::ex_BadConversion &er)
	{
		ERROR("Tried to convert \"" << er.get_Data()
				<< "\" to a \""
				<< er.get_TypeName());
		throw (er);
	}
	catch (mysqlcppapi::ex_base &er)
	{
		ERROR("Unclassified: " << er.what());
		throw (er);
	}
}

XCDR_NAMESPACE_END
