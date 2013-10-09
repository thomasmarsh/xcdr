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
// $Source: /usr/local/cvs/xcdr/src/include/gateway/sonus/sonus_records.h,v $
// $Revision: 1.2 $
// $Date: 2003/04/04 16:03:58 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: sonus_records.h,v $
// Revision 1.2  2003/04/04 16:03:58  tmarsh
// namespace changes
//
// Revision 1.1  2003/03/09 08:35:32  tmarsh
// include file reorganization
//
// Revision 1.2  2003/03/06 11:20:03  tmarsh
// updated headers
//
//
// =========================================================================

#ifndef XCDR_SONUS_RECORDS_H
#define XCDR_SONUS_RECORDS_H

#include "common/common.h"

XCDR_NAMESPACE_BEGIN

namespace Sonus
{
	namespace Reboot
	{
		enum FieldIndex
		{
			record_type = 0,
			gateway_name,
			call_id,
			time_zone,
			boot_date,
			boot_time
		};
	}

	namespace Attempt
	{
		enum FieldIndex
		{
        		record_type = 0,
        		gateway_name,
        		call_id,
        		start_time_ticks,
        		time_zone,
        		start_date,
        		start_time,
        		elapsed_setup_response,
        		elapsed_setup_alert,
        		disconnect_time,
        		elapsed_disconnect_term,
        		disconnect_reason,
        		service,
        		direction,
        		provider,
        		transit_network_selection_code,
        		calling_number,
        		called_number,
        		extra_called_address_digits,
        		num_translations,
        		orig_called_number_1,
        		translation_type_1,
        		orig_called_number_2,
        		translation_type_2,
        		billing_number,
        		route_table_name,
        		route_attempt_number,
        		route_selected,
        		local_gateway_signaling_ip,
        		remote_gateway_signaling_ip,
        		ingress_pstn_trunk_name,
        		ingress_pstn_circuit_ep,
        		ingress_ip_circuit_ep,
        		egress_pstn_circuit_ep,
        		egress_ip_circuit_ep,
        		orig_line_info,
        		jurisdiction_info_param,
        		carrier_code,
        		call_group_id,
        		script_log_data,
        		elapsed_time_5,
        		elapsed_time_6
		};
	}

	namespace Start
	{
		enum FieldIndex
		{
			record_type = 0,
		        gateway_name,
			call_id,
			start_time_ticks,
			time_zone,
			start_date,
			start_time,
			elapsed_setup_response,
			elapsed_setup_established,
			service,
			direction,
			provider,
			transit_network_selection_code,
			calling_number,
			called_number,
			extra_called_address_digits,
			num_translations,
			orig_called_number_1,
			translation_type_1,
			orig_called_number_2,
			translation_type_2,
			billing_number,
			route_table_name,
			route_attempt_number,
			route_selected,
			local_gateway_signaling_ip,
			remote_gateway_signaling_ip,
			ingress_pstn_trunk_name,
			ingress_pstn_circuit_ep,
			ingress_ip_circuit_ep,
			egress_pstn_circuit_ep,
			egress_ip_circuit_ep,
			orig_line_info,
			jurisdiction_info_param,
			carrier_code
		};
	}

	namespace Stop
	{
		enum FieldIndex
		{
			record_type = 0,
			gateway_name,
			call_id,
			start_time_clicks,
			time_zone,
			start_date,
			start_time,
			elapsed_setup_response,
			elapsed_setup_alert,
			elapsed_setup_established,
			disconnect_date,
			disconnect_time,
		        elapsed_disconnect_term,
		        service_duration,
		        disconnect_reason,
		        service,
		        direction,
		        provider,
		        transit_network_selection_code,
		        calling_number,
		        called_number,
		        extra_called_address_digits,
		        num_translations,
		        orig_called_number_1,
		        translation_type_1,
		        orig_called_number_2,
		        translation_type_2,
		        billing_number,
		        route_table_name,
		        route_attempt_number,
		        route_selected,
		        local_gateway_signaling_ip,
		        remote_gateway_signaling_ip,
		        ingress_pstn_trunk_name,
		        ingress_pstn_circuit_ep,
		        ingress_ip_circuit_ep,
		        egress_pstn_circuit_ep,
		        egress_ip_circuit_ep,
		        bytes_sent,
		        packets_sent,
		        bytes_rcvd,
		        packets_rcvd,
		        orig_line_info,
		        jurisdiction_info_param,
		        carrier_code,
		        call_group_id,
		        script_log_data,
		        time_elapsed_receipt_setup_to_exit,
		        time_elapsed_generation_setup_to_exit,
		        calling_party_nature,
		        called_party_nature
		};
	};
};

XCDR_NAMESPACE_END

#endif // XCDR_SONUS_RECORDS_H
