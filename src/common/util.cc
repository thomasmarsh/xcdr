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
// $Source: /usr/local/cvs/xcdr/src/common/util.cc,v $
// $Revision: 1.6 $
// $Date: 2003/04/04 16:06:36 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: util.cc,v $
// Revision 1.6  2003/04/04 16:06:36  tmarsh
// namespace changes
//
// Revision 1.5  2003/04/02 20:35:51  tmarsh
// added XCDR namespacing; eradicated all using namespace std
//
// Revision 1.4  2003/03/09 08:26:12  tmarsh
// include file reorganization
//
// Revision 1.3  2003/03/07 22:28:55  tmarsh
// added libxcdrXML
//
// Revision 1.2  2003/03/06 11:20:02  tmarsh
// updated headers
//
//
// =========================================================================

#include "common/util.h"

XCDR_NAMESPACE_BEGIN

std::string Util::strip(const std::string &rhs)
{
	if (rhs == "")
		return rhs;

	std::string rv = rhs;

	while (rv.substr(0, 1) == " ")
	{
		rv = rv.substr(1, rv.length() - 1);

		if (rv.length() == 0)
			return "";
	}
	while (rv.substr(rv.length() - 1, 1) == " ")
	{
		rv = rv.substr(0, rv.length() - 1);

		if (rv.length() == 0)
			return "";
	}
	return rv;
}

std::string Util::stripnl(const std::string &rhs)
{
	if (rhs == "")
		return rhs;

	std::string rv = rhs;

	while ((rv.substr(0, 1) == " ") ||
	       (rv.substr(0, 1) == "\n"))
	{
		rv = rv.substr(1, rv.length() - 1);

		if (rv.length() == 0)
			return "";
	}
	while ((rv.substr(rv.length() - 1, 1) == " ") ||
	       (rv.substr(rv.length() - 1, 1) == "\n"))
	{
		rv = rv.substr(0, rv.length() - 1);

		if (rv.length() == 0)
			return "";
	}
	return rv;
}


void writePID(const std::string& module, const std::string& home_dir)
{
	std::ofstream pidfile((home_dir + "/var/" + module + ".pid").c_str());
		    
	pidfile << getpid() << std::endl << std::flush;
	pidfile.close();
}

XCDR_NAMESPACE_END
