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
// $Source: /usr/local/cvs/xcdr/src/include/rdbms/sql.h,v $
// $Revision: 1.3 $
// $Date: 2003/04/04 16:03:58 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: sql.h,v $
// Revision 1.3  2003/04/04 16:03:58  tmarsh
// namespace changes
//
// Revision 1.2  2003/03/09 18:38:47  tmarsh
// added table type
//
// Revision 1.1  2003/03/09 08:35:32  tmarsh
// include file reorganization
//
// Revision 1.3  2003/03/07 17:23:08  tmarsh
// updates to compile
//
// Revision 1.2  2003/03/07 13:27:07  tmarsh
// fixes to compile
//
// Revision 1.1  2003/03/07 11:26:44  tmarsh
// added
//
//
// =========================================================================

#ifndef XCDR_RDBM_SQL_H
#define XCDR_RDBM_SQL_H

#include "rdbms/model.h"
#include "common/util.h"
#include "common/logging.h"
#include "common/common.h"

#include <list>
#include <string>

XCDR_NAMESPACE_BEGIN

namespace SQLBinding
{
	inline bool isIntType(std::string &type)
	{
		if ((type == "TINYINT")	  ||
		    (type == "SMALLINT")  ||
		    (type == "MEDIUMINT") ||
		    (type == "INT")       ||
		    (type == "INTEGER")   ||
		    (type == "BIGINT"))
			return true;

		return false;
	}

	inline bool isFloatType(std::string &type)
	{
		if ((type == "REAL")    ||
		    (type == "DOUBLE")  ||
		    (type == "FLOAT")   ||
		    (type == "DECIMAL") ||
		    (type == "NUMERIC"))
			return true;

		return false;
	}

	inline bool isCharType(std::string &type)
	{
		if ((type == "CHAR") || (type == "VARCHAR"))
			return true;
		return false;
	}

	inline std::string CreateAttribute(Model::Attribute &a)
	{
		// write the name and type

		std::string sql = a.name;
		sql += " ";
		sql += a.externalType;

		bool is_char = isCharType(a.externalType);
		bool is_int = isIntType(a.externalType);
		bool is_float = isFloatType(a.externalType);

		// Write the width information

		if (is_int || is_char)
		{
			if (a.width > 0)
			{
				sql += "(";
				sql += Util::itoa(a.width);
				sql += ")";
			}
		}
		else if (is_float)
		{
			sql += "(";
			sql += Util::itoa(a.width);
			sql += ", ";
			sql += Util::itoa(a.precision);
			sql += ")";
		}

		if (is_int || is_float)
		{
			if (a.unsign)
				sql += " UNSIGNED";
			if (a.zeroFill)
				sql += " ZEROFILL";
		}

		if (is_char)
		{
			if (a.binary)
				sql += " BINARY";
		}

		// handle the not null case

		if (!a.null) sql += " NOT NULL";


		// handle the default value

		if (a.defaultValue.length() > 0)
		{
			sql += " DEFAULT ";
			sql += a.defaultValue;
		}
		return sql;
	}

	inline std::string CreateTable(Model::Entity &e)
	{
		std::string sql = "CREATE TABLE ";
		sql += e.name;
		sql += "\n (\n";

		Model::AttribList::iterator i = e.attributes.begin();
		while (i != e.attributes.end())
		{
			sql += "\t";
			sql += CreateAttribute(*i);
			i++;
			sql += ((i != e.attributes.end()) ? ",\n" : "\n");
		}
		sql += ")";

		if (e.type != "")
		{
			sql += " TYPE=";
			sql += e.type;
		}

		return sql;
	}

	inline std::string DropTable(Model::Entity &e)
	{
		return "DROP TABLE " + e.name;
	}

	inline std::string TruncateTable(Model::Entity &e)
	{
		return "TRUNCATE TABLE " + e.name;
	}
}

XCDR_NAMESPACE_END

#endif // XCDR_RDBMS_SQL_H
