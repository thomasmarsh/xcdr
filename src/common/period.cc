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
// $Source: /usr/local/cvs/xcdr/src/common/period.cc,v $
// $Revision: 1.3 $
// $Date: 2003/04/04 16:06:36 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: period.cc,v $
// Revision 1.3  2003/04/04 16:06:36  tmarsh
// namespace changes
//
// Revision 1.2  2003/04/02 20:35:27  tmarsh
// added parser; almost working
//
// Revision 1.1  2003/03/10 14:44:49  tmarsh
// added
//
// -------------------------------------------------------------------------
//
// NOTE: This file is dangerous to the reader. This was more or less an
// 	 experiment to learn the boost::spirit parser modules.
//
// =========================================================================

#include "common/period.h"
#include "common/logging.h"

#include "boost/spirit/core.hpp"
#include "boost/spirit/utility/chset.hpp"
#include "boost/spirit/utility/escape_char.hpp"
#include "boost/spirit/symbols/symbols.hpp"
#include "boost/spirit/utility/confix.hpp"

using namespace boost::spirit;

#include <vector>
#include <string>
#include <cstring>

XCDR_NAMESPACE_BEGIN


// -------------------------------------------------------------------------
//
// PeriodContext
//
// -------------------------------------------------------------------------

Period::PeriodContext::PeriodContext()
{
	ENTER();
	icu_status = U_ZERO_ERROR;

	calendar = new GregorianCalendar(icu_status);
	if (U_FAILURE(icu_status))
	{
		std::string msg = "Couldn't create Gregorian Calendar: "
			+ errorMessage();

		throw new ICUError(msg);
	}
	LEAVE();
}

Period::PeriodContext::~PeriodContext()
{
	delete calendar;
}


// -------------------------------------------------------------------------
//
// Period
//
// -------------------------------------------------------------------------

Period::Period(const std::string &_period_source)
	throw (Period::PeriodError*)
{
	ENTER();
	period_source = _period_source;
	bool sucess = parse();
	if (!sucess)
		throw new ParseError(period_source);

	LEAVE();
}

Period::~Period()
{
}


std::string Period::toString()
{
	ENTER();
	std::string period_str;

	slist<SubPeriod>::iterator i = subperiods.begin();

	while (i != subperiods.end())
	{
		SubPeriod *sub_p = *i;
		if (sub_p)
			period_str += sub_p->toString();
		i++;
		if (i != subperiods.end())
			period_str += ", ";
	}

	LEAVE();
	return period_str;
}

bool Period::inPeriod(TimeZone *tz, UDate date)
{
	ENTER();
	slist<SubPeriod>::iterator i = subperiods.begin();

	while (i != subperiods.end())
	{
		SubPeriod *sub_p = *i;
		if (sub_p->inPeriod(tz, date))
		{
			LEAVE();
			return true;
		}
		i++;
	}
	LEAVE();
	return false;
}


// -------------------------------------------------------------------------
//
// SubPeriod
//
// -------------------------------------------------------------------------

Period::SubPeriod::SubPeriod(Period::PeriodContext *ctx)
{
	ENTER();
	context = ctx;
	LEAVE();
}

Period::SubPeriod::~SubPeriod()
{
	ENTER();
	context = 0;
	LEAVE();
}


void Period::SubPeriod::addRange(Period::Range *r)
{
	ENTER();
	// assert validity of the range
	if (!rangeValid(r))
		throw new RangeError(
				std::string("invalid range: (") +
				Util::itoa(r->from) +
				std::string(", ") +
				Util::itoa(r->to) +
				std::string(")"));

	TRACE("RANGE VALIDATED: (" << r->from << ", " << r->to << ")");

	ranges[r->scale].push_back(r);
	LEAVE();
}

bool Period::SubPeriod::inPeriod(TimeZone *tz, UDate date)
{
	ENTER();
	//
	// set the timezone if provided
	//

	if (tz != NULL)
	{
		TODO("save original timezone");
		context->calendar->setTimeZone(*tz);
	}

	//
	// set the calendar time to the date provided
	//

	context->calendar->setTime(date, context->icu_status);

	if (U_FAILURE(context->icu_status))
	{
		std::string msg = "Couldn't set time: "
			+ context->errorMessage();
		throw new ICUError(msg);
	}

	// check the ranges

	for (unsigned int j = 0; j < 10; j++)
	{
		if (ranges[j].count() == 0)
			continue;

		bool found = false;
		slist<Range>::iterator i = ranges[j].begin();
		while (i != ranges[j].end())
		{
			if (inRange(*i))
			{
				found = true;
				break;
			}
			i++;
		}
		if (!found)
		{
			LEAVE();
			return false;
		}
	}
	LEAVE();
	return true;
}


