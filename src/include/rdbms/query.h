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
// $Source: /usr/local/cvs/xcdr/src/include/rdbms/query.h,v $
// $Revision: 1.4 $
// $Date: 2003/04/04 16:03:58 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: query.h,v $
// Revision 1.4  2003/04/04 16:03:58  tmarsh
// namespace changes
//
// Revision 1.3  2003/03/27 19:18:27  tmarsh
// put repository back in
//
// Revision 1.2  2003/03/14 17:57:33  tmarsh
// big xml changes
//
// Revision 1.1  2003/03/09 08:35:32  tmarsh
// include file reorganization
//
// Revision 1.5  2003/03/07 22:28:39  tmarsh
// more debugging
//
// Revision 1.4  2003/03/07 18:08:33  tmarsh
// repository fixes
//
// Revision 1.3  2003/03/07 17:23:08  tmarsh
// updates to compile
//
// Revision 1.2  2003/03/07 13:27:07  tmarsh
// fixes to compile
//
// Revision 1.1  2003/03/07 11:26:44  tmarsh
// added
//
//
// =========================================================================

#ifndef XCDR_RDBMS_QUERY_H
#define XCDR_RDBMS_QUERY_H

#include "common/common.h"
#include "rdbms/context.h"
#include "rdbms/model.h"
#include "rdbms/repository.h"
#include "xml/xml.h"

XCDR_NAMESPACE_BEGIN

namespace RDBMS
{

	/**
	  * Provide parameterized &lt;query&gt; blocks with their parameters
	  * via a generic interface.
	  *
	  * <p>Th</p>
	  */

	class Parameters
	{
	public:
		/* @name Constructors and Destructor */
		//@{
		///** Default constructor */

		Parameters();

		/** Destructor */
		~Parameters();
		//@}

		/** @name Interface for adding and getting parameters */

		//@{
	
		/**
		  * Add a string parameter.
		  *
		  * <p>A provided string parameter <code>val</code> will
		  * be stored as an escaped quoted string. Therefore, the
		  * string <code>"Tom's test"</code> will be stored as
		  * "'Tom\'s test'".</p>
		  */
		void add(std::string &name, std::string &val);

		void add(std::string &name, unsigned int &val);

		void add(std::string &name, std::list<std::string> &val);

		std::string &operator [] (const std::string &name);

		inline bool operator < (const Parameters &other) const
		{
			return params < other.params;
		}

		//@}
	private:
		std::map<std::string, std::string> params;
	};


	/**
	  * An interface to the query repository groups.
	  */
	class QueryGroup
	{
	public:
		typedef std::pair<std::string, Parameters> InputPair;

		QueryGroup(XML::Group &g)
		{
			ENTER();
			group = g;
			DEBUG("Group name: " << g.name);
			LEAVE();
		}

		ResultStore query(const std::string &name, Parameters &p = paramsNull)
		{
			ENTER();
			InputPair input;
			input.first = name;
			input.second = p;
			if (!query_cache.count(input))
			{
				build(input);
			}

			Query q = GetContext()->createQuery();

			q << query_cache[input];

			DEBUG("QUERY PREVIEW: " << q.preview());

			LEAVE();
			return q.store();
		}

		void execute(const std::string &name, Parameters &p = paramsNull)
		{
			InputPair input;
			input.first = name;
			input.second = p;
			if (!query_cache.count(input))
			{
				build(input);
			}

			GetContext()->execute(query_cache[input]);
		}

		void build(InputPair &input);

	private:
		XML::Group group;

		static Parameters paramsNull;
		std::map<InputPair, std::string> query_cache;
	};


	/**
	  * An interface to the query repository.
	  */

	class Repository
	{
	public:
		Repository(std::string &name);
		QueryGroup *getGroup(const std::string &name)
		{
			ENTER();
			if (!repository->groups.count(name))
			{
				FATAL("No group: " << name);
				TODO("Throw an exception here");
			}
			DEBUG("Allocating group");
			QueryGroup *group =
				new QueryGroup(repository->groups[name]);

			LEAVE();
			return group;
		}
	private:
		void initialize()
		{
			SimpleXMLParser p;
			repository = p.getRepository("/home/xcdr/etc/repository.xml");
			DEBUG("REPOSITORY MODEL='" << repository->model << "'");
		}

		Model::Model *model;
		XML::Repository *repository;

		std::string name;
	};
}

XCDR_NAMESPACE_END

#endif // XCDR_RDBMS_QUERY_H
