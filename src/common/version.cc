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
// $Source: /usr/local/cvs/xcdr/src/common/version.cc,v $
// $Revision: 1.2 $
// $Date: 2003/04/04 16:06:36 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: version.cc,v $
// Revision 1.2  2003/04/04 16:06:36  tmarsh
// namespace changes
//
// Revision 1.1  2003/04/02 20:36:10  tmarsh
// added
//
//
// =========================================================================

#include "common/version.h"

#include <sstream>
#include <iostream>

XCDR_NAMESPACE_BEGIN

std::string XCDRBuildInformation()
{
	std::stringstream s;
	const char *endl = "\n";

	s << "XCDR Version:  " << XCDR_VERSION_STRING << endl
	  << "VERSIONID:     " << XCDR_VERSION_ID << endl
	  << "Build Date:    " << __DATE__ << " " << __TIME__ << endl
	  << "Compiler:      " << XCDR_BUILD_COMPILER << endl
	  << "GCC Version:   " << __GNUC__ << "."
	                       << __GNUC_MINOR__ << "."
			       << __GNUC_PATCHLEVEL__ << endl
	  << "Compile Flags: " << XCDR_BUILD_FLAGS << endl
	  << "Build Host:    " << XCDR_BUILD_HOST << endl
	  << "OS:            " << XCDR_BUILD_OS << endl
	  << "Kernel Rel:    " << XCDR_BUILD_KERNEL_REL << endl
	  << "Kernel Ver:    " << XCDR_BUILD_KERNEL_VER << endl
	  << "Architecture:  " << XCDR_BUILD_ARCH << endl
	  << "Processor:     " << XCDR_BUILD_PROCESSOR << endl;

	return s.str();
}

XCDR_NAMESPACE_END
