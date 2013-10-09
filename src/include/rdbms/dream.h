#include "xml/parser.h"
#include "xml/element.h"




struct Children
{
	Children &operator(const std::string &name)
	{
		contained.push_back(name);
		return *this;
	}

	std::list<std::string> contained;
};

struct Typedef : public Element<Typedef>
{
	void init();
	{
		name = "typedef";
		children ("profile");
	}

};

struct ModelParser : public XML::Parser<ModelParser>
{
	void init()
	{
		name = "model";

		children ("typedef")
			 ("entity")
			 ("foobar");
	}
};

Model getModel()
{
	ModelParser p;
	p.parse("/home/xcdr/etc/model.xml");
	Model m(p);
	return m;
}

int main()
{
	Model m = getModel();
}
