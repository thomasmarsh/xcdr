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
// $Source: /usr/local/cvs/xcdr/src/rater/populate.cc,v $
// $Revision: 1.5 $
// $Date: 2003/05/14 11:36:05 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: populate.cc,v $
// Revision 1.5  2003/05/14 11:36:05  tmarsh
// replacing DigitNode with dtrie
//
// Revision 1.4  2003/04/04 16:06:36  tmarsh
// namespace changes
//
// Revision 1.3  2003/03/28 17:46:33  tmarsh
// major updates; rater now works
//
// Revision 1.2  2003/03/27 19:16:45  tmarsh
// rater rewrite
//
//
// =========================================================================

#include "rater/rater.h"
#include "rdbms/rdbms.h"
#include "tok.h"

#include "common/logging.h"

XCDR_NAMESPACE_BEGIN

typedef unsigned int uint;

uint getIntervalDuration(uint i_interval, uint i_interval_index)
{
	ENTER();
	RDBMS::Query query(RDBMS::GetContext()->getConnection());

	query << "SELECT duration from RateInterval WHERE "
	      << "i_interval = " << i_interval << " AND "
	      << "i_interval_index = " << i_interval_index;

	RDBMS::ResultStore result = query.store();
	LEAVE();
	return result[0][0];
}

void loadInterval(Rate *rate, Bracket *bracket)
{
	ENTER();
	RDBMS::Query query(RDBMS::GetContext()->getConnection());

	query << "SELECT i_interval, i_interval_index, price "
	      << "FROM RateIntervalMap WHERE "
	      << "i_rate = " << rate->i_rate << " AND "
	      << "i_bracket = " << bracket->i_bracket
	      << " ORDER BY i_interval, i_interval_index";

	DEBUG("EXECUTE SQL: " << query.preview());

	Interval *interval = 0;
	RDBMS::ResultStore results = query.store();
	RDBMS::ResultStore::iterator ri = results.begin();
	while (ri != results.end())
	{
		interval = new Interval;
		interval->i_interval = (*ri)[0];
		interval->i_interval_index = (*ri)[1];
		interval->price = ((float) (*ri)[2]);
		interval->duration = getIntervalDuration(interval->i_interval,
							 interval->i_interval_index);
		DEBUG("New Interval:");
		DEBUG("\tinterval->i_interval = " << interval->i_interval);
		DEBUG("\tinterval->i_interval_index = " << interval->i_interval_index);
		DEBUG("\tinterval->duration = " << interval->duration);
		DEBUG("\tinterval->price = " << interval->price);
		bracket->intervals.push_back(interval);
		ri++;
	}
	LEAVE();
}

void loadBracket(Rate *rate)
{
	ENTER();
	RDBMS::Query query(RDBMS::GetContext()->getConnection());

	query << "SELECT i_bracket, i_bracket_index FROM RateBracketMap WHERE "
	      << "i_rate = " << rate->i_rate
	      << " ORDER BY i_bracket, i_bracket_index";

	DEBUG("EXECUTE SQL: " << query.preview());

	unsigned int i_bracket = 0, i_bracket_index = 0;
	RDBMS::ResultStore results = query.store();
	RDBMS::ResultStore::iterator ri = results.begin();
	while (ri != results.end())
	{
		i_bracket = (*ri)[0];
		i_bracket_index = (*ri)[1];

		DEBUG("Bracket indexes:");
		DEBUG("\ti_bracket = " << i_bracket);
		DEBUG("\ti_bracket_index = " << i_bracket_index);
		RDBMS::Query query_b(RDBMS::GetContext()->getConnection());
		query_b << "SELECT duration FROM Bracket WHERE "
		        << "i_bracket = " << i_bracket << " AND "
		        << "i_bracket_index = " << i_bracket_index;

		DEBUG("EXECUTE SQL: " << query_b.preview());
		Bracket *bracket = 0;
		RDBMS::ResultStore results_b = query_b.store();
		RDBMS::ResultStore::iterator rj = results_b.begin();
		while (rj != results_b.end())
		{
			bracket = new Bracket;
			bracket->i_bracket = i_bracket;
			bracket->i_bracket_index = i_bracket_index;
			bracket->duration = (*rj)[0];
			loadInterval(rate, bracket);
			DEBUG("New Bracket:");
			DEBUG("\tbracket->i_bracket = " << bracket->i_bracket);
			DEBUG("\tbracket->i_bracket_index = " << bracket->i_bracket_index);
			DEBUG("\tbracket->duration = " << bracket->duration);
			DEBUG("\tbracket->intervals.count() = " << bracket->intervals.count());
			rate->brackets.push_back(bracket);
			rj++;
		}
		ri++;
	}
	LEAVE();
}

