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
// $Source: /usr/local/cvs/xcdr/src/util/rmap/rmap.h,v $
// $Revision: 1.3 $
// $Date: 2003/05/12 14:24:52 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: rmap.h,v $
// Revision 1.3  2003/05/12 14:24:52  tmarsh
// some docs
//
// Revision 1.2  2003/05/05 17:20:31  tmarsh
// misc updates
//
// Revision 1.1  2003/04/08 14:30:53  tmarsh
// added
//
// Revision 1.3  2003/04/04 16:03:58  tmarsh
// namespace changes
//
// Revision 1.2  2003/03/27 19:15:22  tmarsh
// rater rewrite
//
// Revision 1.1  2003/03/17 23:47:22  tmarsh
// moved to root build dir
//
// Revision 1.2  2003/03/15 21:09:09  tmarsh
// added dtor and size method
//
// Revision 1.1  2003/03/14 17:56:43  tmarsh
// added
//
//
// =========================================================================

#ifndef XCDR_UTIL_RMAP_RMAP_H
#define XCDR_UTIL_RMAP_RMAP_H

#include "common/dtrie.h"
#include "common/ttree.h"
#include "common/logging.h"

#include <cstddef>
#include <cstring>

//XXX TEMP
#include <fstream>

XCDR_NAMESPACE_BEGIN

// The rmap is a digital trie[10] which maps telno->value by storing all lengths
// of the telephone number. For example 15124598814 -> TX will create digital
// trie entries of
//
// 	15124598814 -> TX
// 	1512459881 -> TX
// 	...
// 	1512 -> TX
// 	151 -> TX
// 	15 -> TX
// 	1 -> TX
//
// In the case where multiple entries are encountered (for LERG there will be
// about 150,000 inserts for key '1', the NANP country code), the rmap will
// not create a new destination, but only increase the count for that destination.
//
// Once the rmap has been populated, the print() method should be called. This
// method (current) just dumps the tree to a tab separated file which can be
// quickly loaded by the rater. The logic for this dumping procedure is that
// only nodes with a count of 1 will be printed, and the printing will continue
// from the parent node. Therefore, in the example above, 1, 15, 151 will all have
// a count greater than one. 1512 will have a count equal to one and hence printing
// will stop for that branch, and 1513 will be the next case to test.

struct rmap : public dtrie::dtrie_base<rmap, char*, 10, dtrie::decimal_functor>
{
protected:
	typedef dtrie::dtrie_base<rmap, char*, 10, dtrie::decimal_functor> base_type;
	typedef rmap * rmap_ptr;

	char *key;
	char *value;
	unsigned int count;
	ttree::set<char*, char> refs;
	ttree::set<char*, char> dests;
public:
	std::size_t size()
	{
		std::size_t sz = sizeof(rmap);
		sz += refs.size();
		sz += dests.size();
		if (key) sz += strlen(key);
		if (value) sz += strlen(value);
		if (children)
		{
			sz += sizeof(rmap **) * 10;
			for (unsigned int i = 0; i < 10; i++)
				if (children[i])
					sz += children[i]->size();
		}
		return sz;
	}

	rmap() : base_type(), key(0), value(0)
	{
	}

	void clear()
	{
		ENTER();
		if (key) delete key;
		key = 0;
		value = 0;
		count = 0;
		refs.clear();
		dests.clear();
		if (children)
			for (unsigned int i = 0; i < 10; i++)
				if (children[i])
					children[i]->clear();
		LEAVE();
	}

	virtual ~rmap()
	{
		ENTER();
		clear();
		base_type::clear();
		LEAVE();
	}
	
	bool has_key(const char * &k) const
	{
		ENTER();
		if (find(this, const_cast<char*>(k)))
			return true;
		return false;
	}


	bool insert(char *dest, char *value)
	{
		ENTER();
		// for dest "1512459"
		// enter dest:
		// 	1
		// 	15
		// 	151
		// 	..
		// 	1512459

		if (has_key(const_cast<const char*>(dest)))
		{
			rmap *p = find(this, dest);

			assert(p);

			if (strcmp(dest, p->value) != 0)
				return true;
			else
				WARN("duplicated key, value mismatch: ("
					<< dest << ", "
					<< value << ") -> "
					<< p->value);
			return false;
		}

		unsigned int len = strlen(dest);
		for (unsigned int i = 1; i < len+1; i++)
		{
			char *sub = subdest(dest, i);
			insert2(sub, value);
		}
		LEAVE();
		return true;
	}

	unsigned int print(std::ofstream &fp)
	{
		if (count == 1)
		{
			assert(key);
			assert(value);
			fp << value << "\t" << key << "\n";
			return 1;
		}

		unsigned int cnt = 0;
		if (children)
			for (unsigned int i = 0; i < 10; i++)
				if (children[i])
					cnt += children[i]->print(fp);
		return cnt;
	}
private:
	char *subdest(char *dest, unsigned int len)
	{
		ENTER();
		static char buf[10];
		strcpy(buf, dest);
		buf[len] = '\0';

		ttree::set<char*,char> *p = dests.find(&dests, buf);
		if (!p)
		{
			char *tmp = strdup(buf);
			p = dests.p_insert(tmp);
		}
		LEAVE();
		return p->key;
	}

	bool insert2(char *dest, char *value)
	{
		ENTER();
		TRACE(dest << " -> " << value);
		rmap *p = find(this, dest);
		if (!p)
			return insert_new(dest, value);

		if (!p->refs.has_key(value))
		{
			p->count++;
			p->refs.insert(strdup(value));
		}
		LEAVE();
		return true;
	}

	bool insert_new(char *dest, char *value)
	{
		ENTER();
		assert(dest);
		assert(value);
		rmap *p = allocate(this, dest);
		if (!p)
		{
			LEAVE();
			p->key = 0;
			p->value = 0;
			assert(1==0);
			return false;
		}
		p->key = dest;
		p->value = value;
		p->refs.insert(value);
		p->count = 1;
		LEAVE();
		return true;
	}
};

XCDR_NAMESPACE_END

#endif // XCDR_UTIL_RMAP_RMAP_H
