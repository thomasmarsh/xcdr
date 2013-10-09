#include "common/dtrie.h"
#include <iostream>

using namespace XCDR;
using namespace std;

int main()
{
	dtrie::digit_map<char*> dmap;

	char *s1 = "1512"; char *t1 = "TX";
	char *s2 = "1202"; char *t2 = "MO";
	char *s3 = "1513"; char *t3 = "OH";
	char *s4 = "1514"; char *t4 = "IN";
	char *s5 = "1534"; char *t5 = "IX";
	char *s6 = "1544"; char *t6 = "IZ";

	cout << dmap.count() << endl;
	dmap.insert(s1, t1);
	dmap.insert(s2, t2);
	dmap.insert(s3, t3);
	dmap.insert(s4, t4);
	dmap.insert(s5, t5);
	dmap.insert(s6, t6);

//	cout << dmap["15123124"] << endl;
//	cout << dmap["15123124"] << endl;
	cout << dmap.count() << endl;
	cout << dmap.size() << endl;
}
