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
// $Source: /usr/local/cvs/xcdr/test/test_threshold.cc,v $
// $Revision: 1.4 $
// $Date: 2003/04/04 16:07:10 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: test_threshold.cc,v $
// Revision 1.4  2003/04/04 16:07:10  tmarsh
// namespace changes
//
// Revision 1.3  2003/03/09 08:56:48  tmarsh
// include file reorganization
//
// Revision 1.2  2003/03/06 11:20:05  tmarsh
// updated headers
//
//
// =========================================================================

#include "common/threshold.h"

#include <iostream>
#include <unistd.h>

using namespace std;
using namespace XCDR;

int main()
{
	cout << "testing 1 second threshold with wait()" << endl << flush;
	Threshold t1(1);

	t1.wait();


	cout << "testing 1 second threshold with reached()" << endl << flush;

	Threshold t2(1);

	unsigned int i = 0;
	while (!t2.reached())
	{
		i++;
	}
	cout << "\t" << i << " iterations" << endl << flush;

	cout << "testing counter to 1000 iterations (usleep(1))" << endl << flush;
	i = 0;

	Timer t;
	Threshold t3(1000, &i);

	while (!t3.reached())
	{
		usleep(1);
		i++;
	}

	cout << "\t" << t.elapsed() << " seconds" << endl << flush;

	cout << "testing 2 second counter to 1000000 iterations" << endl << flush;

	i = 0;

	Threshold t4(Threshold::T_OR, 1000000, &i, 2);

	while (!t4.reached())
	{
		usleep(1);
		i++;
	}

	cout << "\t" << i << " iterations" << endl << flush;

	cout << "reseting threshold and testing previous again..." << endl << flush;
	i = 0;

	t4.reset();

	while (!t4.reached())
	{
		usleep(1);
		i++;
	}

	cout << "\t" << i << " iterations" << endl << flush;
}

