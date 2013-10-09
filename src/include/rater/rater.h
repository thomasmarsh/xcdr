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
// $Source: /usr/local/cvs/xcdr/src/include/rater/rater.h,v $
// $Revision: 1.6 $
// $Date: 2003/05/05 17:20:31 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: rater.h,v $
// Revision 1.6  2003/05/05 17:20:31  tmarsh
// misc updates
//
// Revision 1.5  2003/04/04 16:03:58  tmarsh
// namespace changes
//
// Revision 1.4  2003/03/28 17:46:24  tmarsh
// major updates; rater now works
//
// Revision 1.3  2003/03/27 19:37:11  tmarsh
// formatting updates
//
// Revision 1.2  2003/03/27 19:15:22  tmarsh
// rater rewrite
//
//
// =========================================================================

#ifndef XCDR_RATER_RATER_H
#define XCDR_RATER_RATER_H

#include "rater/digit.h"
#include "rater/party.h"
#include "rater/query.h"
#include "rater/response.h"

XCDR_NAMESPACE_BEGIN

#ifdef CONSOLE_USAGE
struct ConsoleContext;
class SetContextCommand;
class ListCommand;
class LoadCommand;
class LookupCommand;
#endif



class Rater
{
public:
#ifdef CONSOLE_USAGE
	friend struct ConsoleContext;
	friend class SetContextCommand;
	friend class ListCommand;
	friend class LoadCommand;
	friend class LookupCommand;
#endif

	Rater();
	~Rater();

	void initChargeStack();
	void rate(Query *q, Response *r);
	void findBracket();
	void findParties();
	void findTariff();
	void findRate();
	void charge();
	void loadLookups();
	void populate();

private:
	// billing data
	BillingPartyList parties;

	// lookups
	DigitNode<char*> *timezone_map;
	DigitNode<char*> *country_map;
	DigitNode<char*> *province_map;
	DigitNode<char*> *city_map;
	DigitNode<char*> *lata_map;
	DigitNode<char*> *ocn_map;


	// rate stack
	Query		*query;
	Response	*response;
	BillingParty	*current_party;
	Tariff		*current_tariff;
	Rate		*current_rate;
	Bracket		*current_bracket;
//	ttree<char,TimeZone*> timezone_map;

	// charge stack
	Interval	*interval;
	IntervalList	*il_current;
	unsigned int	duration;
	double		current_charge;
	unsigned int	remainder;
	unsigned int	charged_time;
	unsigned int	idur;
public:
	inline std::size_t size()
	{
		std::size_t sz = sizeof(Rater);
		if (timezone_map) sz += timezone_map->size();
		if (country_map) sz += country_map->size();
		if (province_map) sz += province_map->size();
		if (city_map) sz += city_map->size();
		if (lata_map) sz += lata_map->size();
		if (ocn_map) sz += ocn_map->size();
		sz += parties.size();
		return sz;
	}
};

XCDR_NAMESPACE_END

#endif // XCDR_RATER_RATER_H
