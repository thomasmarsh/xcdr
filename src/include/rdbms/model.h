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
// $Source: /usr/local/cvs/xcdr/src/include/rdbms/model.h,v $
// $Revision: 1.3 $
// $Date: 2003/04/04 16:03:58 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: model.h,v $
// Revision 1.3  2003/04/04 16:03:58  tmarsh
// namespace changes
//
// Revision 1.2  2003/03/09 11:42:41  tmarsh
// removed redundant include
//
// Revision 1.1  2003/03/09 08:35:32  tmarsh
// include file reorganization
//
// Revision 1.3  2003/03/07 17:23:07  tmarsh
// updates to compile
//
// Revision 1.2  2003/03/07 13:27:07  tmarsh
// fixes to compile
//
// Revision 1.1  2003/03/07 11:26:44  tmarsh
// added
//
//
// =========================================================================

#ifndef XCDR_RDBMS_MODEL_H
#define XCDR_RDBMS_MODEL_H

#include "common/common.h"

#include <list>
#include <map>
#include <string>

XCDR_NAMESPACE_BEGIN

namespace Model
{
	//
	// The following are components of the Model
	//

	struct Profile
	{
		std::string	name;
		std::string	type;
		std::string	externalType;

		bool		primaryKey;
		bool		null;
		bool		required;
		bool		unsign;
		bool		zeroFill;
		bool		binary;

		unsigned int	width;
		unsigned int	precision;
		unsigned int	scale;

		std::string	defaultValue;
		std::string	label;
	};

	struct Attribute : public Profile
	{
		std::string	name;
		std::string	profile;
	};

	typedef std::list<Attribute> AttribList;

	typedef std::list<std::list<std::string> > PopulateRecords;

	struct Entity
	{
		std::string	name;
		std::string	type;
		std::string	comment;

		AttribList	attributes;
		PopulateRecords	populate;
	};


	//
	// This is the model structure itself
	//

	struct Model
	{
		std::string	version;
		std::string	adaptor;

		std::map<std::string, Profile> typedefs;
		std::map<std::string, Entity> entities;
	};

}

XCDR_NAMESPACE_END

#endif // XCDR_RDBMS_MODEL_H
