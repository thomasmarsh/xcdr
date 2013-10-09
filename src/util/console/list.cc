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
// $Source: /usr/local/cvs/xcdr/src/util/console/list.cc,v $
// $Revision: 1.4 $
// $Date: 2003/05/14 11:37:18 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: list.cc,v $
// Revision 1.4  2003/05/14 11:37:18  tmarsh
// replacing DigitNode with dtrie
//
// Revision 1.3  2003/04/04 16:06:36  tmarsh
// namespace changes
//
// Revision 1.2  2003/03/21 11:05:06  tmarsh
// broke out command headers
//
// Revision 1.1  2003/03/18 17:54:12  tmarsh
// added
//
// Revision 1.1  2003/03/15 21:11:26  tmarsh
// added
//
//
// =========================================================================

#include "console.h"
#include "list.h"
#include "tok.h"

#include "common/logging.h"
#include "common/util.h"

#include <iostream>
#include <cstdlib>

XCDR_NAMESPACE_BEGIN

// -------------------------------------------------------------------------
//
// LIST COMMAND
//
// -------------------------------------------------------------------------

enum ListingType
{
	DNODE = 0,
	VALUE
};

struct ListingEntry
{
	ListingType type;
	unsigned int count;
	std::size_t size;
	string name;
	string value;

	ListingEntry(ListingType t, unsigned int c, std::size_t s, const string &n, const string &v)
	{
		type = t;
		count = c;
		size = s;
		name = n;
		value = v;
	}
};

typedef list<ListingEntry> Listing;


inline char *nstr(char *v)
{
	if (v) return v;
	return "<null>";
}

void ListCommand::print()
{
}

void ListCommand::execute()
{
	ENTER();

	build();
	print();
	LEAVE();
}

void xprint(const Listing &l)
{
	ENTER();
	Listing::const_iterator i = l.begin();
	while (i != l.end())
	{
		string msg;
		switch (i->type)
		{
		case DNODE:
			msg += "<node>\t";
			msg += Util::itoa(i->count) + "\t";
			msg += Util::itoa(i->size) + "\t";
			msg += i->name;
			break;
		case VALUE:
			msg += "<value>\t";
			msg += Util::itoa(i->count) + "\t";
			msg += Util::itoa(i->size) + "\t";
			msg += i->value;
		default:
			break;
		}
		INFO(msg);
		i++;
	}
	LEAVE();
}

#define ADD(T, C, S, N, V) \
	l.push_back(ListingEntry(T, C, S, N, V))

