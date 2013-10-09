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
// $Revision: 1.2 $
// $Date: 2003/04/06 00:24:29 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: ttree.h,v $
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
#include <cstdlib>

XCDR_NAMESPACE_BEGIN

const unsigned int SK5=SK5=0x55555555,SK3=0x33333333;
const unsigned int SKF0=0xF0F0F0F,SKFF=0xFF00FF;

int CTPop(int map)
{
	map -= ((map >> 1) & SK5);
	map = (map & SK3) + ((map >> 2) & SK3);
	map = (map * SKF0) + ((map >> 4) & SKF0);
	map += map >> 8;
	return (map + (map >> 16)) & 0x3F;
}

unsigned int hash_key(char *key)
{
	unsigned int a=31415, b=27183;
	unsigned int vh;
	for (vh=0; *key; key++, a *= b)
		vh = (a * vh + (*key));
	return vh;
}

unsigned int rehash_key(char *key, int level)
{
	unsigned int a=31415, b=27183;
	unsigned int vh;
	for (vh=0; *key; key++, a *= b)
		vh = (a * vh * level + (*key));
	return vh;
}


namespace hamt
{
	template <typename K, typename D>
	struct entry
	{
		entry *next;
		K key;
		D data;
	};

	typedef <template D
}