void loadRateIdent(Rate *rate)
{
	ENTER();
	RDBMS::Query query(RDBMS::GetContext()->getConnection());

	query << "SELECT i_type, ident FROM RateIdentifier WHERE "
	      << "i_identifier = " << rate->i_identifier;

	DEBUG("EXECUTE SQL: " << query.preview());

	RDBMS::ResultStore results = query.store();
	// XXX: ASSERT SIZE!

	rate->identifier.i_type = ((Identifier::Type) ((int) results[0][0]));
	rate->identifier.value = strdup(results[0][1]);
	DEBUG("Rate Ident:");
	DEBUG("\trate->identifier.i_type = " << ((Identifier::Type) rate->identifier.i_type));
	DEBUG("\trate->identifier.value = " << rate->identifier.value);
	LEAVE();
}

void loadRates(Tariff *tariff)
{
	ENTER();
	RDBMS::Query query(RDBMS::GetContext()->getConnection());

	query << "SELECT i_rate, i_identifier FROM Rate WHERE "
	      << "i_tariff = " << tariff->i_tariff;

	DEBUG("EXECUTE SQL: " << query.preview());
	Rate *rate = 0;
	RDBMS::ResultStore results = query.store();
	RDBMS::ResultStore::iterator ri = results.begin();
	while (ri != results.end())
	{
		rate = new Rate;
		rate->i_rate = (*ri)[0];
		rate->i_identifier = (*ri)[1];
		loadRateIdent(rate);
		loadBracket(rate);
		DEBUG("New Rate:");
		DEBUG("\trate->i_rate = " << rate->i_rate);
		DEBUG("\trate->i_identifier = " << rate->i_identifier);
		DEBUG("\trate->identifier.i_type = " << rate->identifier.i_type);
		DEBUG("\trate->identifier.value = " << rate->identifier.value);
		DEBUG("\trate->brackets.count() = " << rate->brackets.count());
		tariff->rates.push_back(rate);
		ri++;
	}
	LEAVE();
}

void loadPeriods(Tariff *tariff)
{
	ENTER();
	RDBMS::Query query_a(RDBMS::GetContext()->getConnection());
	RDBMS::Query query_b(RDBMS::GetContext()->getConnection());

	query_a << "SELECT i_period FROM PeriodTariffMap "
		<< "WHERE i_tariff = " << tariff->i_tariff
		<< " ORDER BY priority";

	DEBUG("EXECUTE SQL: " << query_a.preview());
	uint i_period = 0;
	RDBMS::ResultStore results = query_a.store();
	RDBMS::ResultStore::iterator ri = results.begin();
	while (ri != results.end())
	{
		i_period = (*ri)[0];
		DEBUG("i_period = " << i_period);

		query_b << "SELECT period, description FROM Period WHERE "
			<< "i_period = " << i_period;

		DEBUG("EXECUTE SQL: " << query_b.preview());
		Period *period;
		RDBMS::ResultStore results_b = query_b.store();
		RDBMS::ResultStore::iterator rj = results_b.begin();
		while (rj != results_b.end())
		{
			period = new Period;
			period->i_period = i_period;
			period->period_string = strdup((*rj)[0]);
			period->description = strdup((*rj)[1]);
			tariff->periods.push_back(period);
			DEBUG("New Period:");
			DEBUG("\tperiod->i_period = " << period->i_period);
			DEBUG("\tperiod->period_string = " << period->period_string);
			DEBUG("\tperiod->description = " << period->description);
			rj++;
		}
		ri++;
	}
	LEAVE();
}

void loadTariffIdent(Tariff *tariff)
{
	ENTER();
	RDBMS::Query query(RDBMS::GetContext()->getConnection());
	
	query << "SELECT i_type, ident FROM TariffIdentifier WHERE "
	      << "i_tariff = " << tariff->i_tariff;

	DEBUG("EXECUTE SQL: " << query.preview());
	Identifier *ident = 0;
	RDBMS::ResultStore results = query.store();
	RDBMS::ResultStore::iterator ri = results.begin();
	while (ri != results.end())
	{
		ident = new Identifier;
		ident->i_type = ((Identifier::Type) ((int) (*ri)[0]));
		ident->value = strdup((*ri)[1]);
		tariff->identifiers.push_back(ident);
		DEBUG("New Identifier:");
		DEBUG("\tident->i_type = " << ident->i_type);
		DEBUG("\tident->value = " << ident->value);
		ri++;
	}
	LEAVE();
}