// --------------------------------------------------------------------------

std::string Period::SubPeriod::toString()
{
	ENTER();
	std::string result = "";
	bool first = true;

	for (unsigned int i = 0; i < 10; i++)
	{
		if (ranges[i].count() == 0)
			continue;

		DEBUG("ranges[i].count() = " << ranges[i].count());

		if (first)
			first = false;
		else result += " ";

		switch ((Scale) i)
		{
		case YEAR:	result += "yr {"; break;
		case MONTH:	result += "mo {"; break;
		case WEEK:	result += "wk {"; break;
		case YDAY:	result += "yd {"; break;
		case MDAY:	result += "md {"; break;
		case WDAY:	result += "wd {"; break;
		case HOUR:	result += "hr {"; break;
		case MINUTE:	result += "min {"; break;
		case SECOND:	result += "sec {"; break;
		default:	result += "UNK {"; break;
		}

		slist<Range>::iterator j = ranges[i].begin();
		while (j != ranges[i].end())
		{
			Range *r = *j;
			if (!r) throw new InternalError("unexpected null");
			result += Util::itoa(r->from);
			if ((r->to != PERIOD_NULL) &&
			    (r->to != r->from))
			{
				result += "-";
				result += Util::itoa(r->to);
			}

			j++;
			if (j != ranges[i].end())
				result += " ";
		}

		result += "}";
	}
	LEAVE();
	return result;
}


// --------------------------------------------------------------------------

bool Period::SubPeriod::inRange(Period::Range *r)
{
	ENTER();
	unsigned int use_value = PERIOD_NULL;
	GregorianCalendar *cal = context->calendar;
	switch (r->scale)
	{
	case YEAR:
		use_value = cal->get(Calendar::YEAR, context->icu_status);
		break;
	case MONTH:
		use_value = cal->get(Calendar::MONTH, context->icu_status);
		break;
	case WEEK:
		use_value = cal->get(Calendar::WEEK_OF_MONTH, context->icu_status);
		break;
	case YDAY:
		use_value = cal->get(Calendar::DAY_OF_YEAR, context->icu_status);
		break;
	case MDAY:
		use_value = cal->get(Calendar::DAY_OF_MONTH, context->icu_status);
		break;
	case WDAY:
		use_value = cal->get(Calendar::DAY_OF_WEEK, context->icu_status);
		break;
	case HOUR:
		use_value = cal->get(Calendar::HOUR_OF_DAY, context->icu_status);
		break;
	case MINUTE:
		use_value = cal->get(Calendar::MINUTE, context->icu_status);
		break;
	case SECOND:
		use_value = cal->get(Calendar::SECOND, context->icu_status);
		break;
	default:
		FATAL("Unkown range scale: " << r->scale);
		throw new InternalError("unknown range scale");
		break;
	}

	if ((r->to == PERIOD_NULL) || (r->from == r->to))
	{
		if (use_value == r->from)
		{
			LEAVE();
			return true;
		}
	}
	else
	{
		if (r->from > r->to)
		{
			if ((r->from <= use_value) || (r->to >= use_value))
			{
				LEAVE();
				return true;
			}
		}
		else
		{
			if ((r->from <= use_value) && (use_value <= r->to))
			{
				LEAVE();
				return true;
			}
		}
	}
	LEAVE();
	return false;
}


// --------------------------------------------------------------------------

inline bool checkValue(Period::Scale scale, unsigned int value)
{
	ENTER();
	if (value > Period::PERIOD_NULL)
		return false;

	switch (scale)
	{
	case Period::YEAR:
		if ((value > 1969) || (value == Period::PERIOD_NULL))
			return true;
		break;
	case Period::MONTH:
		if (((value > 0) && (value < 13)) || (value == Period::PERIOD_NULL))
			return true;
		break;
	case Period::WEEK:
		if (((value > 0) && (value < 7)) || (value == Period::PERIOD_NULL))
			return true;
		break;
	case Period::YDAY:
		if (((value > 0) && (value < 366)) || (value == Period::PERIOD_NULL))
			return true;
		break;
	case Period::MDAY:
		if (((value > 0) && (value < 32)) || (value == Period::PERIOD_NULL))
			return true;
		break;
	case Period::WDAY:
		if (((value > 0) && (value < 8)) || (value == Period::PERIOD_NULL))
			return true;
		break;
	case Period::HOUR:
		if ((value < 24) || (value == Period::PERIOD_NULL))
			return true;
		break;
	case Period::MINUTE:
		if ((value < 60) || (value == Period::PERIOD_NULL))
			return true;
		break;
	case Period::SECOND:
		if ((value < 60) || (value == Period::PERIOD_NULL))
			return true;
		break;
	case Period::UNKNOWN:
	default:
		break;
	}
	return false;
}