void ListCommand::build()
{
	ENTER();
	Listing l;
	std::size_t tot_count = 0;
	std::size_t par_count = 0;
	std::size_t tot_size = 0;
	std::size_t par_size = 0;
	std::size_t tz_cn=0, co_cn=0, pr_cn=0, ci_cn=0, la_cn=0, oc_cn=0;
	std::size_t tz_sz=0, co_sz=0, pr_sz=0, ci_sz=0, la_sz=0, oc_sz=0;
	unsigned int i = 0;
	std::size_t size = 0;
	std::size_t count = 0;

	ConsoleContext::dmap *current = 0;
	ConsoleContext::dmap *parent = 0;

	// ----------------------------------------------------------------------
	//
	// pass 1: get sizes
	//
	// ----------------------------------------------------------------------

	switch (context->state)
	{
	case (ConsoleContext::ROOT):
		TRACE("ConsoleContext::ROOT");
		tz_cn = context->rater->timezone_map->count();
		co_cn = context->rater->country_map->count();
		pr_cn = context->rater->province_map->count();
		ci_cn = context->rater->city_map->count();
		la_cn = context->rater->lata_map->count();
		oc_cn = context->rater->ocn_map->count();
		tot_count = tz_cn + co_cn + pr_cn + ci_cn + la_cn + oc_cn;

		tz_sz = context->rater->timezone_map->size();
		co_sz = context->rater->country_map->size();
		pr_sz = context->rater->province_map->size();
		ci_sz = context->rater->city_map->size();
		la_sz = context->rater->lata_map->size();
		oc_sz = context->rater->ocn_map->size();
		tot_size = tz_sz + co_sz + pr_sz + ci_sz + la_sz + oc_sz;
		
		par_count = tot_count;
		par_size = tot_size;

		break;

	case (ConsoleContext::TIMEZONE):
	case (ConsoleContext::COUNTRY):
	case (ConsoleContext::PROVINCE):
	case (ConsoleContext::CITY):
	case (ConsoleContext::LATA):
	case (ConsoleContext::OCN):
		TRACE("ConsoleContext::(TIMEZONE|COUNTRY|...|OCN)");
		current = context->dnode_stack.back().node;
		tot_count = current->count();
		tot_size = current->size();
		if ((context->dnode_stack.size()) == 1)
		{
			tz_cn = context->rater->timezone_map->count();
			co_cn = context->rater->country_map->count();
			pr_cn = context->rater->province_map->count();
			ci_cn = context->rater->city_map->count();
			la_cn = context->rater->lata_map->count();
			oc_cn = context->rater->ocn_map->count();
			par_count = tz_cn + co_cn + pr_cn + ci_cn + la_cn + oc_cn;

			tz_sz = context->rater->timezone_map->size();
			co_sz = context->rater->country_map->size();
			pr_sz = context->rater->province_map->size();
			ci_sz = context->rater->city_map->size();
			la_sz = context->rater->lata_map->size();
			oc_sz = context->rater->ocn_map->size();
			par_size = tz_sz + co_sz + pr_sz + ci_sz + la_sz + oc_sz;
		}
		else
		{
			list<ConsoleContext::DNode>::iterator d;
			d = context->dnode_stack.end();
			d--; assert(d != context->dnode_stack.begin());
			d--;
			parent = (*d).node;

			par_count = parent->count();
			par_size = parent->size();
		}
		break;
	}

	// ----------------------------------------------------------------------
	//
	// pass 2: append nodes
	//
	// ----------------------------------------------------------------------

	ADD(DNODE, tot_count, tot_size, ".", "");
	ADD(DNODE, par_count, par_size, "..", "");


	switch (context->state)
	{
	case (ConsoleContext::ROOT):
		TRACE("ConsoleContext::ROOT");
		ADD(DNODE, tz_cn, tz_sz, "timezone", "");
		ADD(DNODE, co_cn, co_sz, "country", "");
		ADD(DNODE, pr_cn, pr_sz, "province", "");
		ADD(DNODE, ci_cn, ci_sz, "city", "");
		ADD(DNODE, la_cn, la_sz, "lata", "");
		ADD(DNODE, oc_cn, oc_sz, "ocn", "");
		break;

	case (ConsoleContext::TIMEZONE):
	case (ConsoleContext::COUNTRY):
	case (ConsoleContext::PROVINCE):
	case (ConsoleContext::CITY):
	case (ConsoleContext::LATA):
	case (ConsoleContext::OCN):
		TRACE("ConsoleContext::(TIMEZONE|COUNTRY|...|OCN)");
		current = context->dnode_stack.back().node;
		for (i = 0; i < 10; i++)
		{
			if (current->children)
				if (current->children[i]->count() > 0)
					ADD(DNODE,
					    current->children[i]->count(),
					    current->children[i]->size(),
					    Util::itoa(i), "");
		}
		if (current->data)
		{
			size = strlen(current->data);
			count = 1;
			ADD(VALUE, count, size, "value", string(nstr(current->data)));
		}
		break;
	default:
		break;
	}
	xprint(l);
	LEAVE();
}

string ListCommand::usage()
{
	ENTER();
	string msg = "ls";
	return msg;
	LEAVE();
}

void ListCommand::setParams(const list<string> &params)
{
	ENTER();
	LEAVE();
}

bool ListCommand::matches(const string &name)
{
	if ((name == "list") ||
	    (name == "ls") ||
	    (name == "l"))
		return true;
	return false;
}

XCDR_NAMESPACE_END
