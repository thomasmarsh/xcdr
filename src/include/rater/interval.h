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
// $Source: /usr/local/cvs/xcdr/src/include/rater/interval.h,v $
// $Revision: 1.4 $
// $Date: 2003/04/04 16:03:58 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: interval.h,v $
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

#ifndef XCDR_RATER_INTERVAL_H
#define XCDR_RATER_INTERVAL_H

#include "common/slist.h"

XCDR_NAMESPACE_BEGIN

struct Interval
{
	unsigned int i_interval;
	unsigned int i_interval_index;
	unsigned int duration;
	double price;

	Interval()
	{
		i_interval = 0;
		i_interval_index = 0;
		duration = 0;
		price = 0;
	}
	~Interval()
	{
	}

	inline std::size_t count() const { return 1; }
	inline std::size_t deepCount() const { return 1; }

	inline std::size_t size() const
	{
		return sizeof(Interval);
	}
};

typedef slist<Interval> IntervalList;

XCDR_NAMESPACE_END

#endif // XCDR_RATER_INTERVAL_H
