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
// $Source: /usr/local/cvs/xcdr/src/include/gateway/mvam/mvam.h,v $
// $Revision: 1.1 $
// $Date: 2003/03/09 08:35:32 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: mvam.h,v $
// Revision 1.1  2003/03/09 08:35:32  tmarsh
// include file reorganization
//
// Revision 1.2  2003/03/06 11:20:03  tmarsh
// updated headers
//
//
// =========================================================================

#ifndef MVAM_H
#define MVAM_H

#include "config.h"
#include "handler.h"

#include <deque>
#include <set>

//---------------------------------------------------------------------------
//
// MVAM Terminology:
//
//    ARQ: answer(?) request
//    BRW: bandwidth request
//    DRQ: disconnect request
//    DCF: disconnect confirmation
//
//---------------------------------------------------------------------------


namespace MVAM
{
    const string SOURCE_TYPE = "MVAM";

    const string VERSION_STRING = "version";
    const string START_STRING = "start";
    const string STOP_STRING  = "stop";
    const string INCOMPLETE_STRING = "incomplete";

    enum mvam_rec_t
    {
	    VERSION_RECORD,
	    START_RECORD,
	    INCOMPLETE_RECORD,
	    STOP_RECORD
    };


    //-----------------------------------------------------------------------
    //
    // build a DateTime
    //
    //-----------------------------------------------------------------------

    inline static DateTime toDateTime(string& date, string& time)
    {
        DateTime dt;

	try
	{
	    dt = DateTime(date, time);
	}
	catch (StringTokenizer::NoMoreTokensException)
	{
	    DEBUG("WARNING: bad datetime: " << date << " " << time);

            dt = DateTime();
        }
        return dt;
    }


    //-----------------------------------------------------------------------
    //
    // MVAM Record
    //
    // This is the abstract base for the Record types which are
    // currently:
    //
    //    MVAM::Start
    //    MVAM::Stop
    //    MVAM::Incomplete
    //    MVAM::Version
    //
    //-----------------------------------------------------------------------

    class Record
    {
    public:
        friend class RecordList;

        virtual ~Record() {}

        friend ostream& operator << (ostream& o, Record& r);
        off_t  pos;

        mvam_rec_t type;
        string date;
        string time;
        string call_identifier;

        inline string& getSourceString() { return _source; }

    protected:
	string _source;

    private:
	unsigned int sequence;
	time_t timestamp;

	// raw source record

        virtual void parse(string& raw) {}
        virtual void print(ostream& o) {}
    };


    // -------------------------------------------------------------------
    //
    // Start Record
    // 
    // -------------------------------------------------------------------

    class Start: public Record
    {
    public:

        Start() {}
        Start(string& raw);

        string connecting_endpoint;
        string bandwidth;
        string calling_party;
        string calling_endpoint;
        string called_endpoint;
        string called_party;

    private:
        void parse(string& raw);

        void print(ostream& o);
    };
   

    // -----------------------------------------------------------------------
    //
    // Stop Record
    //
    // -----------------------------------------------------------------------

    class Stop : public Record
    {
    public:
        Stop() {}
        Stop(string& raw);

        string disconnecting_endpoint;
        string disconnect_reason;
        string call_duration;

    private:
        void parse(string& raw);

        void print(ostream& o);
    };


    // -----------------------------------------------------------------------
    //
    // Incomplete Record
    //
    // -----------------------------------------------------------------------

    class Incomplete : public Record
    {
    public:
        Incomplete() {}
        Incomplete(string& raw);

        string disconnecting_endpoint;
        string calling_party;
        string called_party;
        string disconnect_reason;
        string call_duration;

    private:
        void parse(string& raw);

        void print(ostream& o);
    };


    // -----------------------------------------------------------------------
    //
    // Version Record
    //
    // -----------------------------------------------------------------------

    class Version : public Record
    {
    public:
        Version() {}
        Version(string& raw);

        string version_identifier;

    private:
        void print(ostream& o);
        void parse(string& raw);
    };


    // -----------------------------------------------------------------------
    //
    // Record List
    //
    // NOTE: The RecordList is too large a class and the call profile
    // categorization (Category and CategoryList) should be moved out in a
    // later release.
    //
    // -----------------------------------------------------------------------
    
    class MVAMHandler;

    class RecordList
    {
    public:

        RecordList();
        RecordList(Mediator *m, MVAMHandler* h);
        ~RecordList();

        void setMediator(Mediator *m) { mediator = m; }
        void setHandler(MVAMHandler* h) { handler = h; }


	// list operations
        void append(Record *r);
        unsigned int length() { return data.size(); }


        bool ready();
        bool expired() const;
        void categorize();
        void finalize( unsigned int duration );

