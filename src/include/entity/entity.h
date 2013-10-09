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
// $Source: /usr/local/cvs/xcdr/src/include/common/entity.h,v $
// $Revision: 1.3 $
// $Date: 2003/04/02 20:34:25 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: entity.h,v $
// Revision 1.3  2003/04/02 20:34:25  tmarsh
// added XCDR namespace
//
// Revision 1.2  2003/03/09 12:07:59  tmarsh
// more header file reorg changes
//
// Revision 1.1  2003/03/09 08:35:31  tmarsh
// include file reorganization
//
// Revision 1.4  2003/03/07 18:07:15  tmarsh
// use new getcontext method
//
// Revision 1.3  2003/03/06 11:20:02  tmarsh
// updated headers
//
//
// =========================================================================


#ifndef XCDR_ENTITY_H
#define XCDR_ENTITY_H

#include "common/common.h"
#include "common/etypes.h"
#include "common/pidfile.h"
#include "common/stats.h"
#include "rdbms/rdbms.h"

#include <string>

XCDR_NAMESPACE_BEGIN

// --------------------------------------------------------------------------
//
// CLASS: Entity
//
// This is the parent class for Mediator, Reader, etc. It provides a contract
// that every concrete implementation must provide the following methods:
//
// 	- getTypeID()	returns a unique id for the type
// 	- initialize()	set up any data specific to the implementation
// 	- execute()	run repeatedly from mainloop
// 	- wait()	if implementation calls setIdle(true), maybe use this
// 	- shutdown()	perform any cleanup
// 	- loadProfile()	load configuration from the database
// 	- getName()	return a nice name (e.g., "reader")
//
// Each entity gets an RDBMS::Context, log files, and a PID lock file.
//
// --------------------------------------------------------------------------

class Entity
{
public:
	//
	// Return the id for this instance
	//

	unsigned int getID() const;


	//
	// Return the type id (e.g., 1=reader, 2=mediator)
	//

	virtual EntityType::EntityType getTypeID() const;


	//
	// This is the mainloop; usage for clients of entities should
	// perform the following:
	//
	//     e = EntityImpl(1)
	//     e.run();
	//

	void run();

protected:
	//
	// Constructor is protected; don't instantiate directly
	//

	Entity(unsigned int id);

	//
	// Pure virtual methods to be implemented by client
	//

	virtual void initialize() = 0;
	virtual void shutdown() = 0;
	virtual void execute() = 0;
	virtual void loadProfile() = 0;
	virtual std::string getName() const = 0;

	//
	// Wait has a default fuctionality of sleeping one second
	//

	virtual void wait() const;


	//
	// This is the setIdle function. The implementation should
	// use this in its execute() method.
	//

	void setIdle(bool value);

	//
	// You can query if you're idle with this.
	//

	bool isIdle() const;

	virtual ~Entity();

	RDBMS::Context *getContext() const;

	Statistics::Manager *getStats() { return stats; }

private:

	//
	// Performs intial configuration of the entity before calling
	// initialize()
	//

	void setupEntity();

	//
	// This is the internal holder for the entity_id
	//

	unsigned int entity_id;


	//
	// Various internal booleans
	//

	bool shutdown_called;
	bool is_idle;


	//
	// Our RDBMS context
	//

	RDBMS::Context *context;

	//
	// Our PIDFile
	//
	
	PIDFile pid_lock;

	//
	// Our Statistics module
	//

	Statistics::Manager *stats;
};

XCDR_NAMESPACE_END

#endif // XCDR_ENTITY_H
