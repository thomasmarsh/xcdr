#include "common/logging.h"
#include "common/ttree.h"

#include <iostream>
#include <fstream>

using namespace std;


int main()
{
	Log::filter |= Log::Debug | Log::Trace;
	typedef XCDR::ttree::map<char*,char,char*> CharMap;

	CharMap map;
	XCDR::ttree::set<char*,char> set;

	char *k1, *k2, *k3, *k4;
	char *v1, *v2, *v3, *v4;

	k1 = strdup("foobar");
	v1 = strdup("kjdk");
	k2 = strdup("let");
	v2 = strdup("djdk");
	k3 = strdup("letm");
	v3 = strdup("djdkd");
	k4 = strdup("letme");
	v4 = strdup("haha");

	map.insert(k1, v1);
	map.insert(k2, v2);
	map.insert(k3, v3);
	map.insert(k4, v4);

	INFO("size: " << map.size());
	INFO("count: " << map.count());
	INFO("map.has_key(\"foobar\") = " << map.has_key("foobar"));
	INFO("map[\"foobar\"] = " << map["foobar"]);
	CharMap::iterator j = map.inorder();
	while (!j.done())
	{
		cout << (*j)->key << ": " << (*j)->data << endl;
		j++;
	}


	INFO("loading dictionary...");
	ifstream fp("/usr/share/dict/words", ifstream::in);
	char buf[256];
	while (!fp.eof())
	{
		fp.getline(buf, 256);
		if (strlen(buf))
		{
			char *s = strdup(buf);
			char *v = strdup(v4);
			map.insert(s, v);
			char *s2 = strdup(s);
			set.insert(s2);
		}
	}
	fp.close();
	INFO("size: " << map.size());
	INFO("count: " << map.count());
	INFO("size: " << set.size());
	INFO("count: " << set.count());

	INFO("printing first 10 entries...");

	CharMap::iterator i = map.inorder();

	for (int k = 0; (k < 10) && (!i.done()); k++)
	{
		cout << (*i)->key << endl;
		i++;
	}
}
