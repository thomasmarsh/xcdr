#ifndef TOK_H
#define TOK_H

#include "common.h"

XCDR_NAMESPACE_BEGIN

class Tokenizer : public vector<string>
{
public:
	Tokenizer(const string &str, const string &delim=",\n")
	{
		string::size_type last_pos(str.find_first_not_of(delim, 0));
		string::size_type pos(str.find_first_of(delim, last_pos));

		while ((string::npos != pos) || (string::npos != last_pos))
		{
			push_back(str.substr(last_pos, pos-last_pos));
			if (back() == "\\N")
				back() = "";
			last_pos = str.find_first_not_of(delim, pos);
			pos = str.find_first_of(delim, last_pos);
		}
	}
};

XCDR_NAMESPACE_END

#endif // TOK_H
