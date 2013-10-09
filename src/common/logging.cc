// =========================================================================
//
// XCDR, Version 1.0
//
// Copyright (c) 2003 Thomas D. Marsh. All rights reserved.
// 
// =========================================================================
//
// Redistribution and use is subject to agreement with the author(s). See
// the doc/LICENSE file for more details.
// 
// THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
// ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
// USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
// OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
// SUCH DAMAGE.
// 
// =========================================================================
// 
// $Source: /usr/local/cvs/xcdr/src/common/logging.cc,v $
// $Revision: 1.6 $
// $Date: 2003/04/02 20:35:51 $
// $Author: tmarsh $
// 
// -------------------------------------------------------------------------
//
// $Log: logging.cc,v $
// Revision 1.6  2003/04/02 20:35:51  tmarsh
// added XCDR namespacing; eradicated all using namespace std
//
// Revision 1.5  2003/03/19 00:02:49  tmarsh
// added timestamp suppression
//
// Revision 1.4  2003/03/09 08:26:12  tmarsh
// include file reorganization
//
// Revision 1.3  2003/03/06 11:20:02  tmarsh
// updated headers
//
// 
// =========================================================================

#include "common/logging.h"

#include <ctime>
extern "C" {
#include <values.h>
}

XCDR_NAMESPACE_BEGIN

namespace Log
{
	const unsigned int Trace     = 0x01;
	const unsigned int Debug     = 0x02;
	const unsigned int Info      = 0x04;
	const unsigned int Warn      = 0x08;
	const unsigned int Error     = 0x10;
	const unsigned int Fatal     = 0x20;
	const unsigned int Exception = 0x40;
	const unsigned int Todo	     = 0x80;

	const unsigned int default_filter = Info | Warn | Error | Fatal | Exception;

	std::map<std::string, bool> msg_seen;

	std::string path = "";
	unsigned int id = MAXINT;
	unsigned int filter = default_filter;

	std::ofstream fp;

	unsigned int trace_level = 0;
	bool _is_open = false;
	bool suppress_timestamp = false;

	void details()
	{
		unsigned int i = 1;
		std::cerr << "Filters enabled: " << std::endl;
		while (i < Todo+1)
		{
			if ((i == Trace) && ((filter & i) == i))
				std::cerr << "  Trace" << std::endl;
			if ((i == Debug) && ((filter & i) == i))
				std::cerr << "  Debug" << std::endl;
			if ((i == Info) && ((filter & i) == i))
				std::cerr << "  Info" << std::endl;
			if ((i == Warn) && ((filter & i) == i))
				std::cerr << "  Warn" << std::endl;
			if ((i == Error) && ((filter & i) == i))
				std::cerr << "  Error" << std::endl;
			if ((i == Fatal) && ((filter & i) == i))
				std::cerr << "  Fatal" << std::endl;
			if ((i == Exception) && ((filter & i) == i))
				std::cerr << "  Exception" << std::endl;
			if ((i == Todo) && ((filter & i) == i))
				std::cerr << "  Todo" << std::endl;
			i *= 2;
		}
		std::cerr << std::flush;
	}

	bool initialized()
	{
		if ((id == MAXINT) || (path == ""))
		{
			return false;
		}
		return true;
	}

	void open()
	{
		assert(initialized());

		std::string home = getenv("HOME");
		std::string filename(home + "/var/log/");
		filename += path;
		fp.open(filename.c_str(), std::ios::out | std::ios::app);
		fp << "test" << std::endl << std::flush;
		if (fp.good())
		{
			_is_open = true;
		}
		else
		{
			std::cerr << "Couldn't open logs"
			     << std::endl << std::flush;

			_is_open = false;

			details();

			std::cerr << "log path: >>" << filename << "<<"
			     << std::endl << std::flush;
		}
	}


	void end()
	{
		(_is_open ? fp : std::cerr) << std::endl << std::flush;
	}

	void begin()
	{
		if (!suppress_timestamp)
		{
			time_t rawtime;
			struct tm *timeinfo;
			char buf[200];

			time(&rawtime);
			timeinfo = localtime(&rawtime);

			(void) strftime(buf, 200, "%Y-%m-%d %H:%M:%S: ", timeinfo);
			(_is_open ? fp : std::cerr) << buf;
		}
	}
}

XCDR_NAMESPACE_END
