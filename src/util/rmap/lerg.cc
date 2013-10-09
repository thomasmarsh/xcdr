#include "lerg.h"
#include "xml/parser.h"

XCDR_NAMESPACE_BEGIN

LERGLoader::LERGLoader(const std::string &fname, const std::string &loc_info)
{
	TODO("check success of file operations");

	INFO("Loading location map: " << loc_info);
	XMLParser p;
	p.parse(loc_info.c_str(), &lmap);

	fp.open(fname.c_str(), std::ifstream::in);
}

char *LERGLoader::store(const std::string &s)
{
	ttree::set<char*,char> *p = st_tree.find(&st_tree,
						 const_cast<char*>(s.c_str()));
	char *buf = 0;
	if (!p)
	{
		buf = strdup(s.c_str());
		storage.push_back(buf);
		st_tree.insert(buf);
	}
	else
		buf = p->key;

	return buf;
}

void LERGLoader::handleLine(const std::string &line)
{
	Tokenizer t(line, "\t");

	if ((t.size() < IDX_STATE) || (t[IDX_NXX] == "+"))
		return;

	std::string dest = "1" + t[IDX_NPA] + t[IDX_NXX];
	char *tmp = const_cast<char*>(dest.c_str());
	char *tmp2;
	switch (target)
	{
	case OCN:
		tmp2 = store(t[IDX_OCN]);
		basic_map->insert(tmp, tmp2);
		break;
	case LATA:
		tmp2 = store(t[IDX_LATA]);
		basic_map->insert(tmp, tmp2);
		break;
	case PROVINCE:
		tmp2 = store(t[IDX_STATE]);
		basic_map->insert(tmp, tmp2);
		break;
	case COUNTRY:
		if (!lmap.count(t[IDX_STATE]))
			WARN("no country for state: " << t[IDX_STATE]);
		tmp2 = store(lmap[t[IDX_STATE]].iso_3661);
		basic_map->insert(tmp, tmp2);
		break;
	case TIMEZONE:
		timezone_map->insert(dest, t[IDX_STATE], t[IDX_LOCNAME]);
		break;
	}
}

LERGLoader::~LERGLoader()
{
}

void LERGLoader::setup()
{
	if (target == TIMEZONE)
		timezone_map = new tzmap(&lmap);
	else basic_map = new rmap;
}


#define DIV(x) ((double) x / (double) 1024.0)
std::string byteSize(size_t b)
{
	double r;
	std::string type;

	if (b >= 1073741824) r = DIV(DIV(DIV(b))), type = "Gb";
	else if (b >= 1048576) r = DIV(DIV(b)), type = "Mb";
	else if (b >= 1024) r = DIV(b), type = "Kb";
	else r = (double) b, type = "bytes";

	char buf[20];
	snprintf((char *) &buf, 20, "%g", r);
	return std::string(buf) + type;
}


void LERGLoader::finish()
{
	INFO("\toutput: " << outfname);
	std::ofstream ofp(outfname.c_str(), std::ofstream::out);
	unsigned int count;
	if (target == TIMEZONE)
	{
		count = timezone_map->print(ofp);
		INFO("\tcleanup...");
		delete timezone_map;
		timezone_map = 0;
	}
	else
	{
		INFO("\tmap size: " << byteSize(basic_map->size()));
		INFO("\tidentifiers: " << storage.size());
		count = basic_map->print(ofp);
		INFO("\tcleanup...");
		delete basic_map;
		basic_map = 0;
	}
	INFO("\treduced: " << count);
	ofp.close();
	std::list<char*>::iterator i = storage.begin();
	while (i != storage.end())
	{
		delete *i;
		*i = 0;
		i++;
	}
	storage.erase(storage.begin(), storage.end());
	st_tree.clear();
}

void LERGLoader::process(MapTarget t, const std::string &file)
{
	target = t;
	outfname = file;
	setup();

	fp.clear();
	fp.seekg(0);
	unsigned int count = 0;
	while (!fp.eof())
	{
		char line_buf[512];
		fp.getline(line_buf, 512);
		handleLine(line_buf);
		if ((count % 1383) == 0)
			std::cerr << "\rrecords: " << count << std::flush;
		count++;
	}
	std::cerr << "\r";
	finish();
}

XCDR_NAMESPACE_END
