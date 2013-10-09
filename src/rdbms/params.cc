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
// $Source: /usr/local/cvs/xcdr/src/rdbms/params.cc,v $
// $Revision: 1.6 $
// $Date: 2003/04/04 16:06:36 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: params.cc,v $
// Revision 1.6  2003/04/04 16:06:36  tmarsh
// namespace changes
//
// Revision 1.5  2003/03/27 19:20:24  tmarsh
// some changes to docs
//
// Revision 1.4  2003/03/09 08:28:22  tmarsh
// include file reorganization
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

#include "rdbms/query.h"
#include "common/util.h"

XCDR_NAMESPACE_BEGIN

using namespace RDBMS;

	/**
	  * Provide parameterized &lt;query&gt; blocks with their parameters
	  * via a generic interface.
	  *
	  * <p>Th</p>
	  */

		///** Default constructor */

Parameters::Parameters() {}

		/** Destructor */
Parameters::~Parameters() {}


	
		/**
		  * Add a string parameter.
		  *
		  * <p>A provided string parameter <code>val</code> will
		  * be stored as an escaped quoted string. Therefore, the
		  * string <code>"Tom's test"</code> will be stored as
		  * "'Tom\'s test'".</p>
		  */
void Parameters::add(std::string &name, std::string &val)
{
	params.insert(params.end(),
	      std::make_pair<std::string,std::string>
		(name, "'" + quote(val) + "'"));
}

void Parameters::add(std::string &name, unsigned int &val)
{
	params.insert(params.end(),
			std::make_pair<std::string,std::string>
			(name, Util::itoa(val)));
}

void Parameters::add(std::string &name, std::list<std::string> &val)
{
	std::string rv;
	std::list<std::string>::iterator i = val.begin();

	while (i != val.end())
	{
		rv += "'";
		rv += quote(*i);
		rv += "'";
		i++;
		rv += ((i != val.end()) ? ", " : "");
	}

	params.insert(params.end(),
			std::make_pair<std::string,std::string>
			(name, rv));
}

std::string &Parameters::operator [] (const std::string &name)
{
	assert(params.count(name) == 1);

	return params[name];
}

XCDR_NAMESPACE_END
