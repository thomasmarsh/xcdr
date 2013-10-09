// --------------------------------------------------------------------------
//
// PERIOD TEST
//
// --------------------------------------------------------------------------

#include "common/period.h"
#include "common/version.h"

#include <iostream>

using namespace XCDR;

int main()
{
//	Log::filter |= Log::Trace;
//	Log::filter |= Log::Debug;

	std::cout << XCDRBuildInformation();

	char str[256];
	for (;;)
	{
		std::cout << "> ";

		std::cin.getline(str, 256);

		if (str[0] == 'q')
			break;

		try
		{
			Period p(str);
			std::cout << p.toString() << std::endl;
		}
		catch (Period::PeriodError *exc)
		{
			std::cerr << (*exc) << std::endl;
			delete exc;
		}
	}
	return 0;
}