// --------------------------------------------------------------------------

bool Period::SubPeriod::rangeValid(Period::Range *r)
{
	ENTER();
	if (r->from == PERIOD_NULL)
		return false;

	if (checkValue(r->scale, r->from) && checkValue(r->scale, r->to))
		return true;

	return false;
}



// ==========================================================================
//
// BEGIN PARSING
//
// ==========================================================================


// some utilities for the parser

#define STREQ(x,y) (strcmp(x,y)==0)
#define DSTREQ(x, a, b) ((strcmp(x, a) == 0) || (strcmp(x, b) == 0))

char *tolower(char const *_str)
{
	ENTER();
	char *str = strdup(_str);
	unsigned int len = strlen(str);
	for (unsigned int i = 0; i < len; i++)
	{
//		*(str+i) = (char)(tolower(*(const_cast<char*>(str)+i)));
	}
	LEAVE();
	return str;
}

// --------------------------------------------------------------------------
//
// PeriodBuilder
//
// --------------------------------------------------------------------------

class PeriodBuilder
{
public:
	PeriodBuilder();

	void setPeriod(Period *);
	void setScale(char const *str);
	void pushRange(char const *str);
	void pushSubPeriod();
	void clear();

private:
	enum State
	{
		SCALE=0, FROM=1, TO=2, NEXT=3
	};

	Period::Scale classify(char const *str);
	unsigned int decode(char const *str);

	Period *period;
	State expect;
	Period::SubPeriod *current_subperiod;
	Period::Scale current_scale;
	Period::Range *current_range;
};


// ============== //
// GLOBAL BUILDER //
// ============== //

PeriodBuilder BUILDER;


// --------------------------------------------------------------------------

PeriodBuilder::PeriodBuilder()
{
	ENTER();
	clear();
	LEAVE();
}

void PeriodBuilder::clear()
{
	ENTER();
	current_subperiod = 0;
	current_range = 0;
	expect = SCALE;
	LEAVE();
}

void PeriodBuilder::setPeriod(Period *p)
{
	ENTER();
	period = p;
	LEAVE();
}

// --------------------------------------------------------------------------


Period::Scale PeriodBuilder::classify(char const *_str)
{
	ENTER();
	char *str = tolower(_str);
	Period::Scale scale;

	if      (DSTREQ(str, "year",   "yr"))	scale = Period::YEAR;
	else if (DSTREQ(str, "month",  "mo"))	scale = Period::MONTH;
	else if (DSTREQ(str, "week",   "wk"))	scale = Period::WEEK;
	else if (DSTREQ(str, "yday",   "yd"))	scale = Period::YDAY;
	else if (DSTREQ(str, "mday",   "md"))	scale = Period::MDAY;
	else if (DSTREQ(str, "wday",   "wd"))	scale = Period::WDAY;
	else if (DSTREQ(str, "hour",   "hr"))	scale = Period::HOUR;
	else if (DSTREQ(str, "minute", "min"))  scale = Period::MINUTE;
	else if (DSTREQ(str, "second", "sec"))	scale = Period::SECOND;
	else
		throw new Period::InternalError("unrecognized scale classifier");

	delete str;

	LEAVE();
	return scale;
}


// --------------------------------------------------------------------------

void PeriodBuilder::setScale(char const *str)
{
	ENTER();
	if ((expect != SCALE) && (expect != NEXT))
		throw new Period::InternalError("bad internal builder state");


	if (!current_subperiod)
		current_subperiod = new Period::SubPeriod(&(period->context));

	current_range = new Period::Range;
	current_range->scale = classify(str);
	current_range->from = Period::PERIOD_NULL;
	current_range->to = Period::PERIOD_NULL;
	current_scale = current_range->scale;
	expect = FROM;
	LEAVE();
}


