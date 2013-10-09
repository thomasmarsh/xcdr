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
// $Source: /usr/local/cvs/xcdr/src/include/rater/rate.h,v $
// $Revision: 1.4 $
// $Date: 2003/04/04 16:03:58 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: rate.h,v $
// Revision 1.4  2003/04/04 16:03:58  tmarsh
// namespace changes
//
// Revision 1.3  2003/03/28 17:46:24  tmarsh
// major updates; rater now works
//
// Revision 1.2  2003/03/27 19:29:37  tmarsh
// fixed size reporting
//
// Revision 1.1  2003/03/27 19:15:37  tmarsh
// added
//
//
// =========================================================================

#ifndef XCDR_RATER_RATE_H
#define XCDR_RATER_RATE_H

#include "common/slist.h"
#include "rater/bracket.h"

XCDR_NAMESPACE_BEGIN

struct Rate
{
	unsigned int i_rate;
	unsigned int i_identifier;
	BracketList brackets;
	Identifier identifier;

	Rate()
	{
		i_rate = 0;
	}

	~Rate()
	{
		brackets.clear();
	}

	inline std::size_t count() const { return 1; }
	inline std::size_t deepCount() const { return 1+brackets.count(); }

	inline std::size_t size() const
	{
		std::size_t sz = sizeof(Rate);
		sz += brackets.size();

		// the identifier knows the size of it's contained string
		// so the Rater doesn't include it's size from the sizeof(Rate)
		sz += identifier.size() - sizeof(Identifier);
		return sz;
	}
};

typedef slist<Rate> RateList;

XCDR_NAMESPACE_END

#endif // XCDR_RATER_RATE_H
