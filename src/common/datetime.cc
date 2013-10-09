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
// $Source: /usr/local/cvs/xcdr/src/common/datetime.cc,v $
// $Revision: 1.6 $
// $Date: 2003/04/04 16:06:36 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: datetime.cc,v $
// Revision 1.6  2003/04/04 16:06:36  tmarsh
// namespace changes
//
// Revision 1.5  2003/04/02 20:35:51  tmarsh
// added XCDR namespacing; eradicated all using namespace std
//
// Revision 1.4  2003/03/09 08:26:12  tmarsh
// include file reorganization
//
// Revision 1.3  2003/03/06 11:20:02  tmarsh
// updated headers
//
//
// =========================================================================

#include "common/datetime.h"

#include <iomanip>

XCDR_NAMESPACE_BEGIN

DateTime::DateTime(const std::string& date, const std::string& time)
	throw(StringTokenizer::NoMoreTokensException)
{
	StringTokenizer st(date, "/");

	month = Util::atoi(st.getNext().c_str());
	day = Util::atoi(st.getNext().c_str());
	year = Util::atoi(st.getNext().c_str());

	StringTokenizer st2 = StringTokenizer(time, ":");

	hour = Util::atoi(st2.getNext().c_str());
	minute = Util::atoi(st2.getNext().c_str());
	sec = Util::atoi(st2.getNext().c_str());
}

void DateTime::toCurrentTime()
{
	struct tm* t;
	std::time_t secs;

	time(&secs);
			
	t = localtime(&secs);

	year = t->tm_year;
	month = t->tm_mon;
	day = t->tm_mday;
	hour = t->tm_hour;
	minute = t->tm_min;
	sec = t->tm_sec;
	
}


DateTime::DateTime(unsigned short y, unsigned short mo, unsigned short d,
		   unsigned short h, unsigned short mi, unsigned short s)
  : year(y), month(mo), day(d), hour(h), minute(mi), sec(s)
{
}

unsigned int DateTime::operator - (const DateTime& rhs)
{
	return ((asSeconds()) - (rhs.asSeconds()));
}

bool DateTime::operator>(const DateTime& rhs) const
{
	if (rhs.year > year) return false;
	if (rhs.year < year) return true;

	if (rhs.month > month) return false;
	if (rhs.month < month) return true;
	
	if (rhs.day > day) return false;
	if (rhs.day < day) return true;

	if (rhs.hour > hour) return false;
	if (rhs.hour < hour) return true;

	if (rhs.minute > minute) return false;
	if (rhs.minute < minute) return true;

	if (rhs.sec> sec) return false;
	if (rhs.sec< sec) return true;

	return false;
}

bool DateTime::operator<(const DateTime& rhs) const
{
	return (rhs > *this);
}

void DateTime::write(std::ostream& o) const
{
	o << std::setw(2) << std::setfill('0') << month << "/"
	  << std::setw(2) << std::setfill('0') << day << "/"
	  << year << " "
	  << std::setw(2) << std::setfill('0') << hour << ":"
	  << std::setw(2) << std::setfill('0') << minute << ":"
	  << std::setw(2) << std::setfill('0') << sec;
}

XCDR_NAMESPACE_END

std::ostream& operator << (std::ostream& o, const XCDR::DateTime& rhs)
{
	rhs.write(o);

	return o;
}

