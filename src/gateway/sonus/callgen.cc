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
// $Source: /usr/local/cvs/xcdr/src/gateway/sonus/callgen.cc,v $
// $Revision: 1.9 $
// $Date: 2003/04/04 16:01:53 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: callgen.cc,v $
// Revision 1.9  2003/04/04 16:01:53  tmarsh
// namespace changes
//
// Revision 1.8  2003/03/09 08:50:05  tmarsh
// include file reorganization
//
// Revision 1.7  2003/03/06 11:20:03  tmarsh
// updated headers
//
//
// =========================================================================

#include "common/util.h"

#include <string>

#include <time.h>
#include <unistd.h>

using namespace std;

XCDR_NAMESPACE_BEGIN

const unsigned int SHELF_INDEX = 0x0001;
unsigned int	   BOOT_COUNT  = 0x0001;
unsigned int       CALL_ID     = 0x00000001;
const char*	   TIME_ZONE   = "GMT-05:00-Eastern";
unsigned int	   ROLLOVER    = 0xfffffffe;

const char* version="Sonus Networks, Inc.0000000004600000000000000000000060000000000000000000000000000000000000000000ACT2001020121581300000000000000";
const char* reboot="REBOOT, None, 0x0001080D00000001, GMT, 02/01/2001, 21:58:14.2";
const char* gateway_name = "PHAETON";


//
// CLOCK
//

unsigned int CLOCK;

void initClock()
{
	// set the clock back a bit

	time_t secs;

	time(&secs);

	CLOCK = secs - (60 * 60 * 24 * 7 * 5 * 10 * 4);
}

class SonusDate
{
public:
	SonusDate()
	{
	}
	string dateString()
	{
		string s;
		s += Util::itoa(month, "%02d");
		s += "/";
		s += Util::itoa(day, "%02d");
		s += "/";
		s += Util::itoa(year, "%4d");
		return s;
	}
protected:
	unsigned int month, day, year;
};

class SonusTime
{
public:
	SonusTime()
	{
	}
	string timeString()
	{
		string s;
		s += Util::itoa(hour, "%02d");
		s += ":";
		s += Util::itoa(minute, "%02d");
		s += ":";
		s += Util::itoa(second, "%02d");
		s += ".";
		s += Util::itoa(random() % 10);
		return s;
	}
protected:
	unsigned int hour, minute, second;
};

class SonusDateTime : public SonusDate, public SonusTime
{
public:
	SonusDateTime()
	{
		struct tm *t;
		time_t secs = CLOCK;
		clock = CLOCK;

		t = localtime(&secs);

		year = t->tm_year + 1900;
		month = t->tm_mon;
		day = t->tm_mday;
		hour = t->tm_hour;
		minute = t->tm_min;
		second = t->tm_sec;
	}

	unsigned int operator - (const SonusDateTime &rhs)
	{
		return clock - rhs.clock;
	}

	unsigned int getClock() { return clock; }
private:
	unsigned int clock;
};



string gen_num()
{
	string cli("1");
	int i = 0;
	while (i < 10)
	{
		cli += Util::itoa(random() % 10);
		i++;
	}
	return cli;
}

class Call
{
public:
	Call()
	{
		cli = gen_num();
		cld = gen_num();
		call_id = CALL_ID;
		finish_time = CLOCK+(random() % 180);
	}

	bool finished()
	{
		if (CLOCK >= finish_time) return true;

		return false;
	}

