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
// $Source: /usr/local/cvs/xcdr/src/include/common/dtrie.h,v $
// $Revision: 1.4 $
// $Date: 2003/05/14 11:35:14 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: dtrie.h,v $
// Revision 1.4  2003/05/14 11:35:14  tmarsh
// bailout case for null pointers
//
// Revision 1.3  2003/05/05 17:20:31  tmarsh
// misc updates
//
// Revision 1.2  2003/04/08 14:29:20  tmarsh
// misc updates
//
// Revision 1.1  2003/04/07 00:36:39  tmarsh
// added
//
//
// =========================================================================

#ifndef XCDR_COMMON_DTRIE_H
#define XCDR_COMMON_DTRIE_H

#include "common/common.h"
#include "common/logging.h"

#include <cstddef>
#include <cstring>

XCDR_NAMESPACE_BEGIN

namespace dtrie
{
	// -------------------------------------------------------------------
	//
	// __builtin_functor
	//
	// this is more an example, as it is not likely to work..
	//
	// -------------------------------------------------------------------

	struct __builtin_functor
	{
		template <typename T>
		inline static unsigned int translate(T deref)
		{
			return (unsigned int) deref;
		}
	};


	// -------------------------------------------------------------------
	//
	// dtrie_base<P,K,N,F>
	//
	// P	= inherited (allocatable) type
	// K	= key type
	// N	= number of digits in the digital trie
	// F	= functor (maps a dereferenced key to a digit)
	//
	// -------------------------------------------------------------------

	template
	<
		typename P,
		typename K,
		unsigned int N,
		typename F=__builtin_functor
	>
	struct dtrie_base
	{
		// -----------------------------------------------------------
		//
		// Contained data
		//
		// -----------------------------------------------------------

		P **children;
		bool used;


		// -----------------------------------------------------------
		//
		// CTOR and DTOR
		//
		// -----------------------------------------------------------

		dtrie_base() : children(0), used(false)
		{
		}

		void clear()
		{
			ENTER();
			if (children)
			{
				for (unsigned int i = 0; i < N; i++)
					if (children[i])
					{
						delete children[i];
						children[i] = 0;
					}
				delete [] children;
				children = 0;
			}
			LEAVE();
		}

		virtual ~dtrie_base()
		{
			ENTER();
			clear();
			LEAVE();
		}


		// -----------------------------------------------------------
		//
		// ptr find(ptr head, Key k)
		//
		// Returns the found pointer to the node with key k, or null
		// in the case of failure. The head pointer is necessary to
		// allow assignment of the start node.
		//
		// -----------------------------------------------------------

		virtual P *find(const P* head, const K& _k) const
		{
			register K k = const_cast<K>(_k);
			register P *p = const_cast<P*>(head);
			register unsigned int i;

			do
			{
				if ((*k) == 0)
					return p;

				i = F::translate(*k++);

				if (!(p->children && p->children[i]))
					return 0;

				p = p->children[i];
			} while (true);
		}


		// -----------------------------------------------------------
		//
		// ptr allocate(ptr head, Key k)
		//
		// Returns a pointer to the allocated node. Requires a
		// pointer to head in order to set the start node.
		//
		// -----------------------------------------------------------

		P *allocate(const P *head, const K &_k)
		{
			register K k = const_cast<K>(_k);
			register P *p = const_cast<P*>(head);
			register unsigned int i;
			do
			{
				if ((*k) == 0)
				{
					p->used = true;
					return p;
				}

				i = F::translate(*k++);

				if (!p->children)
				{
					p->children = new P*[N];
					for (unsigned int j = 0; j < N; j++)
						p->children[j] = 0;
				}

				if (!p->children[i])
					p->children[i] = new P;

				p = p->children[i];

			} while (true);
		}


		// -----------------------------------------------------------
		//
		// size_t count
		//
		// Counts the number of nodes in the tree recursively by
		// checking the 'used' flag.
		//
		// -----------------------------------------------------------

