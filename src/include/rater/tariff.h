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
// $Source: /usr/local/cvs/xcdr/src/include/rater/tariff.h,v $
// $Revision: 1.4 $
// $Date: 2003/04/04 16:03:58 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: tariff.h,v $
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

#ifndef XCDR_RATER_TARIFF_H
#define XCDR_RATER_TARIFF_H

#include "rater/ident.h"
#include "rater/period.h"
#include "rater/rate.h"

XCDR_NAMESPACE_BEGIN

struct Tariff
{
	unsigned int i_tariff;
	char *description;

	// constraints
	PeriodList periods;
	IdentifierList identifiers;
	RateList rates;


	Tariff()
	{
		i_tariff = 0;
		description = 0;
	}

	~Tariff()
	{
		if (description) delete description;
		periods.clear();
		identifiers.clear();
		rates.clear();
	}

	inline std::size_t count() const { return 1; }
	inline std::size_t deepCount() const
	{
		return 1+periods.count()+identifiers.count()+rates.count();
	}

	inline std::size_t size() const
	{
		std::size_t sz = sizeof(Tariff);
		if (description) sz += sizeof(char) * std::strlen(description);
		sz += periods.size();
		sz += identifiers.size();
		sz += rates.size();
		return sz;
	}
};

typedef slist<Tariff> TariffList;

XCDR_NAMESPACE_END

#endif // XCDR_RATER_TARIFF_H