	void writeStart(ostream &o)
	{
		string sep = ",";
		o << "START" << sep				// 1
		  << gateway_name << sep			// 2
		  << "0x" << Util::itox(SHELF_INDEX, 4)
		          << Util::itox(BOOT_COUNT,  4)
			  << Util::itox(call_id,     8) << sep  // 3
		  << start_time.getClock() << sep		// 4
		  << TIME_ZONE << sep				// 5
		  << start_time.dateString() << sep		// 6
		  << start_time.timeString() << sep		// 7
		  << random() % 100 << sep			// 8 
		  << random() % 100 << sep			// 9
		  << random() % 100 << sep 			// 10
		  << "VoIP" << sep				// 11
		  << "IP-TO-PSTN" << sep			// 12
		  << "DEFAULT" << sep				// 13
		  << "0288" << sep				// 14
		  << cli << sep					// 15
		  << cld << sep					// 16
		  << sep					// 17
		  << 0 << sep					// 18
		  << sep					// 19
		  << 0 << sep					// 20
		  << sep					// 21
		  << 0 << sep					// 22
		  << sep					// 23
		  << "ROUTE01" << sep				// 24
		  << 1 << sep					// 25
		  << "10.99.81.1:TG-A7N4-ATT1" << sep		// 26
		  << "10.99.81.1" << sep			// 27
		  << "10.99.84.1" << sep			// 28
		  << " " << sep					// 29
		  << " " << sep					// 30
		  << "10.7.2.5:5002/10.7.2.6:5002" << sep	// 31
		  << "1:7:2:1:11025:0x00040404:0x00030303" << sep //32
		  << " " << sep					// 33
		  << 0 << sep					// 34
		  << sep					// 35
		  << sep					// 36
		  << "0x00000002" << sep			// 37
		  << " " << sep					// 38
		  << " " << sep					// 39
		  << 2 << sep					// 40
		  << 2 << sep					// 41
		  << "\"SIP,917506_105429@10.98.84.1,  "
		     << "<sip:" << cli << "@10.98.84.1>,  "
		     << "<sip:" << cld << "@10.98.81.1>\"" << sep // 42
		  << 12 << sep					// 43
		  << 1 << sep					// 44
		  << 0 << sep					// 45
		  << 5 << sep					// 46
		  << " " << sep					// 47
		  << " " << sep					// 48
		  << "0x0a" << sep				// 49
		  << cld << sep					// 50
		  << 1 << sep					// 51
		  << 1 << sep					// 52
		  << " " << sep					// 53
		  << "TG-A7N4-ATT1" << sep			// 54
		  << ""						// 55
		  << endl << flush;
	}

	void writeStop(ostream &o)
	{
		SonusDateTime stop_time;
		string sep = ",";
		o << "STOP" << sep				// 1
		  << gateway_name << sep			// 2
		  << "0x" << Util::itox(SHELF_INDEX, 4)
		          << Util::itox(BOOT_COUNT,  4)
			  << Util::itox(call_id,     8) << sep  // 3
		  << start_time.getClock() << sep		// 4
		  << TIME_ZONE << sep				// 5
		  << start_time.dateString() << sep		// 6
		  << start_time.timeString() << sep		// 7
		  << random() % 100 << sep			// 8 
		  << random() % 100 << sep			// 9
		  << random() % 100 << sep			// 10
		  << stop_time.dateString() << sep		// 11
		  << stop_time.timeString() << sep		// 12
		  << random() % 100 << sep			// 13
		  << stop_time - start_time << sep		// 14
		  << (random() % 254) + 1 << sep		// 15
		  << "VoIP" << sep				// 16
		  << "IP-TO-PSTN" << sep			// 17
		  << "DEFAULT" << sep				// 18
		  << "0288" << sep				// 19
		  << cli << sep					// 20
		  << cld << sep					// 21
		  << sep					// 22
		  << 0 << sep					// 23
		  << sep					// 24
		  << 0 << sep					// 25
		  << sep					// 26
		  << 0 << sep					// 27
		  << sep					// 28
		  << "ROUTE01" << sep				// 29
		  << 1 << sep					// 30
		  << "10.99.81.1:TG-A7N4-ATT1" << sep		// 31
		  << "10.99.81.1" << sep			// 32
		  << "10.99.84.1" << sep			// 33
		  << " " << sep					// 34
		  << " " << sep					// 35
		  << "10.7.2.5:5002/10.7.2.6:5002" << sep	// 36
		  << "1:7:2:1:11025:0x00040404:0x00030303" << sep //37
		  << random() << sep				// 38
		  << random() << sep				// 39
		  << random() << sep				// 40
		  << random() << sep				// 41
		  << " " << sep					// 42
		  << 0 << sep					// 43
		  << sep					// 44
		  << sep					// 45
		  << "0x00000002" << sep			// 46
		  << " " << sep					// 47
		  << " " << sep					// 48
		  << 2 << sep					// 49
		  << 2 << sep					// 50
		  << "\"SIP,917506_105429@10.98.84.1,  "
		     << "<sip:" << cli << "@10.98.84.1>,  "
		     << "<sip:" << cld << "@10.98.81.1>\"" << sep // 51
		  << 12 << sep					// 52
		  << 1 << sep					// 53
		  << 0 << sep					// 55
		  << 5 << sep					// 56
		  << " " << sep					// 57
		  << " " << sep					// 58
		  << "0x0a" << sep				// 59
		  << cld << sep					// 61
		  << 1 << sep					// 62
		  << 1 << sep					// 63
		  << " " << sep					// 64
		  << 1 << sep					// 65
		  << 0 << sep					// 66
		  << 0 << sep					// 67
		  << 0 << sep					// 68
		  << "TG-A7N4-ATT1" << sep			// 69
		  << ""						// 70
		  << endl << flush;
	}

