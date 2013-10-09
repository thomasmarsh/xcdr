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
// $Source: /usr/local/cvs/xcdr/src/include/common/slist.h,v $
// $Revision: 1.1 $
// $Date: 2003/04/02 20:34:46 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: slist.h,v $
// Revision 1.1  2003/04/02 20:34:46  tmarsh
// added from rater
//
// Revision 1.1  2003/03/27 19:15:37  tmarsh
// added
//
//
// =========================================================================

#ifndef XCDR_SLIST_H
#define XCDR_SLIST_H

#include "common/common.h"
#include <cstdlib>

XCDR_NAMESPACE_BEGIN

// a singly-linked list with special pointer semantics
//
//	struct T
//	{
//		unsigned int x;
//		unsigned int y;
//	};
//
//	slist<T> tl;
//	tl.push_back(new T);
//	slist<T>::iterator i = tl.begin();
//	while (i != tl.end())
//	{
//		T *t = *i;
//
//		t->x = 1;
//		t->y = 2;
//
//		i++;
//	}

template <typename T, typename L>
struct slist_iterator
{
	L *current;
	typedef slist_iterator<T,L> _Self;

	slist_iterator()
	{
		current = 0;
	}
	
	slist_iterator(const L *c)
	{
		current = const_cast<L*>(c);
	}
	
	slist_iterator(const slist_iterator &other)
	{
		current = other.current;
	}

	inline T* operator *()
	{
		return current->value;
	}
	
	inline _Self& operator++(int)
	{
		this->current = this->current->next;
		return *this;
	}
	
	
	inline bool operator == (const _Self &other)
	{
		return (current == other.current);
	}
	
	inline bool operator != (const _Self &other)
	{
		return (current != other.current);
	}
};

template <typename T>
struct slist
{
	typedef slist<T> _Self;
	typedef slist_iterator<T, _Self> iterator;
	
	slist *next;
	T *value;

	slist()
	{
		next = 0;
		value = 0;
	}
	
	iterator begin() const
	{
		return iterator(const_cast<_Self*>(this));
	}
	
	iterator end() const
	{
		return iterator(0);
	}

	~slist()
	{
		clear();
	}

	void clear()
	{
		if (value) { delete value; value = 0; }
		if (next) { delete next; next = 0; }
	}

	void push_back(T *v)
	{
		slist *current = this;
		do
		{
			if (!current->value)
			{
				current->value = v;
				current->next = 0;
				break;
			}
			if (!current->next)
			{
				current->next = new slist<T>;
				current = current->next;
				current->value = v;
				current->next = 0;
				break;
			}
			current = current->next;
		} while (true);
	}

	T *find(const T& input) const
	{
		return find(&input);
	}
	
	T *find(const T *input) const
	{
		_Self *current = const_cast<_Self*>(this);
		do
		{
			if (current->value)
				if (input->match(current->value))
					return current->value;
			if (!current->next)
				return 0;
			current = current->next;
		} while (true);
	}

	inline std::size_t deepCount() const
	{
		std::size_t cnt = 1;
		if (value) cnt += value->count();
		if (next) cnt += next->count();
		return cnt;
	}

	inline std::size_t count() const
	{
		std::size_t cnt = 0;
		if (value) cnt++;
		if (next) cnt += next->count();
		return cnt;
	}

	inline std::size_t size() const
	{
		std::size_t sz = sizeof(slist);
		if (value) sz += value->size();
		if (next) sz += next->size();
		return sz;
	}
};

XCDR_NAMESPACE_END

#endif // XCDR_SLIST_H
