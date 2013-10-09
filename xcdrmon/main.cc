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
// $Source: /usr/local/cvs/xcdr/xcdrmon/main.cc,v $
// $Revision: 1.2 $
// $Date: 2003/03/06 11:20:05 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: main.cc,v $
// Revision 1.2  2003/03/06 11:20:05  tmarsh
// updated headers
//
//
// =========================================================================

#include "monitor.h"

#include <iostream>
#include <fstream>
#include <map>
#include <list>

#include <unistd.h>
#include <time.h>
#include <sys/time.h>

using namespace std;


long clock()
{
	struct timeval tv;

	gettimeofday(&tv, NULL);

	return tv.tv_sec;
}

class LogGenerator
{
public:
	LogGenerator(string _name, string _user_string)
	{
		name = _name;
		user_string = _user_string;
		out.open((name + ".log").c_str(), ios::out);
		start = clock();
	}
	void update(unsigned int value)
	{
		out << (clock() - start) << "\t" << value << endl << flush;
	}

	string getName() { return name; }
	string getUserString() { return user_string; }
private:
	string name;
	string user_string;
	ofstream out;
	unsigned int start;
};

int main(int argc, char **argv)
{
	LogGenerator raw("raw", "Raw Records Count");
	LogGenerator med("med", "Mediated Records Count");
	LogGenerator cps("cps", "Calls / interval");

	LogGenerator t_unk("t_unk", "UNKNOWN's in CDR_Raw");
	LogGenerator t_ver("t_ver", "VERSION's in CDR_Raw");
	LogGenerator t_reb("t_reb", "REBOOT's in CDR_Raw");
	LogGenerator t_swi("t_swi", "SWITCHOVER's in CDR_Raw");
	LogGenerator t_sta("t_sta", "START's in CDR_Raw");
	LogGenerator t_sto("t_sto", "STOP's in CDR_Raw");
	LogGenerator t_att("t_att", "ATTEMPT's in CDR_Raw");
	LogGenerator t_int("t_int", "INTERMEDIATES's in CDR_Raw");

	map<int, LogGenerator*> tmap;
	tmap.insert(make_pair(0, &t_unk)); tmap.insert(make_pair(1, &t_ver));
	tmap.insert(make_pair(2, &t_reb)); tmap.insert(make_pair(3, &t_swi));
	tmap.insert(make_pair(4, &t_sta)); tmap.insert(make_pair(5, &t_sto));
	tmap.insert(make_pair(6, &t_att)); tmap.insert(make_pair(7, &t_int));

	list<unsigned int> mediated;
	list<long> mtimes;
	unsigned int r = 0, m = 0, c = 0, lm = 0, d = 0;
	bool started=false;
	unsigned int start_clock;
	const unsigned int MSIZE = 10;
	struct timeval t1, t2;
	gettimeofday(&t1, NULL);
	gettimeofday(&t2, NULL);
	long vsleep = t1.tv_usec;
	long vftime = 1000000;
	int wait = 0;
	while (1)
	{
		gettimeofday(&t2, NULL);
		if (t2.tv_usec < vsleep)
			vsleep -= 1000000;
		wait = (vftime - t2.tv_usec + vsleep);
		r = Monitor::getCountCDR_Raw();
		m = Monitor::getCountCDR_Mediated();
		raw.update(r);
		med.update(m);

		d = m - lm;
		lm = m;
		if (d > 0)
		{
			if (mediated.size() >= MSIZE)
			{
				mediated.pop_front();
				mtimes.pop_front();
				mediated.push_back(d);
				mtimes.push_back(clock());
			}
			else if (mediated.size() > 0)
			{
				mediated.push_back(d);
				mtimes.push_back(clock());
			}
			else if (mediated.size() == 0)
			{
				mediated.push_back(d);
				mtimes.push_back(clock());
			}

			unsigned int sum = 0;
			list<unsigned int>::iterator i = mediated.begin();
			while (i != mediated.end())
			{
				sum += (*i);
				i++;
			}
			long delta = mtimes.back() - mtimes.front();
			if (delta == 0) delta = 1;
			c = sum / delta;
		}

		cps.update(c);

		map<unsigned int, unsigned int> spread;

		spread = Monitor::getSpreadCDR_Raw();

		for (unsigned int x = 0; x < 8; x++)
		{
			tmap[x]->update(spread[x]);
		}
		cerr << "." << flush;

		gettimeofday(&t2, NULL);
		if (t2.tv_usec < vsleep)
			vsleep -= 1000000;
		wait = (vftime - t2.tv_usec + vsleep);
		usleep(wait < 0 ? 0 : wait);
		gettimeofday(&t1, NULL);
	}
}