	void writeAttempt(ostream &o)
	{
		SonusDateTime stop_time;
		string sep = ",";
		o << "ATTEMPT" << sep				// 1
		  << gateway_name << sep			// 2
		  << "0x" << Util::itox(SHELF_INDEX, 4)
		          << Util::itox(BOOT_COUNT,  4)
			  << Util::itox(call_id,     8) << sep  // 3
		  << start_time.getClock() << sep		// 4
		  << TIME_ZONE << sep				// 5
		  << start_time.dateString() << sep		// 6
		  << start_time.timeString() << sep		// 7
		  << random() % 100 << sep			// 8 
		  << random() % 100 << sep			// 9
		  << stop_time.timeString() << sep		// 10
		  << random() % 100 << sep 			// 11
		  << (random() % 254) + 1 << sep		// 12
		  << "VoIP" << sep				// 13
		  << "IP-TO-PSTN" << sep			// 14
		  << "DEFAULT" << sep				// 15
		  << "0288" << sep				// 16
		  << cli << sep					// 17
		  << cld << sep					// 18
		  << sep					// 19
		  << 0 << sep					// 20
		  << sep					// 21
		  << 0 << sep					// 22
		  << sep					// 23
		  << 0 << sep					// 24
		  << sep					// 25
		  << "ROUTE01" << sep				// 26
		  << 1 << sep					// 27
		  << "10.99.81.1:TG-A7N4-ATT1" << sep		// 28
		  << "10.99.81.1" << sep			// 29
		  << "10.99.84.1" << sep			// 30
		  << " " << sep					// 31
		  << " " << sep					// 32
		  << "10.7.2.5:5002/10.7.2.6:5002" << sep	// 33
		  << "1:7:2:1:11025:0x00040404:0x00030303" << sep //34
		  << " " << sep					// 35
		  << 0 << sep					// 36
		  << sep					// 37
		  << sep					// 38
		  << "0x00000002" << sep			// 39
		  << " " << sep					// 40
		  << random() % 100 << sep			// 41
		  << random() % 100 << sep			// 42
		  << "0x01" << sep				// 43
		  << "0x01" << sep				// 44
		  << "\"SIP,917506_105429@10.98.84.1,  "
		     << "<sip:" << cli << "@10.98.84.1>,  "
		     << "<sip:" << cld << "@10.98.81.1>\"" << sep // 45
		  << 12 << sep					// 46
		  << 1 << sep					// 47
		  << 0 << sep					// 48
		  << 5 << sep					// 49
		  << " " << sep					// 50
		  << " " << sep					// 51
		  << "0x0a" << sep				// 52
		  << cld << sep					// 53
		  << 1 << sep					// 54
		  << 1 << sep					// 55
		  << " " << sep					// 56
		  << random() % 3 << sep			// 57
		  << "TG-A7N4-ATT1" << sep			// 58
		  << ""						// 59
		  << endl << flush;
	}

private:
	string cli, cld;
	unsigned int call_id;
	unsigned int finish_time;
	SonusDateTime start_time;
};



class CallGenerator
{
public:
	CallGenerator()
	{
		out.open("/tmp/callgen-sonus", ios::out);
		out << version << endl
		    << reboot << endl << flush;
	}

	void startCall()
	{
		calls.push_back(new Call());
		calls.back()->writeStart(out);
		CALL_ID++;
		if (CALL_ID >= ROLLOVER)
		{
			BOOT_COUNT++;
			CALL_ID = 1;
		}
	}

	void attemptCall()
	{
		Call().writeAttempt(out);
		CALL_ID++;
		if (CALL_ID >= ROLLOVER)
		{
			BOOT_COUNT++;
			CALL_ID = 1;
		}
	}

	void stopCalls()
	{
		list<Call*>::iterator i = calls.begin();

		while (i != calls.end())
		{
			if ((*i)->finished())
			{
				(*i)->writeStop(out);
				Call *c = *i;
				calls.erase(i);
				delete c;
				i--;
			}
			i++;
		}
	}




	void mainLoop()
	{
		while (CALL_ID < 100000)
		{
			stopCalls();

			if (random() % 2)
			{
				startCall();
			}
			else
			{
				attemptCall();
			}

			//writeIntermediates(calls);


			CLOCK++;
//			usleep(1000);
		}
		while (calls.size() > 0)
		{
			stopCalls();
			CLOCK++;
		}
	}
private:
	list<Call*> calls;
	ofstream out;
};

XCDR_NAMESPACE_END

int main()
{
	XCDR::initClock();

	XCDR::CallGenerator callgen;

	callgen.mainLoop();
}

