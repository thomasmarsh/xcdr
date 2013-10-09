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
// $Source: /usr/local/cvs/xcdr/src/include/common/activity.h,v $
// $Revision: 1.2 $
// $Date: 2003/04/02 20:34:25 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: activity.h,v $
// Revision 1.2  2003/04/02 20:34:25  tmarsh
// added XCDR namespace
//
// Revision 1.1  2003/03/09 08:35:31  tmarsh
// include file reorganization
//
//
// =========================================================================

#ifndef XCDR_ACTIVITY_H
#define XCDR_ACTIVITY_H

#include "common/common.h"

#include <string>
#include <cstdio>

XCDR_NAMESPACE_BEGIN

typedef std::string ActivityID;

ActivityID generateActivity(unsigned int i_reader,
			    unsigned int i_mediator,
			    unsigned int count)
{
	std::string s;
	char buf16[5];
	char buf32[9];

	std::snprintf(buf16, 5, "%04x", i_reader);
	s += buf16;
	std::snprintf(buf16, 5, "%04x", i_mediator);
	s += buf16;
	std::snprintf(buf32, 9, "%08x", count);
	s += buf32;
	return s;
}

XCDR_NAMESPACE_END

#endif // XCDR_ACTIVITY_H
