#include "tok.h"
#include "lerg.h"
#include "writer.h"
#include "xml/parser.h"

XCDR_NAMESPACE_BEGIN

Loader::Loader(const string &fname)
{
	fp.open(fname.c_str(), ifstream::in);
	tz_map = NULL;
}

Loader::~Loader()
{
	if (tz_map) delete tz_map;
	fp.close();
}

map<string,list<string> > Loader::inverse(const map<string,string> &input)
{
	map<string,list<string> > inverse;
	map<string,string>::const_iterator i = input.begin();

	while (i != input.end())
	{
		inverse[i->second].push_back(i->first);
		i++;
	}

	return inverse;
}

void Loader::reduce()
{
	INFO("reducing: state...");
	state_final = state_map.reduce();
	INFO("reducing: country...");
	country_final = country_map.reduce();
	INFO("reducing: ocn...");
	ocn_final = ocn_map.reduce();
	INFO("reducing: lata...");
	lata_final = lata_map.reduce();
	INFO("reducing: timezone...");
	tz_final = tz_map->reduce();
}

void Loader::handleLine(const string &line)
{
	Tokenizer t(line, "\t");

	string dest = "1";
	if (t.size() > STATE)
	{
		if (t[NXX] != "+")
		{
			dest += t[NPA] + t[NXX];
			dests.push_back(dest);

			// 
			// OCN map
			// 

			ocn_map.add(dest, t[OCN]);

			// 
			// LATA map
			// 

			lata_map.add(dest, t[LATA]);

			// 
			// state map
			// 

			state_map.add(dest, t[STATE]);

			//
			// country map
			// 

			if (!lmap.count(t[STATE]))
				WARN("no country for state: " << t[STATE]);

			country_map.add(dest, lmap[t[STATE]].iso_3661);

			// 
			// Time zone information
			// 

			tz_map->add(dest, t[STATE], t[LOCNAME]);
		}
	}
}


void Loader::run()
{
	INFO("loading loc info...");
	XMLParser p;
	p.parse("loc_info.xml", &lmap);
	tz_map = new TimezoneMap(&lmap);
	INFO("parsing lerg...");
	unsigned int count = 0;
	while (!fp.eof())
	{
		char line_buf[512];
		fp.getline(line_buf, 512);
		handleLine(line_buf);
		if ((count % 500) == 0)
		{
			cerr << "records: " << count << "\r" << flush;
		}
		count++;
	}
	INFO("records: " << count);
	reduce();
	INFO("printing...");

	Writer w;

	w.write("state", state_final);
	w.write("state.inverse", inverse(state_final));

	w.write("country", country_final);
	w.write("country.inverse", inverse(country_final));

	w.write("ocn", ocn_final);
	w.write("ocn.inverse", inverse(ocn_final));

	w.write("lata", lata_final);
	w.write("lata.inverse", inverse(lata_final));

	w.write("timezone", tz_final);
	w.write("timezone.inverse", inverse(tz_final));
}

XCDR_NAMESPACE_END
