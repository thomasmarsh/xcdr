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
// $Source: /usr/local/cvs/xcdr/src/include/common/ttree.h,v $
// $Revision: 1.10 $
// $Date: 2003/05/14 11:35:02 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: ttree.h,v $
// Revision 1.10  2003/05/14 11:35:02  tmarsh
// misc comments
//
// Revision 1.9  2003/05/13 16:30:16  tmarsh
// minor update for fewer dereferencings
//
// Revision 1.8  2003/05/13 15:58:08  tmarsh
// optimizations to iterator and cleanup
//
// Revision 1.7  2003/05/13 14:26:29  tmarsh
// added documentation of the iterator
//
// Revision 1.6  2003/05/13 13:41:25  tmarsh
// fixed iterator!
//
// Revision 1.5  2003/05/12 14:24:40  tmarsh
// new attempt at broken iterator
//
// Revision 1.4  2003/05/05 17:20:31  tmarsh
// misc updates
//
// Revision 1.3  2003/04/08 14:29:03  tmarsh
// major updates
//
// Revision 1.2  2003/04/06 00:24:29  tmarsh
// optimizations and cleanups
//
// Revision 1.1  2003/04/05 23:52:20  tmarsh
// added, need destructor...
//
//
// =========================================================================

#ifndef XCDR_COMMON_TTREE_H
#define XCDR_COMMON_TTREE_H

#include "common/common.h"
#include "common/stack.h"
#include "common/logging.h"
#include <cstdlib>

XCDR_NAMESPACE_BEGIN

// -------------------------------------------------------------------------
// 
// A ternary tree struct for string keys with at most O(log n+k)
// comparisons.
//
// TODO: generalize a bit. It would be nice to have ttree_set
//       as well as ttree_map.
//
// Usage:
//
// 	ttree::map<char*, char, char*> map;
//
// 	char *foo = "a";
// 	char *bar = "b";
// 	map[foo] = bar;
//
// -------------------------------------------------------------------------


namespace ttree
{
	// P is the inheritting type
	// K is the key type
	// D is the dereferenced key type

	template <typename P, typename K, typename D>
	class _ttree_base
	{
	protected:
		P*	left;
		P*	middle;
		P*	right;
		typedef _ttree_base<P,K,D> stype;


		enum Direction { LEFT=0, MIDDLE=1, RIGHT=2, STOPITER=3 };

		struct stack_node
		{
			Direction dir;
			P *node;

			stack_node(Direction d=LEFT, P* n=0) : dir(d), node(n)
			{
			}

			~stack_node()
			{
				dir = STOPITER;
				node = 0;
			}
		};

		struct inorder_frame
		{
			int state;
			P* node;
			stack<stack_node*> _stack;

			inorder_frame(P* node) : state(0), node(node)
			{
			}

			~inorder_frame()
			{
				state = -1;
				node = 0;
			}
		};


