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
// $Source: /usr/local/cvs/xcdr/src/include/rater/response.h,v $
// $Revision: 1.4 $
// $Date: 2003/04/04 16:03:58 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: response.h,v $
// Revision 1.4  2003/04/04 16:03:58  tmarsh
// namespace changes
//
// Revision 1.3  2003/03/28 17:46:24  tmarsh
// major updates; rater now works
//
// Revision 1.2  2003/03/27 19:15:22  tmarsh
// rater rewrite
//
//
// =========================================================================

#ifndef XCDR_RATER_RESPONSE_H
#define XCDR_RATER_RESPONSE_H

//#include <unicode/gregocal.h>
#include "rater/party.h"
#include "rater/activity.h"

XCDR_NAMESPACE_BEGIN

struct DestinationInfo
{
	bool is_nanp;
	bool valid;

	char *country;
	char *province;
	char *city;
	char *timezone;
	char *lata;	// NANP only
	char *ocn;	// NANP only

	DestinationInfo()
	{
		is_nanp = false;
		valid = false;
		country = 0;
		province = 0;
		city = 0;
		timezone = 0;
		lata = 0;
		ocn = 0;
	}
};

struct Response
{
	// External

	double charged_amount;
	unsigned int charged_time;

//	UDate localtime_delta;

	DestinationInfo cli_info;
	DestinationInfo cld_info;

	
	// DB binding
	
	unsigned int i_customer;
	unsigned int i_vendor;
	unsigned int i_tariff;
	unsigned int i_activity;
	unsigned int i_period;
	unsigned int i_rate;
	unsigned int i_bracket;
	unsigned int i_bracket_index;
	unsigned int i_interval;

	// Private
	
//	BillingParty	*party;
//	Tariff		*tariff;
//	Activity	*activity;
//	Period		*period;
//	Bracket		*bracket;
//	Interval	*interval;
//	IntervalList	*interval_list;
	bool		has_nanp;

	// Methods

	Response()
	{
		charged_amount = 0;
		charged_time = 0;
//		localtime_delta = 0;
		i_customer = 0;
		i_vendor = 0;
		i_tariff = 0;
		i_rate = 0;
		i_activity = 0;
		i_period = 0;
		i_bracket = 0;
		i_bracket_index = 0;
		i_interval = 0;
		has_nanp = false;
	}
//	~Response();
};

XCDR_NAMESPACE_END

#endif // XCDR_RATER_RESPONSE_H
