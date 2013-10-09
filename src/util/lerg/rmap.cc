#include "rmap.h"

XCDR_NAMESPACE_BEGIN

ReducingMap::ReducingMap()
{
}

void ReducingMap::add(const string &key, const string &value)
{
	base_data.insert(base_data.end(),
			make_pair<string,string>(key, value));

	base_data_inverse[value].push_back(key);
	for (unsigned int i = 0; i < 5; i++)
	{
		string sub = key.substr(0, key.length()-i-1);
		lev_data[i][sub].push_back(value);
	}
}

bool ReducingMap::expected(const list<string> &l, string &value)
{
	list<string>::const_iterator i = l.begin();
	bool found = false;
	while (i != l.end())
	{
		if ((*i) == value)
			found = true;
		else
			return false;
		i++;
	}
	return found;
}

string ReducingMap::uniqueKey(const string &key)
{
	string last_sub = key;
	string value = base_data[key];
	for (unsigned int i = 0; i < 5; i++)
	{
		string sub = key.substr(0, key.length()-i-1);

		if (!expected(lev_data[i][sub], value))
			return last_sub;

		last_sub = sub;
	}
	return key;
}

map<string,string> &ReducingMap::reduce()
{
	map<string,string>::iterator i = base_data.begin();
	while (i != base_data.end())
	{
		string unique = uniqueKey(i->first);
		if (final.count(unique) == 0)
		{
			final.insert(final.end(),
				make_pair<string,string>(unique, i->second));
		}
//		else
//		{
//			if (final[unique] != i->second)
//			{
//				cerr << "mismatch: key=" << unique << ", "
//				     << "new=" << i->second << ", "
//				     << "old=" << final[unique]
//				     << endl << flush;
//			}
//		}
		i++;
	}
	return final;
}

XCDR_NAMESPACE_END
