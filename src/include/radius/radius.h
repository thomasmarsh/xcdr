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
// $Source$
// $Revision$
// $Date$
// $Author$
//
// -------------------------------------------------------------------------
//
// $Log$
// =========================================================================

#ifndef XCDR_RADIUS_H
#define XCDR_RADIUS_H

#include "common/common.h"
#include "common/ttree.h"

XCDR_NAMESPACE_BEGIN

namespace Radius
{
	const unsigned int ACCESS_REQUEST	= 1;
	const unsigned int ACCESS_ACCEPT	= 2;
	const unsigned int ACCESS_REJECT	= 3;
	const unsigned int ACCOUNTING_REQUEST	= 4;
	const unsigned int ACCOUNTING_RESPONSE	= 5;
	const unsigned int ACCESS_CHALLENGE	= 11;
	const unsigned int STATUS_SERVER	= 12; // experimental
	const unsigned int STATUS_CLIENT	= 13; // experimental
	const unsigned int RESERVED		= 255;

	class Dictionary
	{
	protected:
		ttree::map<char*, char, int> dict;
		vector<char*> idx;
	public:
		void add(char *value, int index);
	};

	class Request;

	class Session
	{
	protected:
		Dictionary *dictionary;
	public:
		Request *request();
	};

	class Request
	{
	protected:
	public:
		Request()
		{
		}
	};
}


XCDR_NAMESPACE_END

#endif // XCDR_RADIUS_H
