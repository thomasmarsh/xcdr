#ifndef RMAP_H
#define RMAP_H

#include "common.h"

XCDR_NAMESPACE_BEGIN

class ReducingMap
{
public:
	ReducingMap();
	void add(const string &key, const string &value);

	bool expected(const list<string> &l, string &value);

	string uniqueKey(const string &key);

	map<string,string> &reduce();

	const map<string,string> &getBase() { return base_data; }
private:
	map<string,string> base_data;
	map<string,list<string> > base_data_inverse;
	map<unsigned int, map<string, list<string> > > lev_data;

	map<string,string> final;
};

XCDR_NAMESPACE_END

#endif // RMAP_H
