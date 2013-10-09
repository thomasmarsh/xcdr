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
// $Source: /usr/local/cvs/xcdr/src/common/threshold.cc,v $
// $Revision: 1.6 $
// $Date: 2003/04/04 16:06:36 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: threshold.cc,v $
// Revision 1.6  2003/04/04 16:06:36  tmarsh
// namespace changes
//
// Revision 1.5  2003/04/02 20:35:51  tmarsh
// added XCDR namespacing; eradicated all using namespace std
//
// Revision 1.4  2003/03/09 08:26:12  tmarsh
// include file reorganization
//
// Revision 1.3  2003/03/06 11:20:02  tmarsh
// updated headers
//
//
// =========================================================================

#include "common/threshold.h"

#include <unistd.h>
#include <iostream>

XCDR_NAMESPACE_BEGIN

//
// Constructors and baseInit()
//

Threshold::Threshold(unsigned int maxc,
		     unsigned int *cnt)
{
	baseInit();
	update(maxc, cnt);
}

Threshold::Threshold(unsigned int maxt)
{
	baseInit();
	update(maxt);
}

Threshold::Threshold(unsigned int flags,
		     unsigned int maxc,
		     unsigned int *cnt,
		     unsigned int maxt)
{
	baseInit();
	update(flags, maxc, cnt, maxt);
}


void Threshold::baseInit()
{
	timer = NULL;
	counter = 0;

	max_t = 0;
	max_c = 0;

	mode_timer = false;
	mode_counter = false;
	mode_test = 0;
}


//
// update functions
//

void Threshold::update(unsigned int maxc,
		       unsigned int *cnt)
{
	mode_counter = true;
	counter = cnt;
	max_c = maxc;
}


void Threshold::update(unsigned int maxt)
{
	if (!timer)
		timer = new Timer();

	mode_timer = true;
	max_t = maxt;

	timer->reset();
}

void Threshold::update(unsigned int flags,
		      unsigned int maxc,
		      unsigned int *cnt,
		      unsigned int maxt)
{
	assert((flags == T_AND) || (flags == T_OR));

	mode_test = flags;

	update(maxt);
	update(maxc, cnt);
}

//
// Destructor
//


Threshold::~Threshold()
{
	if (mode_timer && (timer != NULL))
	{
		delete timer;
	}
}

void Threshold::reset()
{
	if (mode_timer)
	{
		timer->reset();
	}
}

void Threshold::wait()
{
	assert(mode_timer);
	assert(!reached());


	sleep(max_t - (timer->elapsed()));
}

bool Threshold::counterReached()
{
	assert(mode_counter);

	//cout << *counter  << ">=" << max_c << endl << flush;
	return ((*counter) >= max_c);
}

bool Threshold::timerReached()
{
	assert(mode_timer);

	//cout << timer->elapsed() << ">=" << max_t << endl << flush;
	return ((timer->elapsed()) >= (time_t) (max_t));
}


bool Threshold::reached()
{
	if (mode_timer && mode_counter)
	{
		if (mode_test == T_AND)
		{
			return (timerReached() && counterReached());
		}
		else if (mode_test == T_OR)
		{
			return (timerReached() || counterReached());
		}
		else
		{
			assert(1==0);
		}
	}
	else if (mode_timer)
	{
		return timerReached();
	}
	else if (mode_counter)
	{
		return counterReached();
	}
	
	assert(1==0);
}

XCDR_NAMESPACE_END
