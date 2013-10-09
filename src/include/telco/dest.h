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
// $Source: /usr/local/cvs/xcdr/src/include/telco/dest.h,v $
// $Revision: 1.1 $
// $Date: 2003/03/09 08:35:32 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: dest.h,v $
// Revision 1.1  2003/03/09 08:35:32  tmarsh
// include file reorganization
//
// Revision 1.2  2003/03/06 11:20:03  tmarsh
// updated headers
//
//
// =========================================================================

#ifndef XCDR_TELCO_DEST_H
#define XCDR_TELCO_DEST_H

#include <string>

struct DestinationFlagType
{
	unsigned int	i_type;
	std::string	name;
};

struct DestinationFlag
{
	unsigned int	i_flag;
	unsigned int	i_type;
	std::string	value;
};


struct DestinationFlagMap
{
	unsigned int	i_dest;
	unsigned int	i_flag;
};

struct Destination
{
	unsigned int	i_dest;
	std::string	destination;
};

struct CallDestinationFlags
{
	unsigned int	ordinal; // 0 = CLI, 1 = CLD
	std::string	i_call;
	unsigned int	i_flag;
};

#endif // XCDR_TELCO_DEST_H
