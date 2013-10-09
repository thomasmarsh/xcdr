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
// $Source: /usr/local/cvs/xcdr/src/util/rmap/lerg.h,v $
// $Revision: 1.1 $
// $Date: 2003/04/08 14:30:53 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: lerg.h,v $
// Revision 1.1  2003/04/08 14:30:53  tmarsh
// added
//
// Revision 1.3  2003/04/04 16:03:58  tmarsh
// namespace changes
//
// Revision 1.2  2003/03/27 19:15:22  tmarsh
// rater rewrite
//
// Revision 1.1  2003/03/17 23:47:22  tmarsh
// moved to root build dir
//
// Revision 1.2  2003/03/15 21:09:09  tmarsh
// added dtor and size method
//
// Revision 1.1  2003/03/14 17:56:43  tmarsh
// added
//
//
// =========================================================================

#ifndef XCDR_RMAP_LERG_H
#define XCDR_RMAP_LERG_H

#include "rmap.h"
#include "tzmap.h"
#include "loc_info.h"

#include <fstream>

XCDR_NAMESPACE_BEGIN

class LERGLoader
{
public:
	enum LERGIndex
	{
		IDX_LATA = 0,
		IDX_NPA = 4,
		IDX_NXX = 5,
		IDX_OCN = 13,
		IDX_LOCNAME=14,
		IDX_COUNTY=15,
		IDX_STATE=16
	};

	enum MapTarget
	{
		OCN,
		LATA,
		PROVINCE,
		COUNTRY,
		TIMEZONE
	};

	LERGLoader(const std::string &fname, const std::string &loc_info);

	~LERGLoader();

	void process(MapTarget t, const std::string &outfile);

private:
	char *store(const std::string &s);
	void handleLine(const std::string &line);
	void loadLocationMap();
	void setup();
	void finish();

	rmap	*basic_map;
	tzmap	*timezone_map;
	ttree::set<char*,char> st_tree;
	std::list<char*> storage;

	std::string outfname;
	std::ifstream fp;
	LocationMap lmap;
	MapTarget target;

};

XCDR_NAMESPACE_END

#endif // XCDR_RMAP_LERG_H