void loadTariffs(BillingParty *party)
{
	ENTER();
	RDBMS::Query query(RDBMS::GetContext()->getConnection());

	query << "SELECT i_tariff, description FROM Tariff WHERE "
	      << "i_party = " << party->i_party
	      << " ORDER BY priority";

	DEBUG("EXECUTE SQL: " << query.preview());
	Tariff *tariff;
	RDBMS::ResultStore results = query.store();
	RDBMS::ResultStore::iterator ri = results.begin();
	while (ri != results.end())
	{
		tariff = new Tariff;
		tariff->i_tariff = (*ri)[0];
		tariff->description = strdup((*ri)[1]);
		loadTariffIdent(tariff);
		loadPeriods(tariff);
		loadRates(tariff);
		DEBUG("New Tariff:");
		DEBUG("\ttariff->i_tariff = " << tariff->i_tariff);
		DEBUG("\ttariff->description = " << tariff->description);
		DEBUG("\ttariff->identifiers.count() = " << tariff->identifiers.count());
		DEBUG("\ttariff->periods.count() = " << tariff->periods.count());
		DEBUG("\ttariff->rates.count() = " << tariff->rates.count());
		party->tariffs.push_back(tariff);
		ri++;
	}
	LEAVE();
}

void loadPartyIdent(BillingParty *party)
{
	ENTER();
	RDBMS::Query query(RDBMS::GetContext()->getConnection());
	
	query << "SELECT i_type, ident FROM BillingPartyIdentifier WHERE "
	      << "i_party = " << party->i_party;

	DEBUG("EXECUTE SQL: " << query.preview());
	Identifier *ident;
	RDBMS::ResultStore results = query.store();
	RDBMS::ResultStore::iterator ri = results.begin();
	while (ri != results.end())
	{
		ident = new Identifier;
		ident->i_type = ((Identifier::Type) ((int) (*ri)[0]));
		ident->value = strdup((*ri)[1]);
		DEBUG("New Identifier:");
		DEBUG("\tident->i_type = " << ident->i_type);
		DEBUG("\tident->value = " << ident->value);
		party->identifiers.push_back(ident);
		ri++;
	}
	LEAVE();
}

void loadParties(BillingPartyList &l)
{
	ENTER();
	RDBMS::Query query(RDBMS::GetContext()->getConnection());
	
	query << "SELECT i_party, name FROM BillingParty";

	DEBUG("EXECUTE SQL: " << query.preview());
	BillingParty *party = 0;
	RDBMS::ResultStore results = query.store();
	RDBMS::ResultStore::iterator ri = results.begin();
	while (ri != results.end())
	{
		party = new BillingParty;
		party->i_party = (*ri)[0];
		party->name = strdup((*ri)[1]);
		loadPartyIdent(party);
		loadTariffs(party);
		DEBUG("New Party:");
		DEBUG("\tparty->i_party = " << party->i_party);
		DEBUG("\tparty->name = " << party->name);
		DEBUG("\tparty->identifiers.count() = " << party->identifiers.count());
		DEBUG("\tparty->tariffs.count() = " << party->tariffs.count());
		l.push_back(party);
		ri++;
	}
	LEAVE();
}

#include "tok.h"
#include <fstream>

void loadLookupTable(const std::string &table, const std::string &path, DigitNode<char*> *node)
{
	INFO("loading table: " << table);
	std::ifstream fp(path.c_str(), std::ifstream::in);
	char buf[256];
	while (!fp.eof())
	{
		fp.getline(buf, 256);
		Tokenizer t(buf, "\t");
		if (t.size() == 2)
		{
			node->insert(strdup(t[1].c_str()), strdup(t[0].c_str()));
		}
	}
	fp.close();
#if 0
	RDBMS::Query query(RDBMS::GetContext()->getConnection());
	
	query << "SELECT value, dest FROM " << table;

	DEBUG("EXECUTE SQL: " << query.preview());
	RDBMS::ResultStore results = query.store();
	RDBMS::ResultStore::iterator ri = results.begin();
	while (ri != results.end())
	{
		node->push_back(strdup((*ri)[1]), strdup((*ri)[0]));
		ri++;
	}
#endif
}

void Rater::loadLookups()
{
	loadLookupTable("TimezoneLookupMap", "data/timezone.txt", timezone_map);
	loadLookupTable("CountryLookupMap", "data/country.txt", country_map);
	loadLookupTable("ProvinceLookupMap", "data/province.txt", province_map);
//	loadLookupTable("CityLookupMap", "data/country.txt", city_map);
	loadLookupTable("LATALookupMap", "data/lata.txt", lata_map);
	loadLookupTable("OCNLookupMap", "data/ocn.txt", ocn_map);
}

void Rater::populate()
{
	ENTER();
	try
	{
		loadLookups();
		loadParties(parties);
	}
	catch (mysqlcppapi::ex_base &exc)
	{
		FATAL("TOPLEVEL MYSQL: " << exc.what());
		exit(1);
	}
	LEAVE();
}

XCDR_NAMESPACE_END
