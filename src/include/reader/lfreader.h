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
// $Source: /usr/local/cvs/xcdr/src/include/reader/lfreader.h,v $
// $Revision: 1.3 $
// $Date: 2003/04/04 16:03:58 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: lfreader.h,v $
// Revision 1.3  2003/04/04 16:03:58  tmarsh
// namespace changes
//
// Revision 1.2  2003/03/09 08:44:12  tmarsh
// include file reorganization
//
// Revision 1.1  2003/03/09 08:35:32  tmarsh
// include file reorganization
//
// Revision 1.4  2003/03/06 11:20:03  tmarsh
// updated headers
//
//
// =========================================================================

#ifndef XCDR_LFREADER_H
#define XCDR_LFREADER_H

#include "reader/reader.h"
#include "reader/cache/profile.h"
#include "rdbms/query.h"
#include "common/filestream.h"
#include "common/threshold.h"

#include <list>

XCDR_NAMESPACE_BEGIN

// --------------------------------------------------------------------------
//
// LocalFileReader Entity Implementation
//
// --------------------------------------------------------------------------

class LocalFileReader : public ReaderBase
{
public:
	LocalFileReader(unsigned int id);
	~LocalFileReader();
private:
	//
	// Contractual requirements from Entity
	//

	void initialize();
	void execute();
	void wait() const;
	void shutdown();
	void loadProfile();

	//
	// Grab lines from the file
	//

	void poll();

	//
	// Inserts buffered lines into the database
	//

	unsigned int deposit();

	//
	// Updates commited status for previous inserted entries and
	// updates the ReaderLog table.
	//

	void commit();

	//
	// Looks for new files in directory
	//

	void refresh();

	//
	// Various internals
	//

	unsigned int i_dir;
	std::string filename;
	off_t last_offset;
	Reader::Profile profile;
	Reader::ProfileCache *profile_cache;
	FileStream *stream;
	std::list<std::pair<off_t,std::string> > line_buffer;
	unsigned int counter;
	unsigned int line_count;

	unsigned int rawCount();

	bool commit_pending;
	bool waiting;
	bool idle_flag;
	bool stream_idle;

	//
	// Thresholds
	//

	Threshold *refresh_threshold;
	Threshold *commit_threshold;
	Threshold *poll_threshold;
	Threshold *deposit_threshold;

	RDBMS::Repository *repository;
	RDBMS::QueryGroup *query_handle;
};

XCDR_NAMESPACE_END

#endif // XCDR_LFREADER_H
