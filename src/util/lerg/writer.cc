#include "writer.h"

XCDR_NAMESPACE_BEGIN

void Writer::write(const string &name, const map<string,string> &data)
{
	string path = "out/" + name + ".txt";
	ofstream fp(path.c_str(), fstream::out);

	map<string,string>::const_iterator i = data.begin();
	while (i != data.end())
	{
		fp << i->second << "\t" << i->first << endl;
		i++;
	}
	fp.close();
}

void Writer::write(const string &name, const map<string,list<string> > &data)
{
	string path = "out/" + name + ".txt";
	ofstream fp(path.c_str(), fstream::out);

	map<string,list<string> >::const_iterator i = data.begin();
	while (i != data.end())
	{
		fp << "::" << i->first << "::" << endl;
		list<string>::const_iterator j = i->second.begin();
		while (j != i->second.end())
		{
			fp << *j;
			j++;
			fp << ((j != (i->second.end())) ? " " : "\n\n");
		}
		i++;
	}
	fp.close();
}

XCDR_NAMESPACE_END
