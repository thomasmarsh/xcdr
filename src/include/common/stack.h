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
// $Source: /usr/local/cvs/xcdr/src/include/common/stack.h,v $
// $Revision: 1.1 $
// $Date: 2003/05/14 11:32:44 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: stack.h,v $
// Revision 1.1  2003/05/14 11:32:44  tmarsh
// added
//
// =========================================================================

#ifndef XCDR_COMMON_STACK_H
#define XCDR_COMMON_STACK_H

#include "common/common.h"
#include "common/slist.h"
#include <cstdlib>

XCDR_NAMESPACE_BEGIN

// -------------------------------------------------------------------------
//
// This is a very simple stack for internal use by ttree and other data
// structures within XCDR (mainly for iteration purposes).
//
// -------------------------------------------------------------------------

template <typename T>
struct stack
{
	typedef stack<T> stack_type;

	stack_type *top;
	stack_type *next;
	stack_type *prev;
	T data;
	unsigned int _size;

	stack() : top(this), next(0), prev(0), data(0), _size(0)
	{
	}

	inline unsigned int size() { return top->_size; }

	inline void push(T t)
	{
		stack<T> *n = new stack<T>;
		n->top = n;
		n->next = 0;
		n->prev = top;
		n->_size = top->_size + 1;
		top->next = n;
		top = n;
		top->data = t;
	}

	inline T pop()
	{
		stack<T> *n = top;
		top = top->prev;
		T ret = n->data;
		delete n;
		return ret;
	}
};

XCDR_NAMESPACE_END

#endif // XCDR_COMMON_STACK_H
