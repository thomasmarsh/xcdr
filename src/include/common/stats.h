// ==========================================================================
//
// XCDR, Version 1.0
//
// Copyright (c) 2003 Thomas D. Marsh. All rights reserved.
//
// ==========================================================================
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
// $Source: /usr/local/cvs/xcdr/src/include/common/stats.h,v $
// $Revision: 1.2 $
// $Date: 2003/04/02 20:34:25 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: stats.h,v $
// Revision 1.2  2003/04/02 20:34:25  tmarsh
// added XCDR namespace
//
// Revision 1.1  2003/03/09 08:35:31  tmarsh
// include file reorganization
//
// Revision 1.2  2003/03/06 11:20:02  tmarsh
// updated headers
//
//
// =========================================================================

#ifndef XCDR_STATS_H
#define XCDR_STATS_H

#include "common/common.h"
#include <fstream>
#include <string>
#include <memory>
#include <map>

extern "C" {
#include <sys/time.h>
}

XCDR_NAMESPACE_BEGIN

/*
 * TODO:XXX: Fix this fp copying business...
 *
 * Usage:
 *
 * 	class MyClass
 * 	{
 * 	private:
 * 		Statistics::Manager stats;
 * 		...
 * 	};
 *
 *
 * 	MyClass::MyClass()
 * 	{
 * 		stats.addCounter("cps");
 * 	}
 *
 * 	void MyClass::UpdateStats()
 * 	{
 * 		stats["cps"].update(100);
 * 	}
 */

namespace Statistics
{

	class Counter
	{
	public:
		Counter(const std::string &_name, std::string &path)
		{
			name = _name;
			std::string fqp = path + "/" + name;
			fp = new std::ofstream();
			fp->open(fqp.c_str(), std::ios::out | std::ios::app);
		}

		~Counter()
		{
//			fp->close();
//			delete fp;
		}

		inline long getTime()
		{
			struct timeval t;

			gettimeofday(&t, NULL);

			return t.tv_sec;
		}

		template <typename T>
		void update(T stat)
		{
			*fp << getTime() << "\t" << stat
			   << std::endl << std::flush;
		}

		std::string getName() const { return name; }
		std::ofstream *getFile() const { return fp; }

		Counter(const Counter &c)
		{
			name = c.getName();
			fp = c.getFile();
		}
		Counter() {}
	private:
		std::string name;
		std::ofstream *fp;
	};

	class Manager
	{
	public:
		Manager(const std::string &_module, unsigned int _id)
		{
			module = _module;
			id = _id;
		}

		void addCounter(const std::string &name)
		{
//			TODO("format as $HOME/var/stat/<entity>.<id>/<name>");
			std::string path = "/home/xcdr/var/stat";
			counters.insert(counters.end(),
					std::make_pair<std::string,Counter>(
						name, Counter(name, path)));
		}

		Counter &operator [] (const char *n)// const
		{
			std::string name(n);
			assert(counters.count(name) == 1);

			std::map<std::string, Counter>::iterator i;

			i = counters.find(name);

			return (*i).second;
		}

	private:
		std::map<std::string, Counter> counters;
		std::string module;
		unsigned int id;
	};
}

XCDR_NAMESPACE_END

#endif // XCDR_STATS_H
