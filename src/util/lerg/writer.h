#ifndef WRITER_H
#define WRITER_H

#include "common.h"

XCDR_NAMESPACE_BEGIN

class Writer
{
public:
	void write(const string &name, const map<string,string> &input);
	void write(const string &name, const map<string,list<string> > &input);
};

XCDR_NAMESPACE_END

#endif // WRITER_H