		std::size_t count()
		{
			if (!this) return 0;
			std::size_t c = 0;
			if (children)
				for (unsigned int i = 0; i < N; i++)
					if (children[i])
						c += children[i]->used
						   + children[i]->count();
			return c;
		}

		std::size_t size()
		{
			std::size_t sz = sizeof(P);
			if (children)
			{
				sz += sizeof(P**) * N;
				for (unsigned int i = 0; i < N; i++)
					if (children[i])
						sz += children[i]->size();
			}
			return sz;
		}
	};


	// -------------------------------------------------------------------
	//
	// map<K,T,N,F>
	//
	// K	= key type
	// T	= data type
	// N	= number of digits in the digital trie
	// F	= functor
	//
	// -------------------------------------------------------------------

	template
	<
		typename K,
		typename T,
		unsigned int N,
		typename F=__builtin_functor
	>
	struct map : public dtrie_base<map<K,T,N,F>,K,N,F>
	{
		// -----------------------------------------------------------
		//
		// Contained data
		//
		// -----------------------------------------------------------

		typedef dtrie_base<map<K,T,N,F>,K,N,F> base_type;
		typedef map<K,T,N,F> map_type;
		typedef map<K,T,N,F> *map_ptr;
		T data;


		// -----------------------------------------------------------
		//
		// Constructor
		//
		// -----------------------------------------------------------

		map() : base_type(), data(0)
		{
		}


		// -----------------------------------------------------------
		//
		// Map interface
		//
		// -----------------------------------------------------------

		bool has_key(const K &k) const
		{
			if (find(this, k))
				return true;
			return false;
		}

		virtual const T &operator [] (const K &k) const
		{
			static T null_hack = 0;

			map_ptr p = find(this, k);
			if (!p) return null_hack;
			return p->data;
		}

		bool insert(const K &k, const T &t)
		{
			map_ptr p = allocate(this, k);
			if (!p) return false;
			p->data = const_cast<T>(t);
			return true;
		}


		// -----------------------------------------------------------
		//
		// this is a bit of a hack to get around the loss of
		// polymorphism when using template classes
		//
		// -----------------------------------------------------------

		inline const map_ptr get_map_ptr() const
		{ return const_cast<map_ptr>(this); }
	};


	// -------------------------------------------------------------------
	//
	// digit_map
	//
	// The digit map is specialized for several purposes:
	//
	//    - A functor "decimal" is used which maps and ASCII
	// 	digit (e.g., '0' is 48 ASCII, and is mapped to an integer
	// 	zero value).
	//
	//    - Key values are char*
	//
	//
	// -------------------------------------------------------------------

	struct decimal_functor
	{
		inline static unsigned int translate(char c)
		{
			TRACE("'" << c << "'" << " -> " << ((unsigned int) c-'0'));
			return c-'0';
		}
	};

	template <typename T>
	struct digit_map : public map<char*, T, 10, decimal_functor>
	{
		typedef map<char*, T, 10, decimal_functor> digit_type;
		typedef digit_type *digit_ptr;

		digit_map() : digit_type()
		{
		}

		virtual ~digit_map()
		{
		}

		virtual digit_ptr
		find(const digit_ptr head, const char* _k) const
		{
			register char* k = const_cast<char*>(_k);
			register digit_ptr p = const_cast<digit_ptr>(head);
			register unsigned int i;

			do
			{
				if (p->data || ((*k) == 0))
					return p;

				i = decimal_functor::translate(*k++);

				if (!(p->children && p->children[i]))
					return 0;

				p = p->children[i];
			} while (true);
		}

		virtual const T &operator [] (const char* k) const
		{
			static T null_hack = 0;

			const char *c = k;
			digit_ptr p = digit_map<T>::find(get_map_ptr(), c);
			if (!p) return null_hack;
			return p->data;
		}
	};
}

XCDR_NAMESPACE_END

#endif // XCDR_COMMON_DTRIE_H
