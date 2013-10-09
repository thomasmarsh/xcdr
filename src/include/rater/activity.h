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
// $Source: /usr/local/cvs/xcdr/src/include/rater/activity.h,v $
// $Revision: 1.4 $
// $Date: 2003/04/04 16:03:58 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: activity.h,v $
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

#ifndef XCDR_RATER_ACTIVITY_H
#define XCDR_RATER_ACTIVITY_H

#include "common/slist.h"

#include <cstring>

XCDR_NAMESPACE_BEGIN

struct Activity
{
	unsigned int i_activity;
	char *value;

	Activity()
	{
		i_activity = 0;
		value = 0;
	}

	~Activity()
	{
		if (value) delete value;
	}

	inline std::size_t count() const { return 1; }
	inline std::size_t deepCount() const { return 1; }

	inline std::size_t size() const
	{
		std::size_t sz = sizeof(Activity);
		if (value) sz += sizeof(char) * std::strlen(value);
		return sz;
	}

	inline bool match(Activity *other) const
	{
		return ((i_activity == other->i_activity) &&
			(((value == 0) && (other->value == 0)) ||
			 (strcmp(value, other->value) == 0)));
	}
};

typedef slist<Activity> ActivityList;

XCDR_NAMESPACE_END

#endif // XCDR_RATER_ACTIVITY_H
