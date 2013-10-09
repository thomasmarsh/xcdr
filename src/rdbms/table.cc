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
// $Source: /usr/local/cvs/xcdr/src/rdbms/table.cc,v $
// $Revision: 1.5 $
// $Date: 2003/04/04 16:06:36 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: table.cc,v $
// Revision 1.5  2003/04/04 16:06:36  tmarsh
// namespace changes
//
// Revision 1.4  2003/03/09 18:41:11  tmarsh
// use the SQLBinding
//
// Revision 1.3  2003/03/09 12:28:38  tmarsh
// implemented
//
// Revision 1.2  2003/03/09 08:28:22  tmarsh
// include file reorganization
//
// Revision 1.1  2003/03/07 11:26:44  tmarsh
// added
//
//
// =========================================================================

#include "rdbms/sql.h"
#include "rdbms/table.h"
#include "common/logging.h"

XCDR_NAMESPACE_BEGIN

using namespace RDBMS;

void Table::create()
{
	std::string todo = SQLBinding::CreateTable(entity);
	TODO(todo);
}

void Table::drop()
{
	std::string todo = SQLBinding::DropTable(entity);
	TODO(todo);
}

void Table::truncate()
{
	std::string todo = SQLBinding::TruncateTable(entity);
	TODO(todo);
}

ResultStore Table::fetchAll()
{
	ResultStore x;
	TODO("implement");
	return x;
}

bool Table::exists()
{
	TODO("implement");
	return true;
}

bool Table::schemaDifference()
{
	TODO("implement");
	return false;
}

XCDR_NAMESPACE_END