	unsigned int getTotalBytes(unsigned int duration);
	unsigned int getBandwidth();


        // Outputs
	void writeUnresolvables(int err);
        BillableRecord toBillableRecord(unsigned int duration);
	UnbillableRecord toUnbillableRecord(unsigned int duration);


	// report
	void reportUnfinalized();
	string profile();


    private:

	// --------------------------------------------------------------------
	// BEGIN: RecordList private classes
	// --------------------------------------------------------------------

        // -------------------------------------------------------------------
	// Category
	//
	// Maintained by a CategoryList (later). Describes the call profile
	// contained by the RecordList.
	//
        // -------------------------------------------------------------------

	friend class Category
	{
	public:

            // these category ids map to a function which tests true or false in
	    // the RecordList under the heading 'call configuration scenarios'

	    enum categoryId
	    {
	        WELL_FORMED =0,
		INCOMPLETE,
		SINGLE_INCOMPLETE_START_NORMALDROP,
		SINGLE_INCOMPLETE_START_UNDEFINED_RINGING,
		SINGLE_INCOMPLETE_START_UNDEFINED_TIMEOUT,
		MULTIPLE_STOPS,
		FORCED_DROP_STOP,
		DELAYED_STOPS,
		MULTIPLE_STARTS,
		UNMATCHED
	    };


	    // a pointer to a member function. This is used as an argument to
	    // the constructor and later in the test() method.

	    typedef bool (MVAM::RecordList::*TestFunc)();


	    //
            // Constructor
	    //

	    Category(string name, categoryId id, TestFunc f)
	    {
		_count = 0;
		_name = name;
		_id = id;
		_function = f;
	    }


            // Test to see whether this call category matches the call
	    // record list passed

	    inline bool test(MVAM::RecordList& rl)
	    {
		// This strange syntax should read as:
		//   (object).(*(ptrToMemberFunc))()
		// 
		// in other words, we dereference the member function, and
		// use the '.' operator to invoke the method on the target
		// object. Sorry.

	        bool matches = ((rl).*(_function))();

		if (matches)
		    _count++;

		return matches;
	    }


	    // accessors
	    string getName() const { return _name; }
	    inline unsigned int getCount() const { return _count; }
	    inline categoryId getId() const { return _id; }

	private:
	    TestFunc _function;
	    string _name;
	    unsigned int _count;
	    categoryId _id;

	}; // END: class Category


        // -------------------------------------------------------------------
	// CategoryList
	//
	// Tries to use recent historical statistics to determine the most
	// efficient means of classifying the record.
	//
        // -------------------------------------------------------------------

	friend class CategoryList
	{
	public:
	    friend class Category;

	    Category::categoryId categorize(MVAM::RecordList& rl);

	    void sort();
	    void buildCategories();

	    inline bool empty() { return categories.empty(); }

	private:
	    list<Category> categories;
	};


	// --------------------------------------------------------------------
	// END: RecordList private classes
	// --------------------------------------------------------------------

	CategoryList category_list;

	// record retrieval
	
	void sort();

        bool hasStops();

	Start* firstStart();
	Start* secondStart();
	Start* lastStart();
	Incomplete *firstIncomplete();
	Stop* firstStop();
	Stop* failureStop();


	// record configuration scenarios

	bool well_formed();
	bool incomplete();
	bool single_incomplete_start_normalDrop();
	bool single_incomplete_start_undefined_ringing();
	bool single_incomplete_start_undefined_timeout();
	bool multiple_stops();
	bool delayed_stops();
	bool forced_drop_stop();
	bool multiple_starts();


	// member variables
        Mediator *mediator;


    public:
        deque<Record *> data;
    protected:


	Timer wait_timer;
        unsigned int row_count;
	MVAMHandler* handler;

    }; // END: class RecordList


    // -----------------------------------------------------------------------
    //
    // MVAM Handler
    //
    // -----------------------------------------------------------------------

    class MVAMHandler : public Handler
    {
    friend class RecordList;

    public:
        MVAMHandler(Params::MVAMHandler params);
	virtual ~MVAMHandler();
        void report();
	virtual string getType() const { return "MVAM"; }

    protected:
	bool isValidFile(const string& fname) const;

        void poll();
        void flushRecords();

	virtual string handle(string& raw, LoggedStream* ls);
	// unused!!


    	// returns call identifier
	virtual string handle(string& raw);
	virtual void handle(Archive::ReadLineResult& r);

	void commit(const string& call_id);

	Record* toRecord(string& raw);


        map<string, RecordList> mvam_map;

	Params::MVAMHandler mvam_params;

        unsigned long count;

	LoggedStream* current_stream;
    };

}; // namespace MVAM


#endif // MVAM_H
