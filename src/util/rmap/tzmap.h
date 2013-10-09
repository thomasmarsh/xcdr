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
// $Source: /usr/local/cvs/xcdr/src/util/rmap/tzmap.h,v $
// $Revision: 1.2 $
// $Date: 2003/05/12 14:24:52 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: tzmap.h,v $
// Revision 1.2  2003/05/12 14:24:52  tmarsh
// some docs
//
// Revision 1.1  2003/04/08 14:30:53  tmarsh
// added
//
// Revision 1.3  2003/04/04 16:03:58  tmarsh
// namespace changes
//
// Revision 1.2  2003/03/27 19:15:22  tmarsh
// rater rewrite
//
// Revision 1.1  2003/03/17 23:47:22  tmarsh
// moved to root build dir
//
// Revision 1.2  2003/03/15 21:09:09  tmarsh
// added dtor and size method
//
// Revision 1.1  2003/03/14 17:56:43  tmarsh
// added
//
//
// =========================================================================

#ifndef XCDR_UTIL_RMAP_TZMAP_H
#define XCDR_UTIL_RMAP_TZMAP_H

#include "common/dtrie.h"
#include "common/ttree.h"
#include "common/logging.h"

#include "tok.h"
#include "loc_info.h"

#include <string>

#include <cstddef>
#include <cstring>

//XXX TEMP
#include <iostream>

XCDR_NAMESPACE_BEGIN

// See the documentation for rmap. The tzmap inherits from this, and only augments by
// keeping track of various location hints for determining the timezone.

struct tzmap : public rmap
{
	LocationMap *lmap;

	tzmap(LocationMap *l) : rmap(), lmap(l)
	{
	}

	bool hasLocation(const std::string &loc_name, const std::string &l)
	{
		Tokenizer t(l, "|");
		Tokenizer::iterator i = t.begin();
		while (i != t.end())
		{
			if ((*i) == loc_name)
				return true;
			i++;
		}
		return false;
	}

	void insert(const std::string &dest, const std::string &state, const std::string &loc_name)
	{
		if (!lmap->count(state))
		{
			WARN("No timezone for state: " << state);
			return;
		}
		else
		{
			std::string tz = (*lmap)[state].default_tz;
			std::map<std::string,std::string>::iterator subdiv;
			subdiv = (*lmap)[state].subdiv_tz.begin();
			while (subdiv != (*lmap)[state].subdiv_tz.end())
			{
				if (hasLocation(subdiv->first, loc_name))
				{
					tz = subdiv->second;
					break;
				}
				subdiv++;
			}
			char *ds = const_cast<char*>(dest.c_str());
			char *ts = const_cast<char*>(tz.c_str());
			rmap::insert(ds, ts);
		}
	}
};

XCDR_NAMESPACE_END

#endif // XCDR_UTIL_RMAP_TZMAP_H
