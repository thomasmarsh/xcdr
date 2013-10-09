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
// $Source: /usr/local/cvs/xcdr/src/common/csv.cc,v $
// $Revision: 1.4 $
// $Date: 2003/04/02 20:35:51 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: csv.cc,v $
// Revision 1.4  2003/04/02 20:35:51  tmarsh
// added XCDR namespacing; eradicated all using namespace std
//
// Revision 1.3  2003/03/09 08:26:11  tmarsh
// include file reorganization
//
// Revision 1.2  2003/03/06 11:20:02  tmarsh
// updated headers
//
//
// =========================================================================

#include "common/csv.h"
#include "common/strtok.h"

XCDR_NAMESPACE_USE;

using namespace Parser;

CSVParser::CSVParser(const std::string &line)
{
//	data = line;


	try
	{
		StringTokenizer tokenizer(line, ",");
		while (1)
		{
			cache.push_back(tokenizer.getNext());
		}
	}
	catch (StringTokenizer::NoMoreTokensException &exc)
	{
	}
}

CSVParser::~CSVParser()
{
}


std::string CSVParser::operator [] (std::size_t pos)
{
	assert(pos < cache.size());

	return cache[pos];
}

