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
// $Source: /usr/local/cvs/xcdr/src/include/rater/digit.h,v $
// $Revision: 1.4 $
// $Date: 2003/05/14 11:35:40 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: digit.h,v $
// Revision 1.4  2003/05/14 11:35:40  tmarsh
// replacing DigitNode with dtrie
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

#ifndef XCDR_ENGINE_RATER_DIGIT_H
#define XCDR_ENGINE_RATER_DIGIT_H

#include "common/common.h"
#include "common/dtrie.h"

#if 0
#include <cstddef>
#include <cstring>

#ifdef NEW_IDENTIFIERS
#include "rater/ident.h"
#endif // NEW_IDENTIFIERS
#endif // 0

XCDR_NAMESPACE_BEGIN

#define DigitNode dtrie::digit_map

#if 0
template <typename T>
struct DigitNode
{
	DigitNode **children;
#ifdef NEW_IDENTIFIERS
	T::ValueType value;
#else
	T value;
#endif // NEW_IDENTIFIERS

	DigitNode()
	{
		children = 0;
		value = 0;
	}

	~DigitNode()
	{
		if (children)
		{
			for (register int i = 0; i < 10; i++)
				delete children[i];

			delete [] children;
		}
	}

	inline void allocateChildren()
	{
		children = new DigitNode*[10];
		for (register int i = 0; i < 10; i++)
			children[i] = new DigitNode;
	}

	void push(const char *dest, const T v)
	{
		// assert all 0-9

		_push(const_cast<T>(dest), const_cast<T>(v));
	}

	void _push(char *dest, T v)
	{
		register DigitNode *current = this;
		register char c = *dest;

		while (1)
		{
			if (*dest == '\0')
			{
				current->value = v;
				return;
			}

			if (!current->children)
				current->allocateChildren();

			c = *dest++;
			current = current->children[c-'0'];
		}
	}

	T find(char *dest)
	{
		register DigitNode *current = this;
		register T found = 0;
		register char c = '?';

		while (1)
		{
			if (current->value != 0)
				found = current->value;

			if ((*dest == '\0') || ((current->children) == 0))
				break;
			else
			{
				c = *dest++;
				current = current->children[c-'0'];
			}

		}
		return found;
	}

	std::size_t size();
	std::size_t count();
};

inline std::size_t DigitNode<char*>::size()
{
	std::size_t amount = sizeof(DigitNode<char*>);

	if (value)
		amount += (sizeof(char) * std::strlen(value)) + 1;

	if (children)
	{
		for (register int i = 0; i < 10; i++)
		{
			amount += children[i]->size();
		}
	}
	return amount;
}

inline std::size_t DigitNode<char*>::count()
{
	std::size_t amount = 0;
	if (value)
		amount++;

	if (children)
	{
		for (register int i = 0; i < 10; i++)
		{
			amount += children[i]->count();
		}
	}
	return amount;
}

#endif // 0
XCDR_NAMESPACE_END

#endif // XCDR_ENGINE_RATER_DIGIT_H
