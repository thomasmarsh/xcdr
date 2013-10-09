#ifndef TZMAP_H
#define TZMAP_H

#include "loc_info.h"
#include "tok.h"
#include "common.h"
#include "common/logging.h"

XCDR_NAMESPACE_BEGIN

class TimezoneMap : public ReducingMap
{
public:
	TimezoneMap(LocationMap *l) : ReducingMap()
	{
		lmap = l;
	}

	bool hasLocation(const string &loc_name, const string &l)
	{
		Tokenizer t(l, "|");
		Tokenizer::iterator i = t.begin();
		while (i != t.end())
		{
			if ((*i) == loc_name)
				return true;
			i++;
		}
		return false;
	}

	void add(const string &dest,
		 const string &state,
		 const string &loc_name)
	{
		if (!lmap->count(state))
		{
			WARN("No timezone for state: " << state);
			return;
		}
		else
		{
			string tz = (*lmap)[state].default_tz;

			map<string,string>::iterator subdiv;
		       	subdiv = (*lmap)[state].subdiv_tz.begin();
			while (subdiv != (*lmap)[state].subdiv_tz.end())
			{
				if (hasLocation(subdiv->first, loc_name))
				{
					tz = subdiv->second;
					break;
				}
				subdiv++;
			}
			ReducingMap::add(dest, tz);
		}
	}
private:
	LocationMap *lmap;
};

XCDR_NAMESPACE_END

#endif // RMAP_H
