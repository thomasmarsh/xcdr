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
// $Source: /usr/local/cvs/xcdr/src/include/gateway/sonus/sonus.h,v $
// $Revision: 1.3 $
// $Date: 2003/04/04 16:03:58 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: sonus.h,v $
// Revision 1.3  2003/04/04 16:03:58  tmarsh
// namespace changes
//
// Revision 1.2  2003/03/09 08:44:12  tmarsh
// include file reorganization
//
// Revision 1.1  2003/03/09 08:35:32  tmarsh
// include file reorganization
//
// Revision 1.8  2003/03/07 18:09:01  tmarsh
// updates for new rdbms and repository
//
// Revision 1.7  2003/03/06 11:20:03  tmarsh
// updated headers
//
//
// =========================================================================

#ifndef XCDR_SONUS_H
#define XCDR_SONUS_H

#include "common/datetime.h"
#include "common/csv.h"
#include "common/logging.h"

#include "gateway/gateway.h"
#include "gateway/sonus/sonus_records.h"

XCDR_NAMESPACE_BEGIN

using Parser::CSVParser;

namespace Sonus
{

	class Parser : public CSVParser
	{
	public:
		Parser(const std::string &line);

		std::string callID();

		std::string timestamp();

		inline unsigned int cdrType() { return cdr_type; }
	private:
		unsigned int cdr_type;
	};

	class Mediator : public Gateway::Mediator
	{
	public:
		Mediator();

		DateTime getDateTime(Parser &p, unsigned int d, unsigned int t);

		CDR::Mediated mediate(Gateway::Call &recs);
	};
}

XCDR_NAMESPACE_END

#endif // XCDR_SONUS_H
