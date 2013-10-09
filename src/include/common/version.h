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
// $Source: /usr/local/cvs/xcdr/src/include/common/version.h.in,v $
// $Revision: 1.1 $
// $Date: 2003/04/02 20:34:57 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: version.h.in,v $
// Revision 1.1  2003/04/02 20:34:57  tmarsh
// added
//
//
// =========================================================================

#ifndef XCDR_COMMON_VERSION_H
#define XCDR_COMMON_VERSION_H


// -------------------------------------------------------------------------
//
// Version information
//
// -------------------------------------------------------------------------

#define XCDR_VERSION_MAJOR	0
#define XCDR_VERSION_MINOR	5
#define XCDR_VERSION_PATCHLEVEL	1

#define XCDR_VERSION_STRING	"0.5.1"

#define XCDR_VERSION_ID		(XCDR_VERSION_MAJOR * 10000 + \
				 XCDR_VERSION_MINOR * 100 + \
				 XCDR_VERSION_PATCHLEVEL)


// -------------------------------------------------------------------------
//
// Build information
//
// -------------------------------------------------------------------------

#define XCDR_BUILD_COMPILER	"g++ (GCC) 3.3"
#define XCDR_BUILD_FLAGS	"-Wall -g -O3 -pipe -march=athlon-xp -falign-functions=4 -fomit-frame-pointer -mfancy-math-387"
#define XCDR_BUILD_HOST		"r3f168.mistral.cz"

#define XCDR_BUILD_ARCH		"i686"
#define XCDR_BUILD_PROCESSOR	"athlon"

#define XCDR_BUILD_OS		"GNU/Linux"
#define XCDR_BUILD_KERNEL_REL	"2.4.18-14"
#define XCDR_BUILD_KERNEL_VER	"#1 Wed Sep 4 12:13:11 EDT 2002"

// -------------------------------------------------------------------------
//
// Function to return formatted version/build information
//
// -------------------------------------------------------------------------

#include "common/common.h"
#include <string>

XCDR_NAMESPACE_BEGIN

std::string XCDRBuildInformation();

XCDR_NAMESPACE_END

#endif // XCDR_COMMON_VERSION_H
