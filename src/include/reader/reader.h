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
// $Source: /usr/local/cvs/xcdr/src/include/reader/reader.h,v $
// $Revision: 1.4 $
// $Date: 2003/05/05 17:20:31 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: reader.h,v $
// Revision 1.4  2003/05/05 17:20:31  tmarsh
// misc updates
//
// Revision 1.3  2003/04/04 16:03:58  tmarsh
// namespace changes
//
// Revision 1.2  2003/03/09 08:44:12  tmarsh
// include file reorganization
//
// Revision 1.1  2003/03/09 08:35:32  tmarsh
// include file reorganization
//
// Revision 1.7  2003/03/06 11:20:03  tmarsh
// updated headers
//
//
// =========================================================================

#ifndef XCDR_READER_H
#define XCDR_READER_H

// --------------------------------------------------------------------------
//
// CLASS: AbstractReader
//
// DESCRIPTION:
//
//    The AbstractReader is the base class for CDRReader, RADIUSReader, and
//    CDRServerReader. It defines the interface which all readers must
//    conform to.
//
//    The responsibilities of the readers are to collect incoming records
//    on the fly, extract the timestamp and call id, and dump the result
//    in the RawCDR table in the local database. These records will be
//    later picked up by their mediators.
//
//    A reader is instantiated with a reader ID (i_reader in the database).
//    The AbstractReader will populate the common details. The subclasses
//    are responsible for retrieving any implementation specific details.
//
// --------------------------------------------------------------------------

#include "reader/cache/nodemap.h"
#include "entity/entity.h"
#include "common/stats.h"

#include <string>

XCDR_NAMESPACE_BEGIN

enum ReaderType
{
	READER_MVAM,
	READER_SONUS,
	READER_DCO,
	READER_QUINTUM,
	READER_CISCO
};

// --------------------------------------------------------------------------
//
// BASE READER CLASS
//
// --------------------------------------------------------------------------

class ReaderBase : public Entity
{
public:
protected:
	ReaderBase(unsigned int id)
		: Entity(id)
	{
	}

	//
	// Contractual requirements from Entity
	//

	std::string getName() const { return "reader"; }

	EntityType::EntityType getTypeID() const { return EntityType::READER_TYPE; }

	//
	// Variables used by all readers
	//

	NodeMap::NodeMap *node_map;
};

XCDR_NAMESPACE_END

#endif // XCDR_READER_H
