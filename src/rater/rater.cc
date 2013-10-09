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
// $Source: /usr/local/cvs/xcdr/src/rater/rater.cc,v $
// $Revision: 1.4 $
// $Date: 2003/05/14 11:36:05 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: rater.cc,v $
// Revision 1.4  2003/05/14 11:36:05  tmarsh
// replacing DigitNode with dtrie
//
// Revision 1.3  2003/04/04 16:06:36  tmarsh
// namespace changes
//
// Revision 1.2  2003/03/28 17:46:33  tmarsh
// major updates; rater now works
//
// Revision 1.1  2003/03/27 19:16:22  tmarsh
// added
//
//
// =========================================================================

#include "rater/rater.h"
#include "rater/except.h"

#include "common/logging.h"

#undef ENTER
#undef LEAVE
#undef DEBUG
#define ENTER()
#define LEAVE()
#define DEBUG(x)

XCDR_NAMESPACE_BEGIN

Rater::Rater()
{
	ENTER();
	timezone_map = new DigitNode<char*>;
	country_map = new DigitNode<char*>;
	province_map = new DigitNode<char*>;
	city_map = new DigitNode<char*>;
	lata_map = new DigitNode<char*>;
	ocn_map = new DigitNode<char*>;
	LEAVE();
}

Rater::~Rater()
{
	delete timezone_map;
	delete country_map;
	delete province_map;
	delete city_map;
	delete lata_map;
	delete ocn_map;
}

void Rater::initChargeStack()
{
	ENTER();
	interval = il_current->value;
	assert(interval!= 0);
	duration = query->duration;
	current_charge = 0.0;
	remainder = 0;
	charged_time = 0;
	idur = 0;
	LEAVE();
}

inline char *lookup(DigitNode<char*> *node, char *dest)
{
	if (!dest) return strdup("");
	char *l = (*node)[dest];
	if (!l) return strdup("");
	return strdup(l);
}

void Rater::rate(Query *q, Response *r)
{
	ENTER();
	query = q;
	response = r;

	findParties();
	findTariff();

	Identifier *ident;
	ident = new Identifier;
	ident->i_type = Identifier::TERM_LATA;
	ident->value = lookup(lata_map, query->cld);
	query->identifiers.push_back(ident);

	if (strcmp(ident->value, "") == 0)
	{
		throw new RatingFailure("LATANotFound", query, response);
//		ERROR("no lata");
//		return;
	}

	findRate();
	findBracket();

	// charge the call according to the interval list provided
	initChargeStack();
	charge();
	LEAVE();
}

void Rater::findRate()
{
	ENTER();
	if (!current_tariff->rates.count())
	{
//		ERROR("No Rates");
		throw new RatingFailure("NoRates", query, response);
	}
	bool found = false;
	current_rate = 0;
	RateList::iterator ri = current_tariff->rates.begin();
	while (ri != current_tariff->rates.end())
	{
		current_rate = *ri;
		if (query->identifiers.find(&(current_rate->identifier)) != 0)
		{
			found = true;
			break;
		}
		ri++;
	}
	if (!found)
	{
//		ERROR("Rate not found");
		throw new RatingFailure("RateNotFound", query, response);
	}
	response->i_rate = current_rate->i_rate;
	LEAVE();
}

void Rater::findBracket()
{
	ENTER();
	Bracket *last = 0;
	current_bracket = 0;
	unsigned int accum = 0;
	BracketList::iterator bi = current_rate->brackets.begin();
	while (bi != current_rate->brackets.end())
	{
		current_bracket = *bi;
		accum += current_bracket->duration;
		if (accum > query->duration)
		{
			if (!last) last = current_bracket;
			current_bracket = last;
			break;
		}
		last = current_bracket;
		bi++;
	}
	il_current = &(current_bracket->intervals);
	LEAVE();
}

void Rater::findParties()
{
	ENTER();
	bool found = false;

	// find the billing parties
	current_party = 0;
	BillingPartyList::iterator bi = parties.begin();
	while (bi != parties.end())
	{
		current_party = *bi;
		if (hasMatch(query->identifiers, current_party->identifiers))
		{
			found = true;
			break;
		}
		bi++;
	}
	if (!found)
	{
//		ERROR("Party not found");
		throw new RatingFailure("PartyNotFound", query, response);
	}
	response->i_customer = current_party->i_party;
	LEAVE();
}

void Rater::findTariff()
{
	ENTER();
	bool found = false;
	current_tariff = 0;
	TariffList::iterator ti = current_party->tariffs.begin();
	PeriodList::iterator pi;
	Period *current_period = 0;
	while (ti != current_party->tariffs.end())
	{
		current_tariff = *ti;
		if (!current_tariff->periods.count())
		{
//			ERROR("No Periods");
			throw new RatingFailure("NoPeriods", query, response);
		}
		pi = current_tariff->periods.begin();
		while (pi != current_tariff->periods.end())
		{
			current_period = *pi;
			if (current_period->inPeriod())
			{
				DEBUG("response = " << response);
				DEBUG("set response->i_period");
				response->i_period = current_period->i_period;
				if (hasMatch(query->identifiers,
					     current_tariff->identifiers))
				{
					found = true;
					break;
				}
			}
			pi++;
		}
		if (found) break;
		ti++;
	}
	if (!found)
	{
//		ERROR("Tariff not found");
		throw new RatingFailure("TariffNotFound", query, response);
	}
	DEBUG("set response->i_tariff");
	response->i_tariff = current_tariff->i_tariff;
	LEAVE();
}

XCDR_NAMESPACE_END
