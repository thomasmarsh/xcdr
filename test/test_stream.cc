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
// $Source: /usr/local/cvs/xcdr/test/test_stream.cc,v $
// $Revision: 1.4 $
// $Date: 2003/04/04 16:07:10 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: test_stream.cc,v $
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

#include "common/common.h"
#include "common/timer.h"
#include "common/filestream.h"
#include "common/logging.h"

using namespace std;
using namespace XCDR;

int main(int argc, char** argv)
{
//    Debug::debug = true;
    assert(sizeof(char) == 1);
    assert(sizeof(unsigned int) == sizeof(char*));

    int poopy_count = 0;

    if (argc != 3)
    {
	cerr << "usage: test_stream <infile>[:<pos>] <outfile>" << endl << flush;
	exit(1);
    }

    bool idle = false;
    try
    {
	string fin(argv[1]);
	string::size_type c_pos = fin.rfind(':');

	off_t init_pos = 0;
	if (c_pos != string::npos)
	{
	    string pos = fin.substr(c_pos+1, fin.length()-c_pos+1);
	    init_pos = atoi(pos.c_str());
	    fin = fin.substr(0, c_pos);
	}

	FileStream stream(fin, init_pos);
	Timer timer;

	FILE *fp = NULL;
	if ((strlen(argv[2]) == 1) && (argv[2][0] == '-'))
	    fp = stdout;
	else
	    fp = fopen(argv[2], "w");

        pair<long,string> line;
        while (1)
	{
	    line = stream.readline();

	    if (line.second != "")
	    {
		if (idle)
		{
		    idle = false;
		    cerr << "NEW INPUT (idle " << timer.elapsedString() << ")"
			 << endl << flush;
		}
	        fprintf(fp, "%s\n", line.second.c_str());
		fflush(fp);
		//cerr << line.first << endl;
	    }
	    else
	    {
	       if (!idle)
	       {
		   timer.reset();
	           cerr << "IDLE" << endl << flush;
		   if (poopy_count > 3)
		   {
		   	break;
		   }
		   poopy_count++;
	       }
	       idle = true;
	       sleep(1);
            }
	}
    }
    catch (Exception& e)
    {
        DEBUG("FATAL: >>" << e << "<<");
    }
    exit(-1);
}
