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
// $Source: /usr/local/cvs/xcdr/src/rater/charge.cc,v $
// $Revision: 1.3 $
// $Date: 2003/04/04 16:06:36 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: charge.cc,v $
// Revision 1.3  2003/04/04 16:06:36  tmarsh
// namespace changes
//
// Revision 1.2  2003/03/27 19:16:25  tmarsh
// rater rewrite
//
//
// =========================================================================

#include "rater/rater.h"

XCDR_NAMESPACE_BEGIN

void Rater::charge()
{
//	TODO("payphone/prison surcharge");
//	TODO("flatrate");

	do
	{
		idur = interval->duration;
		if (duration > 0)
		{
			if (idur < duration)
				duration -= idur;
			else duration = 0;
			charged_time += idur;
			current_charge += interval->price;
		}
		else
			break;

		if (!il_current->next)
			break;

		il_current = il_current->next;
		interval = il_current->value;
	} while (true);

	if (duration > 0)
	{
		idur = interval->duration;
		remainder = duration % idur;
		if (remainder)
			duration += idur - remainder;


		charged_time += duration;
		current_charge += (interval->price * duration);
	}
	if (current_charge > 0)
	{
//		current_charge += response->connect_fee;

//		if (response->post_call_surcharge)
//			current_charge *= response->post_call_surcharge;
	}

	response->charged_time = charged_time;
	response->charged_amount = current_charge;
}

XCDR_NAMESPACE_END
