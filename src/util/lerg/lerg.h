#ifndef LERG_H
#define LERG_H

#include "rmap.h"
#include "tzmap.h"
#include "loc_info.h"

XCDR_NAMESPACE_BEGIN

const unsigned int LATA = 0;
const unsigned int NPA = 4;
const unsigned int NXX = 5;
const unsigned int OCN = 13;

const unsigned int LOCNAME=14;
const unsigned int COUNTY=15;
const unsigned int STATE=16;

class Loader
{
public:
	Loader(const string &fname);

	~Loader();

	map<string,list<string> > inverse(const map<string,string> &input);

	void reduce();

	void handleLine(const string &line);

	void run();

private:
	list<string> dests;
	ReducingMap state_map;
	ReducingMap country_map;
	ReducingMap lata_map;
	ReducingMap ocn_map;
	TimezoneMap *tz_map;
	LocationMap lmap;
	ifstream fp;

	map<string,string> state_final;
	map<string,string> country_final;
	map<string,string> lata_final;
	map<string,string> ocn_final;
	map<string,string> tz_final;
};

XCDR_NAMESPACE_END

#endif // LERG_H