		// -------------------------------------
		// -- inorder iterator implementation --
		// -------------------------------------
		//
		// This method is a bit complicated for speed purposes. The requirements
		// of the iterator were:
		//
		// 	- must be iterative, not recursive
		//
		// 	- must minimize resource consumption (i.e., don't mirror the
		// 	  object we are iterating over
		//
		// 	- must be as efficient as call/cc
		//
		// Call/cc (call-with-current-continuation) is implemented via a frame
		// structure pointer being passed to this method. The frame contains
		// everything that would normally be a local, and also contains a state
		// attribute which the 'switch' will carry us to. There are only three
		// states:
		//
		// 	0: initialization
		// 	1: continue after initial return
		// 	-1: finished
		//
		// This combination of switch(frame->state) and case statements for
		// each possible state is simply emulation of the 'yield' statement
		// that languages like Sather, Perl, and Ruby implement, or similar
		// to mechanisms natively provided in Lisp/Scheme with call/cc.
		//
		// The recursive implementation of ternary tree traversal is implemented
		// as such:
		//
		// 	void traverse(ttree *p)
		// 	{
		// 		if (!p) return;
		// 		traverse(p->left);
		// 		if (p->splitchar)
		// 			traverse(p->middle);
		// 		else
		// 			printf("%s\n", (char *) p->middle);
		// 		traverse(p->right);
		// 	}
		//
		// Notably, a recursive implementation cannot be used as an iterator
		// without recursing back to the point where it left off when proceeding
		// to the next value. This has a huge penalty unless a funtion pointer
		// is also passed to be used as a callback for found node. Using a
		// callback function, however, limits the flexibility of those wishing
		// to iterate over the structure.
		//
		// To implement an iterative version requires us keeping our own stack.
		// A binary tree implementation (using the yield statement in place of
		// a switch) would look like this:
		//
		// 	binary_tree *inorder()
		// 	{
		// 		node = this;
		// 		stack<binary_tree*> stk;
		//
		// 		while (node)
		// 		{
		// 			while (node->left)
		// 			{
		// 				stk.push(node);
		// 				node = node->left;
		// 			}
		//
		// 			yield node;
		//
		// 			while ((!node->right) && stk.size())
		// 			{
		// 				node = stk.pop();
		// 				yield node;
		// 			}
		// 			node = node->right;
		// 		}
		// 	}
		//
		// A ternary tree is a bit more complicated since the method cannot
		// use the simple logic of:
		//
		// 	1. seek as far left as possible
		// 	2. pop and try to go right
		// 	3. goto step 1
		//
		// Instead, when pushing on the stack, we also store which direction
		// we descended (left, middle, right). When popping off the stack,
		// we consider which way the popped element was pursued and thence
		// pursue the next logical direction. (If we already pursued left, then
		// when the node is popped, we need to try and pursue it via the middle.)
		//
		// Another restriction of the ternary tree is that not every node
		// constitutes a value, so we first have to check for the existence of
		// a key at the current node before returning.
		//
		// The only way to implement this efficiently and with a minimum of
		// boiler-plate code duplication we have to resort to 'goto' statements.
		// We define four goto tags:
		//
		// 	- descend_left
		// 	- descend_middle
		// 	- descend_right
		// 	- ascend
		//
		// Now we can describe our algorithm in these terms:
		//
		// 	P* inorder()
		// 	{
		// 	descend_left:
		// 		while (node->left)
		// 		{
		// 			stack.push(LEFT, node);
		// 			node = node->left;
		// 		}
		//
		// 	descend_middle:
		// 		if (node->middle)
		// 		{
		// 			stack.push(MIDDLE, node);
		// 			node = node->middle;
		// 			goto descend_left;
		// 		}
		//
		// 	descend_right:
		// 		if (node->right)
		// 		{
		// 			stack.push(RIGHT, node);
		// 			node = node->right;
		// 			goto descend_left;
		// 		}
		//
		// 	ascend:
		// 		if (node->key)
		// 			yield node;	// RETURN A VALUE
		//
		// 		if (stack.size())
		// 		{
		// 			direction, node = stack.pop();
		// 			switch (direction)
		// 			{
		// 			case LEFT:    goto descend_middle;
		// 			case MIDDLE:  goto descend_right;
		// 			default:      goto ascend;
		// 			}
		// 		}
		// 		// IF WE GET HERE, ITERATION IS OVER
		// 	}
		//
		// The implementation below follows the above pseudo-code with the
		// addition of the switch statement to allow continuation after a
		// return.

		P* inorder_call(inorder_frame *frame)
		{
			// these are temporaries used in 'ascend:'
			Direction direction;
			stack_node *sn;

			// use a local 'node' variable to limit dereferencing in test
			// cases with large sets (such as /usr/share/dict/words) it
			// is common to see 8-10 pops and pushes in a single call
			// to the iterator. Therfore, the cost of dereferencing,
			// e.g., 'frame->node = frame->node->left', is high compared
			// to the cost of this assignment.
			P *node = frame->node;

			switch (frame->state)
			{
			case 0:
				frame->state = 1;

		descend_left:
				// Pursue left as far as possible

				while (node->left)
				{
					frame->_stack.push(new stack_node(LEFT, node));
					node = node->left;
				}

		descend_middle:
				// pursue middle, then continue trying left

				if (node->middle)
				{
					frame->_stack.push(new stack_node(MIDDLE, node));
					node = node->middle;
					goto descend_left;
				}

		descend_right:
				// pursue right, then continue trying left

				if (node->right)
				{
					frame->_stack.push(new stack_node(RIGHT, node));
					node = node->right;
					goto descend_left;
				}

		ascend:
				// yield a value if there is one before ascending
				if (node->key)
				{
					// save the node on the frame object
					frame->node = node;
					return node;
				}

			// carry on after previous return
			case 1:

				// if the stack is empty, we won't pursue further
				if (frame->_stack.size())
				{
					// pop from the stack

					sn = frame->_stack.pop();
					node = sn->node;
					direction = sn->dir;
					delete sn;

					// pursue the correct direction (middle,
					// or right) or pop again

					switch (direction)
					{
					case LEFT:	goto descend_middle;
					case MIDDLE:	goto descend_right;
					default:	goto ascend;
					}
				}

			case -1:
				frame->state = -1;
			}
			return 0;
		}

	public:
		K	key;
		D	split;

		struct iterator
		{
			inorder_frame frame;
			bool _done;
			P *current;

			inline bool done() { return _done; }

			iterator(P* root) : frame(root), _done(false), current(0)
			{
				(*this)++;
			}

			P* &operator *() { return current; }

			iterator& operator ++(int) { next(); return *this; }

			void next()
			{
				P* tmp = frame.node->inorder_call(&frame);
				if (frame.state == -1)
					_done = true;
				else current = tmp;
			}
		};

		virtual iterator inorder(P* head) { iterator t(head); return t; }

		_ttree_base()
			: left(0), middle(0), right(0),
			  key(0), split(0)
		{
		}
		
		_ttree_base(D s)
			: left(0), middle(0), right(0),
			  key(0), split(s)
		{
		}
		
		virtual ~_ttree_base()
		{
			clear();
		}

