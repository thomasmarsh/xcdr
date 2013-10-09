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
// $Source: /usr/local/cvs/xcdr/src/util/lerg/loc_info.cc,v $
// $Revision: 1.2 $
// $Date: 2003/04/04 16:06:36 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: loc_info.cc,v $
// Revision 1.2  2003/04/04 16:06:36  tmarsh
// namespace changes
//
// Revision 1.1  2003/03/13 15:16:40  tmarsh
// added
//
//
// =========================================================================

#include "loc_info.h"
#include "common/logging.h"

XCDR_NAMESPACE_BEGIN

void IsoCode::initialize()
{
	ENTER();
	setName("iso");
	leafInit();
	LEAVE();
}

void Country::initialize()
{
	ENTER();
	setName("country");
	leafInit();
	LEAVE();
}

void Province::initialize()
{
	ENTER();
	setName("province");
	leafInit();
	LEAVE();
}

void ProvinceAbbrev::initialize()
{
	ENTER();
	setName("abbrev");
	leafInit();
	LEAVE();
}

void Timezone::initialize()
{
	ENTER();
	tz_id = "";
	setName("timezone");
	addRequired("loc_name");
	leafInit();
	LEAVE();
}

const std::string &Timezone::getID()
{
	ENTER();
	LEAVE();
	return tz_id;
}

void Timezone::bind(AttributeMap &attrs)
{
	ENTER();
	tz_id = attrs["loc_name"];
	LEAVE();
}

void Detail::initialize()
{
	ENTER();
	setName("detail");
	addRequired("id");
	leafInit();
	LEAVE();
}

const std::string &Detail::getID()
{
	ENTER();
	LEAVE();
	return id;
}

void Detail::bind(AttributeMap &attrs)
{
	ENTER();
	id = attrs["id"];
	LEAVE();
}

Element *State::create()
{
	ENTER();
	LEAVE();
	return new State;
}

void State::initialize()
{
	ENTER();
	setName("state");
	addRequired("loc_state");

	addContained("iso", new IsoCode);
	addContained("country", new Country);
	addContained("province", new Province);
	addContained("abbrev", new ProvinceAbbrev);
	addContained("timezone", new Timezone);
	addContained("detail", new Detail);
	LEAVE();
}

const std::string &State::getState()
{
	ENTER();
	LEAVE();
	return state;
}

void State::bind(AttributeMap &attrs)
{
	ENTER();
	state = attrs["loc_state"];
	LEAVE();
}

void State::endChild(Element *elt)
{
	ENTER();
	std::string name = elt->getName();
	LocationMap *parent = (LocationMap*) getParent();
	if (name == "iso")
	{
		(*parent)[state].iso_3661 = ((IsoCode*) elt)->getData();
	}
	else if (name == "country")
	{
		(*parent)[state].country = ((Country*) elt)->getData();
	}
	else if (name == "province")
	{
		(*parent)[state].province = ((Province*) elt)->getData();
	}
	else if (name == "abbrev")
	{
		(*parent)[state].province_abbrev = ((ProvinceAbbrev*) elt)->getData();
	}
	else if (name == "timezone")
	{
		Timezone *tz = (Timezone *) elt;
		std::string tz_id = tz->getID();
		if (tz_id == "")
		{
			(*parent)[state].default_tz = tz->getData();
		}
		else
		{
			(*parent)[state].subdiv_tz[tz_id] = tz->getData();
		}
	}
	else if (name == "detail")
	{
		Detail *detail = (Detail *) elt;
		(*parent)[state].detail[detail->getID()] = detail->getData();
	}
	else
	{
		FATAL("what is this: " << elt->getName());
		LEAVE();
		exit(1);
	}
	LEAVE();
}

Element *LocationMap::create()
{
	ENTER();
	LEAVE();
	return new LocationMap;
}

void LocationMap::initialize()
{
	ENTER();
	setName("location-map");
	addContained("state", new State);
	LEAVE();
}

void LocationMap::bind(AttributeMap &attr)
{
	ENTER();
	LEAVE();
}

XCDR_NAMESPACE_END
