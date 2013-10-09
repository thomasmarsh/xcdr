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
// $Source: /usr/local/cvs/xcdr/src/include/common/period.h,v $
// $Revision: 1.2 $
// $Date: 2003/04/02 20:34:10 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: period.h,v $
// Revision 1.2  2003/04/02 20:34:10  tmarsh
// added parser
//
// Revision 1.1  2003/03/10 14:45:00  tmarsh
// added
//
//
// =========================================================================

#ifndef XCDR_COMMON_PERIOD_H
#define XCDR_COMMON_PERIOD_H

#include "common/common.h"
#include "common/logging.h"
#include "common/util.h"
#include "common/slist.h"

#include <unicode/gregocal.h>
#include <unicode/timezone.h>

#include <iostream>

#include <string>
#include <list>
#include <map>

using namespace icu;

XCDR_NAMESPACE_BEGIN

class PeriodBuilder;

// ---------------------------------------------------------------------------
// PERIOD CLASS
// ---------------------------------------------------------------------------

class Period
{
public:
	// -------------------------------------------------------------------
	// PARSE EXCEPTION
	// -------------------------------------------------------------------

	class PeriodError : public Exception
	{
	public:
		PeriodError(const std::string &msg)
			: Exception(msg) {}
	};

	class ParseError : public PeriodError
	{
	public:
		ParseError(const std::string &src)
			: PeriodError(std::string("can't parse: ") + src) {}
	};

	class RangeError : public PeriodError
	{
	public:
		RangeError(const std::string &msg) : PeriodError(msg) {}
	};

	class ICUError : public PeriodError
	{
	public:
		ICUError(const std::string &msg) : PeriodError(msg) {}
	};

	class InternalError : public PeriodError
	{
	public:
		InternalError(const std::string &msg) : PeriodError(msg) {}
	};


	// -------------------------------------------------------------------
	// CTOR & DTOR
	// -------------------------------------------------------------------

	friend class PeriodBuilder;

	Period(const std::string &period_string) throw (PeriodError*);

	~Period();


	// -------------------------------------------------------------------
	// SCALE
	// -------------------------------------------------------------------

	enum Scale 
	{
		UNKNOWN=0, YEAR=1,
		MONTH=2,   WEEK=3,
		YDAY=4,    MDAY=5,
		WDAY=6,    HOUR=7,
		MINUTE=8,  SECOND=9
	};


	// -------------------------------------------------------------------
	// RANGE
	// -------------------------------------------------------------------

	struct Range
	{
		Scale scale;
		unsigned int from;
		unsigned int to;
	};


	// -------------------------------------------------------------------
	// PeriodContext
	// -------------------------------------------------------------------

	struct PeriodContext
	{
		GregorianCalendar *calendar;
		UErrorCode icu_status;

		std::string errorMessage()
		{
			return std::string(u_errorName(icu_status));
		}

		PeriodContext();
		~PeriodContext();
	};


	// -------------------------------------------------------------------
	// string representation
	// -------------------------------------------------------------------

	std::string toString();


	// -------------------------------------------------------------------
	// SIZE & COUNT METHODS
	// -------------------------------------------------------------------

	inline std::size_t size() const
	{
		return (sizeof(Period) - sizeof(slist<Range>) + subperiods.size());
	}

	inline std::size_t count() const { return 1; }

	inline std::size_t deepCount() const { return 1+subperiods.deepCount(); }


	bool inPeriod(TimeZone *tz, UDate date);

	static const unsigned int PERIOD_NULL = 1234567890;

private:
	bool parse();


	// -------------------------------------------------------------------
	// SUBPERIOD
	// -------------------------------------------------------------------

	class SubPeriod
	{
	public:
		friend class PeriodBuilder;

		SubPeriod(PeriodContext *ctx);
		~SubPeriod();

		bool inPeriod(TimeZone *tz, UDate date);

		void addRange(Range *r);

		std::string toString();

		inline std::size_t size() const
		{
			return 1;
		}

		inline std::size_t count() const { return 1; }

		inline std::size_t deepCount() const { return 1; }


	private:
		typedef std::map<Scale, std::list<Range> > Ranges;

		bool inRange(Range *r);
		bool rangeValid(Range *r);

		slist<Range> ranges[10];
		PeriodContext *context;
	};


	// -------------------------------------------------------------------
	// LIST OF SUBPERIODS
	// -------------------------------------------------------------------

	slist<SubPeriod> subperiods;

	PeriodContext context;
	std::string period_source;
};


XCDR_NAMESPACE_END

#endif // XCDR_COMMON_PERIOD_H