		virtual void clear()
		{
			key = 0;
			split = 0;
			if (left)
			{
				left->clear();
				delete left;
				left = 0;
			}
			if (middle)
			{
				middle->clear();
				delete middle;
				middle = 0;
			}
			if (right)
			{
				right->clear();
				delete right;
				right = 0;
			}
		}


		inline P* front(const P* head) const
		{
			if (left) return left->front();
			if (middle) return middle->front();
			if (right) return right->front();

			return this;
		}

		inline P* find(const P* head, const K& _k) const
		{
			K k = const_cast<K>(_k);
			P* p = const_cast<P*>(head);
			while (p)
			{
				if (*k < p->split) p = p->left;
				else if (*k == p->split)
				{
					if (*k++ == 0)
						return p;
					p = p->middle;
				}
				else p = p->right;
			}
			return 0;
		}



		inline P* allocate(P* head, const K &_k)
		{
			K k = const_cast<K>(_k);
			register P* p = const_cast<P*>(head);

			if (!p->split)
				p->split = *k;

			register D d;
			do
			{
				d = *k;
				if (d < p->split)
				{
					if (!p->left) p->left = new P(d);
					p = p->left;
				}
				else if (d == p->split)
				{
					if (d == 0)
						return p;
					else
					{
						if (!p->middle)
							p->middle = new P(d);
						p = p->middle;
						k++;
					}
				}
				else
				{
					if (!p->right) p->right = new P(d);
					p = p->right;
				}

			} while (true);

			// not reached
			return 0;
		}
	};


	template <typename K, typename D, typename V>
	struct map : public _ttree_base<map<K,D,V>,K,D>
	{
	protected:
		// typedefs

		typedef map<K,D,V>* map_ptr;
		typedef map<K,D,V> map_type;
		typedef typename _ttree_base<map<K,D,V>,K,D>::iterator iter_type;




	public:
		// members
		V		data;

		map() : _ttree_base<map_type,K,D>(), data(0)
		{
		}
		
		map(D s) : _ttree_base<map_type,K,D>(s), data(0)
		{
		}
		
		virtual ~map()
		{
			data = 0;
		}

		inline const V &null_hack() const
		{
			static V h=0;
			return h;
		}

		inline bool has_key(const K& k) const
		{
			if (find(this, k))
				return true;

			return false;
		}

		inline const V &operator [] (const K k) const
		{
			map_ptr p = find(this, k);
			if (!p) return null_hack();
			return p->data;
		}


		inline bool insert(const K &k, const V &v)
		{
			map_ptr p = allocate(this, k);
			if (!p) return false;
			p->key = const_cast<K>(k);
			p->data = const_cast<V>(v);
			return true;
		}


		iter_type inorder() { iter_type t(this); return t; }

		// ---------------------------------------------------------
		//
		// XCDR sizing interface
		//
		// ---------------------------------------------------------

		inline std::size_t count() const
		{
			std::size_t ct = 0;
			if (key) ct++;
			if (left) ct += left->count();
			if (middle) ct += middle->count();
			if (right) ct += right->count();
			return ct;
		}

		inline std::size_t size() const
		{
			std::size_t sz = sizeof(map_type);
			if (key) for (K k = key; *k != 0; k++, sz += sizeof(K));
			// XXX: value size hardcoded!
			if (data) for (V v = data; *v != 0; v++, sz += sizeof(char));
			if (left) sz += left->size();
			if (middle) sz += middle->size();
			if (right) sz += right->size();
			return sz;
		}
	};

	template <typename K, typename D>
	struct set : public _ttree_base<set<K,D>,K,D>
	{
	protected:
		// typedefs

		typedef set<K,D>* set_ptr;
		typedef set<K,D> set_type;

	public:
		set() : _ttree_base<set_type,K,D>()
		{
		}
		
		set(D s) : _ttree_base<set_type,K,D>(s)
		{
		}
		
		virtual ~set()
		{
		}

		inline bool has_key(const K& k) const
		{
			if (find(this, k))
				return true;

			return false;
		}

		inline bool insert(const K &k)
		{
			set_ptr p = allocate(this, k);
			if (!p) return false;
			p->key = const_cast<K>(k);
			return true;
		}

		inline set_ptr p_insert(const K &k)
		{
			set_ptr p = allocate(this, k);
			if (!p) return 0;
			p->key = const_cast<K>(k);
			return p;
		}


		// ---------------------------------------------------------
		//
		// XCDR sizing interface
		//
		// ---------------------------------------------------------

		inline std::size_t count() const
		{
			std::size_t ct = 0;
			if (key) ct++;
			if (left) ct += left->count();
			if (middle) ct += middle->count();
			if (right) ct += right->count();
			return ct;
		}

		inline std::size_t size() const
		{
			std::size_t sz = sizeof(set_type);
			if (key) for (K k = key; *k != 0; k++, sz += sizeof(K));
			if (left) sz += left->size();
			if (middle) sz += middle->size();
			if (right) sz += right->size();
			return sz;
		}
	};
}


XCDR_NAMESPACE_END

#endif // XCDR_COMMON_TTREE_H
