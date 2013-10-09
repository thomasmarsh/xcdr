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
// $Source: /usr/local/cvs/xcdr/src/include/rdbms/repository.h,v $
// $Revision: 1.2 $
// $Date: 2003/04/04 16:03:58 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: repository.h,v $
// Revision 1.2  2003/04/04 16:03:58  tmarsh
// namespace changes
//
// Revision 1.1  2003/03/09 08:35:32  tmarsh
// include file reorganization
//
// Revision 1.2  2003/03/07 17:23:08  tmarsh
// updates to compile
//
// Revision 1.1  2003/03/07 13:27:20  tmarsh
// added
//
// Revision 1.1  2003/03/07 11:26:44  tmarsh
// added
//
//
// =========================================================================

#ifndef XCDR_RDBMS_REPOSITORY_H
#define XCDR_RDBMS_REPOSITORY_H

#include <string>
#include <map>
#include <list>
#include <vector>

XCDR_NAMESPACE_BEGIN

namespace XML
{
	const unsigned int BUILD_SQL = 0;
	const unsigned int BUILD_TABLE = 1;
	const unsigned int BUILD_PARAM = 2;

	struct Query
	{
		std::string name;
		std::string type;
		std::string table;

		// inputs
		std::list<std::string> params;

		// sql
		std::list<std::string> sql;
		std::vector<std::string> tables;
		std::vector<std::string> inputs;

		// construction information
		std::list<unsigned int> build_order;
		std::list<unsigned int> build_table;
		std::list<unsigned int> build_param;
	};

	struct Group
	{
		std::string name;
		std::map<std::string, Query> queries;
	};

	struct Repository
	{
		std::string model;
		std::map<std::string, Group> groups;
	};
}

XCDR_NAMESPACE_END

#endif // XCDR_RDBMS_REPOSITORY_H
