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
// $Source: /usr/local/cvs/xcdr/src/include/mediator/mediator.h,v $
// $Revision: 1.4 $
// $Date: 2003/05/05 17:20:31 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: mediator.h,v $
// Revision 1.4  2003/05/05 17:20:31  tmarsh
// misc updates
//
// Revision 1.3  2003/04/04 16:03:58  tmarsh
// namespace changes
//
// Revision 1.2  2003/03/09 08:50:05  tmarsh
// include file reorganization
//
// Revision 1.1  2003/03/09 08:35:32  tmarsh
// include file reorganization
//
// Revision 1.11  2003/03/06 11:20:03  tmarsh
// updated headers
//
//
// =========================================================================

#ifndef XCDR_MEDIATOR_H
#define XCDR_MEDIATOR_H

#include "entity/entity.h"
#include "gateway/sonus/sonus.h"
#include "mediator/cache/nodemap.h"

XCDR_NAMESPACE_BEGIN

class Mediator : public Entity
{
public:
	Mediator(const unsigned int id)
		: Entity(id)
	{
		mediator_id = 0;
		count = 0;
	}

	std::string getName() const { return "mediator"; }

	EntityType::EntityType getTypeID() const { return EntityType::MEDIATOR_TYPE; }

	void initialize();
	void shutdown() {};
	void loadProfile() {};
	void execute();
	void wait() const;

	bool haveSonusData();
	void mediate();
	void localMediation(CDR::Mediated&);

	void checkShutdownCalled();

private:
	Gateway::CallList separateCalls(RDBMS::ResultStore &r);
	NodeTypeMap *node_map;
	Sonus::Mediator sonus_mediator;
	unsigned int mediator_id;
	unsigned int count;

	std::ifstream src_file;
};

XCDR_NAMESPACE_END

#endif // XCDR_MEDIATOR_H
