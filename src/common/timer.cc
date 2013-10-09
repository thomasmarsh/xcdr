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
// $Source: /usr/local/cvs/xcdr/src/common/timer.cc,v $
// $Revision: 1.5 $
// $Date: 2003/04/04 16:06:36 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: timer.cc,v $
// Revision 1.5  2003/04/04 16:06:36  tmarsh
// namespace changes
//
// Revision 1.4  2003/04/02 20:35:51  tmarsh
// added XCDR namespacing; eradicated all using namespace std
//
// Revision 1.3  2003/03/09 08:26:12  tmarsh
// include file reorganization
//
// Revision 1.2  2003/03/06 11:20:02  tmarsh
// updated headers
//
//
// =========================================================================


#include "common/timer.h"
#include "common/util.h"

#include <iostream>

XCDR_NAMESPACE_BEGIN

Timer::Timer()
{
	start_time = time(NULL);
}


void Timer::reset()
{
	start_time = time(NULL);
}

time_t Timer::elapsed() const
{
	return (time(NULL) - start_time);
}


void doMagic(std::string  &str,
	     int          &cur,
	     int          &prev,
	     float        &fcur,
	     float        &fprev,
	     const float  factor,
	     const std::string &name,
	     const bool   last=false)
{
	fcur = (fprev - (float) prev) * factor;
	cur = (int) fcur;

	if (last)
	{
		int rounded = ((fcur - (float) cur) >= 0.5) ? (cur + 1) : cur;

		if (str.length() != 0)
			str += " ";

		str += Util::itoa(rounded) + name;
	}
	else
	{
		if (cur)
			str += Util::itoa(cur) + name;
	}

	fprev = fcur;
	prev = cur;
}


std::string Timer::elapsedString() const
{
	std::string str = "";

	float f_years  = (float) elapsed() / 32659200.0;
	int years = (int) f_years;

	if (years)
		str += years + "y";

	float fcur, fprev = f_years;
	int    cur,  prev = years;

	doMagic(str, cur, prev, fcur, fprev, 12.0, "m");
	doMagic(str, cur, prev, fcur, fprev, 4.5,  "w");
	doMagic(str, cur, prev, fcur, fprev, 7.0,  "d");
	doMagic(str, cur, prev, fcur, fprev, 24.0, "h");
	doMagic(str, cur, prev, fcur, fprev, 60.0, "m");
	doMagic(str, cur, prev, fcur, fprev, 60.0, "s", true);

	return str;
}

StartStopTimer::StartStopTimer()
{
	elapsed_time = 0;
}

void StartStopTimer::start()
{
	last_start = times(&times_arg);
}

void StartStopTimer::stop()
{
	clock_t stop_time = times(&times_arg);
	clock_t old_elapsed_time = elapsed_time;
	
	if (stop_time < last_start) // overflow
	{
		elapsed_time += ((LONG_MAX - last_start) + stop_time);
	}
	else
	{
		elapsed_time += (stop_time - last_start);
	}
	if (old_elapsed_time > elapsed_time)
	{
		std::cerr << "Timer overflow!!!" << std::endl;
		exit(0);
	}
}

clock_t StartStopTimer::elapsed()
{
	return elapsed_time;
}

XCDR_NAMESPACE_END