// --------------------------------------------------------------------------

void PeriodBuilder::pushRange(char const *str)
{
	ENTER();

	if (expect == NEXT)
	{
		current_range = new Period::Range;
		current_range->scale = current_scale;
		expect = FROM;
	}

	if ((expect != FROM) && (expect != TO))
		throw new Period::InternalError("bad internal builder state");

	switch (expect)
	{
	case FROM:
		current_range->from = decode(str);
		expect = TO;
		break;
	case TO:
		current_range->to = decode(str);
		current_subperiod->addRange(current_range);
		current_range = 0;
		expect = NEXT;
		break;
	
	// prevent compiler warnings
	case SCALE:
	case NEXT:
	default:
		// not reached
		throw new Period::InternalError("pushRange: not reached was reached");
		break;
	}
	LEAVE();
}


// --------------------------------------------------------------------------

unsigned int decodeInt(char const *str);
unsigned int decodeMonth(char const *str);
unsigned int decodeDay(char const *str);
unsigned int decodeHour(char const *str);

unsigned int PeriodBuilder::decode(char const *str)
{
	ENTER();
	if ((expect != FROM) && (expect != TO))
		throw new Period::InternalError("bad internal builder state");

	switch (current_range->scale)
	{
	case Period::YEAR:   return decodeInt(str);
	case Period::MONTH:  return decodeMonth(str);
	case Period::WEEK:   return decodeInt(str);
	case Period::YDAY:   return decodeInt(str);
	case Period::MDAY:   return decodeInt(str);
	case Period::WDAY:   return decodeDay(str);
	case Period::HOUR:   return decodeHour(str);
	case Period::MINUTE: return decodeInt(str);
	case Period::SECOND: return decodeInt(str);

	case Period::UNKNOWN:
	default:
		throw new Period::InternalError("decode: not reached was reached");
		break;
	}
	LEAVE();
}


// --------------------------------------------------------------------------

void PeriodBuilder::pushSubPeriod()
{
	ENTER();
	if (current_subperiod)
		period->subperiods.push_back(current_subperiod);
	else
		// reached?
		throw new Period::InternalError("pushSubPeriod: not reached was reached");

	current_subperiod = 0;
	LEAVE();
}

// --------------------------------------------------------------------------

unsigned int decodeHour(char const *_str)
{
	ENTER();
	char *str = tolower(_str);
	int result = -1;

	if      (DSTREQ(str, "1am", "1"))	result = 1;
	else if (DSTREQ(str, "2am", "2"))	result = 2;
	else if (DSTREQ(str, "3am", "3"))	result = 3;
	else if (DSTREQ(str, "4am", "4"))	result = 4;
	else if (DSTREQ(str, "5am", "5"))	result = 5;
	else if (DSTREQ(str, "6am", "6"))	result = 6;
	else if (DSTREQ(str, "7am", "7"))	result = 7;
	else if (DSTREQ(str, "8am", "8"))	result = 8;
	else if (DSTREQ(str, "9am", "9"))	result = 9;
	else if (DSTREQ(str, "10am", "10"))	result = 10;
	else if (DSTREQ(str, "11am", "11"))	result = 11;
	else if (DSTREQ(str, "12noon", "12"))	result = 12;
	else if (DSTREQ(str, "1pm", "13"))	result = 13;
	else if (DSTREQ(str, "2pm", "14"))	result = 14;
	else if (DSTREQ(str, "3pm", "15"))	result = 15;
	else if (DSTREQ(str, "4pm", "16"))	result = 16;
	else if (DSTREQ(str, "5pm", "17"))	result = 17;
	else if (DSTREQ(str, "6pm", "18"))	result = 18;
	else if (DSTREQ(str, "7pm", "19"))	result = 19;
	else if (DSTREQ(str, "8pm", "20"))	result = 20;
	else if (DSTREQ(str, "9pm", "21"))	result = 21;
	else if (DSTREQ(str, "10pm", "22"))	result = 22;
	else if (DSTREQ(str, "11pm", "23"))	result = 23;
	else if (DSTREQ(str, "12pm", "0"))	result = 0;
	else
		throw new Period::InternalError("bad hour format");

	delete str;
	LEAVE();
	return result;
}


// --------------------------------------------------------------------------

