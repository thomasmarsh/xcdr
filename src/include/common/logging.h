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
// $Source: /usr/local/cvs/xcdr/src/include/common/logging.h,v $
// $Revision: 1.4 $
// $Date: 2003/04/04 16:03:58 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: logging.h,v $
// Revision 1.4  2003/04/04 16:03:58  tmarsh
// namespace changes
//
// Revision 1.3  2003/04/02 20:34:25  tmarsh
// added XCDR namespace
//
// Revision 1.2  2003/03/19 00:02:29  tmarsh
// added timestamp suppression
//
// Revision 1.1  2003/03/09 08:35:31  tmarsh
// include file reorganization
//
// Revision 1.6  2003/03/06 11:20:02  tmarsh
// updated headers
//
//
// =========================================================================

#ifndef XCDR_LOGGING_H
#define XCDR_LOGGING_H

// --------------------------------------------------------------------------
// 
// LOCAL INCLUDES
//
// --------------------------------------------------------------------------

#include "common/common.h"
#include "common/except.h"

// --------------------------------------------------------------------------
// 
// SYSTEM INCLUDES
//
// --------------------------------------------------------------------------

#include <string>
#include <fstream>
#include <iostream>
#include <map>

extern "C" {
#include <values.h>
#include <stdlib.h>
}

#define DEBUG_BUILD

XCDR_NAMESPACE_BEGIN

namespace Log
{
	extern const unsigned int Trace;
	extern const unsigned int Debug;
	extern const unsigned int Info;
	extern const unsigned int Warn;
	extern const unsigned int Error;
	extern const unsigned int Fatal;
	extern const unsigned int Exception;
	extern const unsigned int Todo;

	extern std::map<std::string, bool> msg_seen;

	extern unsigned int trace_level;
	extern std::string path;
	extern unsigned int id;
	extern unsigned int filter;

	extern std::ofstream fp;

	extern bool _is_open;
	extern bool suppress_timestamp;

	void details();
	bool initialized();
	void open();
	void end();
	void begin();

	inline bool is_set(unsigned int flag)
	{
		return ((Log::filter & flag) == flag);
	}
}


#ifdef DEBUG_BUILD

#define TRACE(msg) \
	if (Log::is_set(Log::Trace)) \
	{ \
		Log::begin(); \
		(Log::_is_open ? Log::fp : std::cerr) << "TRACE: " \
		        << __FILE__ << ":" << __LINE__ << ": " \
		        << __PRETTY_FUNCTION__ << ": " << msg; \
		Log::end(); \
	}

#define ENTER() \
	if (Log::is_set(Log::Trace)) \
	{ \
		Log::begin(); \
		(Log::_is_open ? Log::fp : std::cerr) << "TRACE: ENTER: " \
		        << __FILE__ << ":" << __LINE__ << ": " \
		        << __PRETTY_FUNCTION__; \
		Log::end(); \
	}

#define LEAVE() \
	if (Log::is_set(Log::Trace)) \
	{ \
		Log::begin(); \
		(Log::_is_open ? Log::fp : std::cerr) << "TRACE: LEAVE: " \
		        << __FILE__ << ":" << __LINE__ << ": " \
		        << __PRETTY_FUNCTION__; \
		Log::end(); \
	}

#define RETURN(m) \
	if (Log::is_set(Log::Trace)) \
	{ \
		Log::begin(); \
		(Log::_is_open ? Log::fp : std::cerr) << "TRACE: RETURN VALUE: " \
		        << m << ": " << __FILE__ << ": " \
			<< __LINE__ << ": " << __PRETTY_FUNCTION__; \
		Log::end(); \
	}


#define TODO(m) \
	if ((Log::is_set(Log::Todo)) && (!Log::msg_seen[m])) \
	{ \
		Log::begin(); \
		(Log::_is_open ? Log::fp : std::cerr) << "XXX TODO: " \
		        << m \
		        << ": (" << __FILE__ << ":" \
			<< __LINE__ << ": " << __PRETTY_FUNCTION__ << ")"; \
		Log::end(); \
		Log::msg_seen[m] = true; \
	}

#else // !DEBUG_BUILD

#define TRACE(msg)
#define ENTER()
#define LEAVE()

#endif // DEBUG_BUILD

#define DEBUG(msg) \
	if (Log::is_set(Log::Debug)) \
	{ \
		Log::begin(); \
		(Log::_is_open ? Log::fp : std::cerr) << "DEBUG: " << msg ; \
		Log::end(); \
	}

#define INFO(msg) \
	if (Log::is_set(Log::Info)) \
	{ \
		Log::begin(); \
		(Log::_is_open ? Log::fp : std::cerr) << msg; \
		Log::end(); \
	}

#define WARN(msg) \
	if (Log::is_set(Log::Warn)) \
	{ \
		Log::begin(); \
		(Log::_is_open ? Log::fp : std::cerr) << "WARNING: " << msg ; \
		Log::end(); \
	}

#define ERROR(msg) \
	if (Log::is_set(Log::Error)) \
	{ \
		Log::begin(); \
		(Log::_is_open ? Log::fp : std::cerr) << "ERROR: " << msg ; \
		Log::end(); \
	}

#define FATAL(msg) \
	if (Log::is_set(Log::Fatal)) \
	{ \
		Log::begin(); \
		(Log::_is_open ? Log::fp : std::cerr) << "FATAL: " << msg ; \
		Log::end(); \
	}

#define EXCEPTION(m,e) \
	if (Log::is_set(Log::Debug)) \
	{ \
		Log::begin(); \
		(Log::_is_open ? Log::fp : std::cerr) << "EXCEPTION: " << m << std::endl \
	   	        << "\t\tDATA: " << e << std::endl; \
		Log::end(); \
	}


XCDR_NAMESPACE_END

using XCDR::Log;

#endif // XCDR_LOGGING_H
