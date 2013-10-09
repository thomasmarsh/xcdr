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
// $Source: /usr/local/cvs/xcdr/src/include/common/datetime.h,v $
// $Revision: 1.2 $
// $Date: 2003/04/02 20:34:25 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: datetime.h,v $
// Revision 1.2  2003/04/02 20:34:25  tmarsh
// added XCDR namespace
//
// Revision 1.1  2003/03/09 08:35:31  tmarsh
// include file reorganization
//
// Revision 1.2  2003/03/06 11:20:02  tmarsh
// updated headers
//
//
// =========================================================================

#ifndef XCDR_DATETIME_H
#define XCDR_DATETIME_H

#include "common/common.h"
#include "common/strtok.h"
#include "common/util.h"

#include <time.h>

XCDR_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
// DateTime
// ---------------------------------------------------------------------------

class DateTime
{
public:
	DateTime() : year(0), month(0), day(0), hour(0), minute(0), sec(0) {}

	DateTime(const std::string& date, const std::string& time)
		throw (StringTokenizer::NoMoreTokensException);

	DateTime(unsigned short y, unsigned short mo, unsigned short d,
	         unsigned short h, unsigned short mi, unsigned short s);

	//DateTime(otl_datetime& datetime);

	void DateTime::toCurrentTime();
	
	unsigned int operator - (const DateTime& rhs);

	bool operator > (const DateTime& rhs) const;
	bool operator < (const DateTime& rhs) const;

	inline unsigned int asSeconds() const
	{
		struct tm t;

		t.tm_sec = sec;
		t.tm_min = minute;
		t.tm_hour = hour;
		t.tm_mday = day;
		t.tm_mon = month;
		t.tm_year = year;
					    
		return (unsigned int) mktime(&t);
	}

	inline std::string toString() const
	{
		std::string dt =
			Util::itoa(year) + "-" +
			Util::itoa(month) + "-" +
			Util::itoa(day) + " " +
			Util::itoa(hour) + ":" +
			Util::itoa(minute) + ":" +
			Util::itoa(sec);

		return dt;
	}

	void write(std::ostream& o) const;

	unsigned short year;
	unsigned short month;
	unsigned short day;

	unsigned short hour;
	unsigned short minute;
	unsigned short sec;

protected:
private:
};

//
// provide compare function to allow sorting by date
//


inline bool DateTimeCompare(DateTime& lhs, DateTime& rhs)
{
	// optimize to compare in following order:
	//
	// 	lyear < ryear
	// 	lmonth < rmonth
	// 	...

	 return (lhs.asSeconds() < rhs.asSeconds());
}

std::ostream& operator << (std::ostream& o, const DateTime& rhs);

XCDR_NAMESPACE_END

#endif // _COMMON_H_