unsigned int decodeMonth(char const *_str)
{
	ENTER();
	char *str = tolower(_str);
	int result = -1;

	if      (DSTREQ(str, "jan", "1"))	result = 1;
	else if (DSTREQ(str, "feb", "2"))	result = 2;
	else if (DSTREQ(str, "mar", "3"))	result = 3;
	else if (DSTREQ(str, "apr", "4"))	result = 4;
	else if (DSTREQ(str, "may", "5"))	result = 5;
	else if (DSTREQ(str, "jun", "6"))	result = 6;
	else if (DSTREQ(str, "jul", "7"))	result = 7;
	else if (DSTREQ(str, "aug", "8"))	result = 8;
	else if (DSTREQ(str, "sep", "9"))	result = 9;
	else if (DSTREQ(str, "oct", "10"))	result = 10;
	else if (DSTREQ(str, "nov", "11"))	result = 11;
	else if (DSTREQ(str, "dec", "12"))	result = 12;
	else
		throw new Period::InternalError("bad month format");

	delete str;
	LEAVE();
	return result;
}


// --------------------------------------------------------------------------

unsigned int decodeDay(char const *_str)
{
	ENTER();
	char *str = tolower(_str);
	int result = -1;

	if      (DSTREQ(str, "su", "1"))	result = 1;
	else if (DSTREQ(str, "mo", "2"))	result = 2;
	else if (DSTREQ(str, "tu", "3"))	result = 3;
	else if (DSTREQ(str, "we", "4"))	result = 4;
	else if (DSTREQ(str, "th", "5"))	result = 5;
	else if (DSTREQ(str, "fr", "6"))	result = 6;
	else if (DSTREQ(str, "sa", "7"))	result = 7;
	else
		throw new Period::InternalError("bad day format");

	delete str;
	LEAVE();
	return result;
}

// --------------------------------------------------------------------------

unsigned int decodeInt(char const *_str)
{
	ENTER();
	LEAVE();
	return Util::atoi(_str);
}


// --------------------------------------------------------------------------

void handle_scale(char const *first, char const *last)
{
	std::string str(first, last);
	DEBUG("SCALE: " << str);
	char const *ch = str.c_str();
	BUILDER.setScale(ch);
}

void handle_day(char const *first, char const *last)
{
	std::string str(first, last);
	DEBUG("DAY: " << str);
	char const *ch = str.c_str();
	BUILDER.pushRange(ch);
}

void handle_month(char const *first, char const *last)
{
	std::string str(first, last);
	DEBUG("MONTH: " << str);
	char const *ch = str.c_str();
	BUILDER.pushRange(ch);
}

void handle_hour(char const *first, char const *last)
{
	std::string str(first, last);
	DEBUG("HOUR: " << str);
	char const *ch = str.c_str();
	BUILDER.pushRange(ch);
}

void handle_int(char const *first, char const *last)
{
	std::string str(first, last);
	DEBUG("INTEGER: " << str);
	char const *ch = str.c_str();
	BUILDER.pushRange(ch);
}

void handle_push(char const *, char const *)
{
	DEBUG("---");
	BUILDER.pushSubPeriod();
}

void handle_endrg(char const*, char const *)
{
	DEBUG("END RANGE");
}

// --------------------------------------------------------------------------

struct PeriodParser : public grammar<PeriodParser>
{
	template <typename ScannerT>
	struct definition
	{

