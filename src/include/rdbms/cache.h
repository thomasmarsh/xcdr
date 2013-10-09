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
// $Source: /usr/local/cvs/xcdr/src/include/rdbms/cache.h,v $
// $Revision: 1.3 $
// $Date: 2003/04/04 16:03:58 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: cache.h,v $
// Revision 1.3  2003/04/04 16:03:58  tmarsh
// namespace changes
//
// Revision 1.2  2003/03/09 18:38:58  tmarsh
// include changes
//
// Revision 1.1  2003/03/09 08:35:32  tmarsh
// include file reorganization
//
// Revision 1.1  2003/03/07 11:26:44  tmarsh
// added
//
//
// =========================================================================

#ifndef XCDR_RDBMS_CACHE_H
#define XCDR_RDBMS_CACHE_H

#include "common/common.h"
#include "common/logging.h"
#include "common/except.h"
#include "rdbms/context.h"

#include <ctime>

XCDR_NAMESPACE_BEGIN

namespace RDBMS
{
	// ----------------------------------------------------------
	//
	// ROW CACHING CONSTRUCTS
	//
	// ----------------------------------------------------------

	namespace Cache
	{

		// --------------------------------------------------
		//
		// The Data class holds the details of a single
		// rowcached item.
		//
		// --------------------------------------------------

		template <class T>
		struct Data
		{
			unsigned int access_count;
			time_t create_time;
			T data;

			inline Data(const T &d)
			{
				access_count = 0;
				TODO("implement clock; this is slow");
				create_time = time(NULL);
				data = d;
			}
			inline Data()
			{
				access_count = 0;
				TODO("implement clock; this is slow");
				create_time = time(NULL);
			}
		};


		/**
		 *
		 * <p>Base is the templated abstract cache for RDBMS
		 * users. Implementations of this class must implement
		 * a constructor</p>
		 */

		template <class IN, class OUT>
		class Base
		{
		public:
			inline OUT get(const IN &input)
			{
				TODO("implement oldest/weakest");
				if (cache.size() > 10000)
				{
					reap();
				}

				if (cache.count(input) == 0)
				{
					cache.insert(std::make_pair<IN, Data<OUT> >(input, Data<OUT>(retrieve(input))));
				}
				cache[input].access_count++;
				last_get = input;
				return cache[input].data;
			}

			void setContext(Context *_context)
			{
				context = _context;
			}

			virtual void initialize()
			{
			}

			void initialize(RDBMS::Context *&_context,
						unsigned int _min_results,
						unsigned int _max_results,
						unsigned int _max_age)
			{
				context = _context;
				min_results = _min_results;
				max_results = _max_results;
				max_age = _max_age;
			}

			virtual ~Base() {}
		protected:
			inline Base(Context *_context = NULL,
				    unsigned int _min_results=0,
			            unsigned int _max_results=1000,
			            unsigned int _max_age=3600)
			{
				context = _context;
				min_results = _min_results;
				max_results = _max_results;
				max_age = _max_age;
			}


			virtual void buildQuery(Query &query, const IN &input) = 0;

			virtual OUT processResults(ResultStore &results) = 0;

			OUT retrieve(const IN &input)
			{
				if (context == NULL)
				{
					FATAL("Conext is null in retrieve");
				}
				assert(context != NULL);

				Query query =
				  (context->getConnection())->create_Query();

				buildQuery(query, input);

				DEBUG("Query: " << query.preview());

				ResultStore results = query.store();

				DEBUG("Records found: " << results.size());

				if (results.size() < min_results)
				{
					if (results.size() == 0)
					{
						ERROR("no entry for input");
						throw NoResults("no data");
					}
					ERROR("too few results");
					throw TooFewResults("too few");
				}
				if (results.size() > max_results)
				{
					ERROR("to many results");
					throw TooManyResults("too many");
				}

				return processResults(results);
			}

			inline void reap()
			{
				TODO("complex reaping code here...");
				cache.erase(cache.begin(), cache.end());
			}

			Context *context;

			unsigned int min_results;
			unsigned int max_results;
			unsigned int max_age;
			IN last_get;
		private:
			static std::map<IN, Data<OUT> > cache;
		};
	}
}


XCDR_NAMESPACE_END

#endif // XCDR_RDBMS_H
