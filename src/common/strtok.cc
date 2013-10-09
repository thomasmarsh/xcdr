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
// $Source: /usr/local/cvs/xcdr/src/common/strtok.cc,v $
// $Revision: 1.5 $
// $Date: 2003/04/04 16:06:36 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: strtok.cc,v $
// Revision 1.5  2003/04/04 16:06:36  tmarsh
// namespace changes
//
// Revision 1.4  2003/04/02 20:35:51  tmarsh
// added XCDR namespacing; eradicated all using namespace std
//
// Revision 1.3  2003/03/09 08:26:12  tmarsh
// include file reorganization
//
// Revision 1.2  2003/03/06 11:20:02  tmarsh
// updated headers
//
//
// =========================================================================


#include "common/strtok.h"

XCDR_NAMESPACE_BEGIN

StringTokenizer::StringTokenizer(const std::string& raw,
				 const std::string& pattern)
{
    int prev_match = 0;
    int next_match = 0;
    separator = pattern;

    while ((next_match = raw.find(pattern, prev_match)) >= 0) {
	tokens.push_back(raw.substr(prev_match, (next_match - prev_match)));
	prev_match = next_match + 1;
    }

    tokens.push_back(raw.substr(prev_match, (raw.length() - prev_match)));
    next_token = tokens.begin();
    num_remaining = size();
}


std::string StringTokenizer::getNext(bool strip_mode)
    throw(NoMoreTokensException)
{
    if (num_remaining <= 0)
	throw NoMoreTokensException();

    num_remaining--;

    std::string rv = *next_token;

    next_token++;

    if (strip_mode)
    {
	rv = Util::strip(rv);
    }

    return rv;
}

std::string StringTokenizer::getRemaining()
	throw(NoMoreTokensException)
{
    if (num_remaining <= 0)
	    throw NoMoreTokensException();

    std::string rv = getNext();

    while (num_remaining > 0) {
	rv += separator + getNext();
    }

    return rv;
}


StringTokenizer::NoMoreTokensException::NoMoreTokensException()
  :  Exception("No More Tokens!")
{
}

XCDR_NAMESPACE_END
