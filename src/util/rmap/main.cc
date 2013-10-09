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
// $Source: /usr/local/cvs/xcdr/src/util/rmap/main.cc,v $
// $Revision: 1.2 $
// $Date: 2003/05/05 17:20:31 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: main.cc,v $
// Revision 1.2  2003/05/05 17:20:31  tmarsh
// misc updates
//
// Revision 1.1  2003/04/08 14:30:53  tmarsh
// added
//
// =========================================================================

#include "lerg.h"

using namespace XCDR;



void go()
{
	LERGLoader loader("lerg.txt", "./loc_info.xml");

	INFO("process: COUNTRY");
	loader.process(LERGLoader::COUNTRY, "out/country.txt");

	INFO("process: PROVINCE");
	loader.process(LERGLoader::PROVINCE, "out/province.txt");

	INFO("process: TIMEZONE");
	loader.process(LERGLoader::TIMEZONE, "out/timezone.txt");

	INFO("process: OCN");
	loader.process(LERGLoader::OCN, "out/ocn.txt");

	INFO("process: LATA");
	loader.process(LERGLoader::LATA, "out/lata.txt");

	INFO("processing complete");
}

int main()
{
//	Log::filter |= Log::Trace;
	Log::filter |= Log::Warn;
	go();
}
