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
// $Source: /usr/local/cvs/xcdr/src/engine/engine.cc,v $
// $Revision: 1.5 $
// $Date: 2003/04/04 16:06:29 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: engine.cc,v $
// Revision 1.5  2003/04/04 16:06:29  tmarsh
// namespace changes
//
// Revision 1.4  2003/03/27 19:18:10  tmarsh
// added .run()
//
// Revision 1.3  2003/03/14 17:56:07  tmarsh
// use new rater; fixes to compile
//
// Revision 1.2  2003/03/06 11:20:02  tmarsh
// updated headers
//
// =========================================================================

#include "engine/engine.h"

XCDR_NAMESPACE_BEGIN

std::string Engine::getName() const
{
	return std::string("engine");
}

Engine::Engine(unsigned int id) : Entity(id)
{
	ENTER();

	i_engine = id;

	LEAVE();
}

XCDR_NAMESPACE_END

int main()
{
	XCDR::Engine e(1);
	e.run();
}
