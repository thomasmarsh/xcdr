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
// $Source: /usr/local/cvs/xcdr/src/include/rater/query.h,v $
// $Revision: 1.3 $
// $Date: 2003/04/04 16:03:58 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: query.h,v $
// Revision 1.3  2003/04/04 16:03:58  tmarsh
// namespace changes
//
// Revision 1.2  2003/03/27 19:15:22  tmarsh
// rater rewrite
//
// Revision 1.1  2003/03/17 23:47:22  tmarsh
// moved to root build dir
//
// Revision 1.2  2003/03/15 21:08:39  tmarsh
// cleanups
//
//
// =========================================================================

#ifndef XCDR_ENGINE_RATER_QUERY_H
#define XCDR_ENGINE_RATER_QUERY_H

#include "rater/ident.h"

XCDR_NAMESPACE_BEGIN

//using namespace icu;

struct Query
{
	char *cli;
	char *cld;

	bool payphone;
	bool prison;

//	UDate effective_start;
//	UDate effective_end;
	unsigned int duration;

	IdentifierList identifiers;

//	Calendar *calendar;
//	TimeZone *timezone;

	Query()
	{
		cli = 0;
		cld = 0;
		payphone = false;
		prison = false;
		duration = 0;
	}
};

XCDR_NAMESPACE_END

#endif // XCDR_ENGINE_RATER_QUERY_H
