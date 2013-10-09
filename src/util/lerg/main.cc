#include "lerg.h"
#include "common/logging.h"

int main()
{
	Log::filter |= Log::Debug;
//	Log::filter |= Log::Trace;

	XCDR::Loader l("/tmp/lerg.txt");
	l.run();
}
