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
// $Source: /usr/local/cvs/xcdr/src/include/rater/period.h,v $
// $Revision: 1.5 $
// $Date: 2003/05/05 17:20:31 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: period.h,v $
// Revision 1.5  2003/05/05 17:20:31  tmarsh
// misc updates
//
// Revision 1.4  2003/04/04 16:03:58  tmarsh
// namespace changes
//
// Revision 1.3  2003/03/28 17:46:24  tmarsh
// major updates; rater now works
//
// Revision 1.2  2003/03/27 19:15:22  tmarsh
// rater rewrite
//
//
// =========================================================================

#ifndef XCDR_RATER_PERIOD_H
#define XCDR_RATER_PERIOD_H

#include "common/slist.h"
//#include "common/period.h"

XCDR_NAMESPACE_BEGIN

struct Period
{
//	Period *period;
	unsigned int i_period;
	char *period_string;
	char *description;

	Period()
	{
		i_period = 0;
		period_string = 0;
		description = 0;
	}

	void initialize(unsigned int ip, char *ps, char *desc)
	{
		i_period = ip;
		period_string = ps;
		description = desc;
//		period = new Period(period_string);
	}

	~Period()
	{
		if (period_string) delete period_string;
		if (description) delete description;
	}

	inline bool inPeriod()
	{
//		return period->inPeriod
		return true;
	}

	inline std::size_t count() const { return 1; }
	inline std::size_t deepCount() const { return 1; }

	inline std::size_t size() const
	{
		std::size_t sz = sizeof(Period);
		if (period_string) sz += sizeof(char) * std::strlen(period_string);
		return sz;
	}
};

typedef slist<Period> PeriodList;

XCDR_NAMESPACE_END

#endif // XCDR_RATER_PERIOD_H
