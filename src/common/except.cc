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
// $Source: /usr/local/cvs/xcdr/src/common/except.cc,v $
// $Revision: 1.5 $
// $Date: 2003/04/08 12:59:59 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: except.cc,v $
// Revision 1.5  2003/04/08 12:59:59  tmarsh
// added UnimplementedException
//
// Revision 1.4  2003/04/02 20:35:51  tmarsh
// added XCDR namespacing; eradicated all using namespace std
//
// Revision 1.3  2003/03/09 08:26:12  tmarsh
// include file reorganization
//
// Revision 1.2  2003/03/06 11:20:02  tmarsh
// updated headers
//
//
// =========================================================================


#include "common/common.h"
#include "common/except.h"
#include "common/util.h"

XCDR_NAMESPACE_BEGIN

//
// Exception
// ---------------------------------------------------------------------------

Exception::Exception(const std::string& err_msg)
  : msg(err_msg)
{
}


void Exception::print(std::ostream& o) const
{
    o << msg;
}

std::ostream& operator << (std::ostream& o, const Exception& e)
{
    e.print(o);

    return o;
}

//
// UnimplementedException
// ---------------------------------------------------------------------------

UnimplementedException::UnimplementedException(const char *file,
					       unsigned int line,
					       const char *function)
	: Exception("Unimplemented: " +
		    std::string(file) + ":" +
		    Util::itoa(line) + ": " +
		    std::string(function))
{
}

XCDR_NAMESPACE_END