		definition(PeriodParser const &self)
		{
			// -----------------------------------------------------------
			// OPERATORS
			// -----------------------------------------------------------

			chlit<>		COMMA(',');
			chlit<>		LBRACK('{');
			chlit<>		RBRACK('}');
			chlit<>		MINUS('-');


			// -----------------------------------------------------------
			// TOKENS
			// -----------------------------------------------------------

			typedef inhibit_case< strlit<> > token_t;

			token_t YEAR	= as_lower_d["year"];
			token_t YR	= as_lower_d["yr"];
			token_t MONTH	= as_lower_d["month"];
			token_t MO	= as_lower_d["mo"];
			token_t WEEK	= as_lower_d["week"];
			token_t WK	= as_lower_d["wk"];
			token_t YDAY	= as_lower_d["yday"];
			token_t YD	= as_lower_d["yd"];
			token_t MDAY	= as_lower_d["mday"];
			token_t MD	= as_lower_d["md"];
			token_t WDAY	= as_lower_d["wday"];
			token_t WD	= as_lower_d["wd"];
			token_t HOUR	= as_lower_d["hour"];
			token_t HR	= as_lower_d["hr"];
			token_t MINUTE	= as_lower_d["minute"];
			token_t MIN	= as_lower_d["min"];
			token_t SECOND	= as_lower_d["second"];
			token_t SEC	= as_lower_d["sec"];

			token_t AM	= as_lower_d["am"];
			token_t PM	= as_lower_d["pm"];
			token_t NOON	= as_lower_d["noon"];

			token_t JAN	= as_lower_d["jan"];
			token_t FEB	= as_lower_d["feb"];
			token_t MAR	= as_lower_d["mar"];
			token_t APR	= as_lower_d["apr"];
			token_t MAY	= as_lower_d["may"];
			token_t JUN	= as_lower_d["jun"];
			token_t JUL	= as_lower_d["jul"];
			token_t AUG	= as_lower_d["aug"];
			token_t SEP	= as_lower_d["sep"];
			token_t OCT	= as_lower_d["oct"];
			token_t NOV	= as_lower_d["nov"];
			token_t DEC	= as_lower_d["dec"];

			token_t SU	= as_lower_d["su"]; // tok MO is defined
			token_t TU	= as_lower_d["tu"];
			token_t WE	= as_lower_d["we"];
			token_t TH	= as_lower_d["th"];
			token_t FR	= as_lower_d["fr"];
			token_t SA	= as_lower_d["sa"];


			// -----------------------------------------------------------
			// BEGIN GRAMMAR
			// -----------------------------------------------------------

			period
			  =	subperiod
			  >>	*( (COMMA
				>> SPACES)[&handle_push]
				>> subperiod
				)
			  >>	SPACES
			  ;

			subperiod
			  =	*( scale
			  	>> SPACES
			  	>> LBRACK
			  	>> range_list
			  	>> RBRACK
			  	>> SPACES
				)
			  ;

			scale
			  =	( YEAR   | YR
				| MONTH  | MO
				| WEEK   | WK
				| YDAY   | YD
				| MDAY   | MD
				| WDAY   | WD
				| HOUR   | HR
				| MINUTE | MIN
				| SECOND | SEC
				)			[&handle_scale]
			  >>	SPACES
			  ;

			range_list
			  =	*( range[&handle_endrg] )
			  ;

			range
			  =	( (range_tok >> MINUS >> range_tok)
				| range_tok
				)
			  >>	SPACES
			  ;

			range_tok
			  =	( hour_tok		[&handle_hour]
				| day_tok
				| month_tok
				| integer		[&handle_int]
				)
			  ;
			integer = uint_p;

			hour_tok
			  =	uint_p
			  >>	( AM | PM | NOON )
			  ; 

			day_tok
			  =	( SU | MO | TU | WE | TH | FR | SA )[&handle_day]
			  >>	lexeme_d[*alpha_p]
			  ;

			month_tok
			  =	month_head[&handle_month]
//			  >>	month_tail
			  ;

			month_head
			  =	( JAN | FEB | MAR | APR | MAY | JUN
				| JUL | AUG | SEP | OCT | NOV | DEC
				)
			  ;
//			month_tail
//			  =	lexeme_d[*as_lower_d[alpha_p]]
//			  ;

			SPACES
			  =	*space_p
			  ;
			
			ALPHA
			  =	as_lower_d[alpha_p]
			  ;
				      
			// -----------------------------------------------------------
			// END GRAMMAR
			// -----------------------------------------------------------

		}

		rule<ScannerT>	period, subperiod, scale, range_list,
				range, range_tok, hour_tok, day_tok,
				month_tok, month_head, month_tail,
				integer, SPACES, ALPHA;

		rule<ScannerT> const & start() const { return period; }
	};
};

// --------------------------------------------------------------------------

bool Period::parse()
{
	BUILDER.clear();
	BUILDER.setPeriod(this);

	PeriodParser parser;
	parse_info<char const*> info =
		boost::spirit::parse(period_source.c_str(), parser); //, space_p);

	if (!info.full)
	{
		std::string here("\t");
		const char *cstr = period_source.c_str();
		for (int i=0; i < (info.stop-cstr); i++)
			here += " ";
		here += "^--Here";
		DEBUG("Parse error in expr:");
		DEBUG("\t" << period_source);
		DEBUG(here);
		return false;
	}
	BUILDER.pushSubPeriod();
	return true;
}


XCDR_NAMESPACE_END
