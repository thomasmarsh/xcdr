#ifndef TOK_H
#define TOK_H

#include <string>
#include <vector>

XCDR_NAMESPACE_BEGIN

class Tokenizer : public std::vector<std::string>
{
public:
	Tokenizer(const std::string &str, const std::string &delim=",\n")
	{
		std::string::size_type last_pos(str.find_first_not_of(delim, 0));
		std::string::size_type pos(str.find_first_of(delim, last_pos));

		while ((std::string::npos != pos) || (std::string::npos != last_pos))
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
