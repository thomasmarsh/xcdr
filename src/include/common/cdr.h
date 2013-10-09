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
// $Source: /usr/local/cvs/xcdr/src/include/common/cdr.h,v $
// $Revision: 1.2 $
// $Date: 2003/04/02 20:34:25 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: cdr.h,v $
// Revision 1.2  2003/04/02 20:34:25  tmarsh
// added XCDR namespace
//
// Revision 1.1  2003/03/09 08:35:31  tmarsh
// include file reorganization
//
// Revision 1.7  2003/03/06 11:20:02  tmarsh
// updated headers
//
//
// =========================================================================

#ifndef XCDR_CDR_H
#define XCDR_CDR_H

#include "common/common.h"
#include "common/datetime.h"
#include "telco/dest.h"

#include <string>
#include <list>

XCDR_NAMESPACE_BEGIN

namespace CDR
{
	const unsigned int TYPE_UNKNOWN		= 0;
	const unsigned int TYPE_VERSION		= 1;
	const unsigned int TYPE_REBOOT		= 2;
	const unsigned int TYPE_SWITCHOVER	= 3;
	const unsigned int TYPE_START		= 4;
	const unsigned int TYPE_STOP		= 5;
	const unsigned int TYPE_ATTEMPT		= 6;
	const unsigned int TYPE_INTERMEDIATE	= 7;

	//
	// The Mediated record is the output from xcdr_mediator
	//

	struct Mediated
	{
		//
		// The following fields match the destination table
		// named CDR_Mediated. The source for the information
		// is given in the comments following the field with the
		// following designations:
		//
		// 	XCDR	The xcdr_mediator global Mediator class
		// 	GW	The Gateway::Mediator instance (e.g., Sonus)
		//
		// If a map is specified, this refers to the cache being
		// used.
		//

		std::string	i_call;		// from XCDR
		std::string	call_id;	// from GW

		unsigned int	i_type_node;	// from XCDR::node_map

		unsigned int	i_ingress;	// from XCDR::target_map
		unsigned int	i_egress;	// from XCDR::target_map

		DateTime	start_time;	// from GW
		DateTime	stop_time;	// from GW

		DateTime	start_time_local; // from XCDR::timezone_map
		DateTime	stop_time_local;  // from XCDR::timezone_map

		unsigned int	setup_seconds;	  // from GW
		unsigned int	elapsed_seconds;  // from GW

		unsigned int	bytes_sent;	// from GW
		unsigned int	bytes_rcvd;	// from GW

		std::string	cli;		// from GW
		std::string	cld;		// from GW

		unsigned int	i_dest_cli;	// from XCDR::destination_map
		unsigned int	i_dest_cld;	// from XCDR::destination_map

		unsigned int	i_customer;	// from XCDR::customer_map

		bool		payphone;	// from GW::Mediator
		bool		prison;		// from GW::Mediator

		bool		billable;	// set by GW and XCDR

		//
		// The following fields are buffers for information
		// that will be used by xcdr_mediator, to be populated
		// by the Gateway::Mediator instance. The XCDR mediator
		// will try each of the items in the list until it finds
		// a match within its target_map and set i_ingress and
		// i_egress accordingly.
		//

		std::list<std::string>	_node_ingress;
		std::list<std::string>	_node_egress;

		std::list<DestinationFlag> cli_flags;
		std::list<DestinationFlag> cld_flags;

		bool		_bad_start_time;
		bool		_bad_stop_time;

		unsigned int	_i_timezone;

		//
		// toInsert creates a SQL fragment used to be appended
		// to a statement in the format
		//
		// 	INSERT INTO TABLE VALUES <toInsert() output>
		//
		// or alternately:
		//
		// 	INSERT INTO TABLE VALUES
		// 	  <toInsert()[0]>,
		// 	  <toInsert()[1]>,
		// 	  ...
		// 	  <toInsert()[n]>
		//
		// which the Mediator does in its batches.
		//

		inline std::string toInsert()
		{
			std::string i = "(";
			i += "\"" + i_call  + "\", ";
			i += "\"" + call_id + "\", ";
			i += Util::itoa(i_type_node) + ", ";
			i += Util::itoa(i_ingress) + ", ";
			i += Util::itoa(i_egress) + ", ";
			i += "\"" + start_time.toString() + "\", ";
			i += "\"" + stop_time.toString() + "\", ";
			i += "\"" + start_time_local.toString() + "\", ";
			i += "\"" + stop_time_local.toString() + "\", ";
			i += Util::itoa(setup_seconds) + ", ";
			i += Util::itoa(elapsed_seconds) + ", ";
			i += Util::itoa(bytes_sent) + ", ";
			i += Util::itoa(bytes_rcvd) + ", ";
			i += "\"" + cli + "\", ";
			i += "\"" + cld + "\", ";
			i += Util::itoa(i_dest_cli) + ", ";
			i += Util::itoa(i_dest_cld) + ", ";
			i += Util::itoa(i_customer) + ", ";
			i += payphone ? "1, " : "0, ";
			i += prison ? "1, " : "0, ";
			i += billable ? "1)" : "0)";
			return i;
		}
	};
}

XCDR_NAMESPACE_END

#endif // XCDR_CDR_H
