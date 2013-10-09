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
// $Source: /usr/local/cvs/xcdr/src/util/rmap/loc_info.h,v $
// $Revision: 1.1 $
// $Date: 2003/04/08 14:30:53 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: loc_info.h,v $
// Revision 1.1  2003/04/08 14:30:53  tmarsh
// added
//
// Revision 1.3  2003/04/04 16:06:36  tmarsh
// namespace changes
//
// Revision 1.2  2003/03/21 15:13:39  tmarsh
// virtual dtor
//
// Revision 1.1  2003/03/13 15:16:40  tmarsh
// added
//
//
// =========================================================================

#ifndef LERG_LOC_INFO_H
#define LERG_LOC_INFO_H

#include "xml/element.h"

#include <map>
#include <string>

XCDR_NAMESPACE_BEGIN

struct Location
{
	std::string iso_3661;
	std::string country;
	std::string province;
	std::string province_abbrev;
	std::string default_tz;
	std::map<std::string,std::string> subdiv_tz;
	std::map<std::string,std::string> detail;
};

typedef std::map<std::string, Location> LocationMapDetail;

template <class T>
class Leaf : public Element
{
public:
	Element *create() { return new T; }
	virtual ~Leaf() {}
	void leafInit() { data = ""; setHandleChars(true); }
	void characters(const std::string &chars) { data += chars; }
	const std::string &getData() { return data; }
	virtual void bind(AttributeMap &attr) {}
private:
	std::string data;
};

class IsoCode : public Leaf<IsoCode>
{
public:
	void initialize();
};

class Country : public Leaf<Country>
{
public:
	void initialize();
};

class Province : public Leaf<Province>
{
public:
	void initialize();
};

class ProvinceAbbrev : public Leaf<ProvinceAbbrev>
{
public:
	void initialize();
};

class Timezone : public Leaf<Timezone>
{
public:
	void initialize();
	const std::string &getID();

	void bind(AttributeMap &attrs);
private:
	std::string tz_id;
};

class Detail : public Leaf<Detail>
{
public:
	void initialize();
	const std::string &getID();
	void bind(AttributeMap &attrs);
private:
	std::string id;
};

class State : public Element
{
public:
	Element *create();

	void initialize();
	const std::string &getState();
	void bind(AttributeMap &attrs);
	void endChild(Element *elt);
private:
	std::string state;
};


class LocationMap : public RootElement, public LocationMapDetail
{
public:
	Element *create();
	void initialize();
	void bind(AttributeMap &attr);
private:
};

XCDR_NAMESPACE_END

#endif // LERG_LOC_INFO_H
