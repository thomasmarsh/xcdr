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
// $Source: /usr/local/cvs/xcdr/src/include/gateway/dco/dco.h,v $
// $Revision: 1.1 $
// $Date: 2003/03/09 08:35:32 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: dco.h,v $
// Revision 1.1  2003/03/09 08:35:32  tmarsh
// include file reorganization
//
// Revision 1.2  2003/03/06 11:20:03  tmarsh
// updated headers
//
//
// =========================================================================

#ifndef DCO_H
#define DCO_H

#include "common.h"
#include "mediator.h"
#include "handler.h"
#include "config.h"

#include <string>

class DCOCache
{
public:
	DCOCache(const string& cache_file_name, const string& primary_dir,
		 const string& backup_dir);
	~DCOCache();

	bool addID(const string& call_id);
	void refresh();

protected:
	map<string, int> call_ids;
	Transaction::Log log;
};


class DCOHandler : public Handler
{
public:
    DCOHandler(Params::DCOHandler params);

    //XXX this class needs an assignment operator

    virtual ~DCOHandler();

    virtual string handle(string& line, LoggedStream* stream);
    static BillableRecord parse(const string& input_string, BillableRecord& r)
	    throw (TelephoneNumber::ParseError, InvalidInputException,
			    TelephoneNumber::CountryCodeLookupException);

    void commit(const string& call_id);

    virtual bool isValidFile(const string& fname) const;

    bool isOTHFile(const string& fname) const;
    bool isDATFile(const string& fname) const;

    /**************************************************
     * commented out because we are not using live files right now
     *

    void cleanUpLiveFiles();
    void cleanUpLiveFiles(map<string,int>& latest_live_files);
    void scanLiveDir(const string& directory,
		     map<string,int>& latest_live_files);
    void addDATFile(const string& fname, const string& directory);
    void addOTHFile(const string& fname, const string& directory);

     ******************************************************/


    void scanArchiveDir(const string& directory, const string& parent_dir);
    string getLogDirFromLiveDir(const string& directory);
    string getBackupLogDirFromLiveDir(const string& directory);
    string DCOHandler::getPrimaryCacheDir();
    string DCOHandler::getBackupCacheDir();
    string DCOHandler::getCacheFileName(const DateTime& date);
    string DCOHandler::getUnresolvablesCacheFileName();

    DCOCache* getCacheForDate(const DateTime& date);
    DateTime DCOHandler::findLRUCache();

    virtual string getType() const { return "DCO"; }

    void processDAT(BillableRecord& r);
    void processOTH(BillableRecord& r);

    virtual void refresh() throw (DirectoryException);
    virtual void poll();


protected:
    list<string> x_archive_dirs;
    list<string> y_archive_dirs;
    string date_cache_dir;

    map<DateTime, pair<DCOCache*, time_t> > cache_map;
    DCOCache unresolvables_cache;

    /*************************************************
     * we're not using live streams or OTH right now
     *

    map<string,LoggedStream*> live_dat_streams;
    map<string,LoggedStream*> live_oth_streams;

    Archive oth_archive;
    ***********************************************/

};

#endif // DCO_H
