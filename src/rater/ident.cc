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
// $Source: /usr/local/cvs/xcdr/src/rater/ident.cc,v $
// $Revision: 1.4 $
// $Date: 2003/04/04 16:06:36 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: ident.cc,v $
// Revision 1.4  2003/04/04 16:06:36  tmarsh
// namespace changes
//
// Revision 1.3  2003/03/28 17:46:45  tmarsh
// added
//
// Revision 1.2  2003/03/27 19:15:22  tmarsh
// rater rewrite
//
//
// =========================================================================

#include "rater/ident.h"
#include "common/logging.h"

#undef ENTER
#undef LEAVE
#undef DEBUG
#define ENTER()
#define LEAVE()
#define DEBUG(x)

XCDR_NAMESPACE_BEGIN

bool hasMatch(const IdentifierList &a, const IdentifierList &b)
{
	ENTER();
	std::size_t acnt = a.count();
	std::size_t bcnt = b.count();
	if ((acnt == 0) || (bcnt == 0))
	{
		LEAVE();
		return false;
	}

	if (bcnt < acnt)
	{
		DEBUG("recurse");
		return hasMatch(b, a);
	}
	
	Identifier *found = 0;
	IdentifierList::iterator i = a.begin();
	while (i != a.end())
	{
		found = b.find(*i);
		if (found)
		{
			LEAVE();
			return true;
		}
		i++;
	}
	LEAVE();
	return false;
}

XCDR_NAMESPACE_END
