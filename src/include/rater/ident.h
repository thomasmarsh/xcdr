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
// $Source: /usr/local/cvs/xcdr/src/include/rater/ident.h,v $
// $Revision: 1.4 $
// $Date: 2003/04/04 16:03:58 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: ident.h,v $
// Revision 1.4  2003/04/04 16:03:58  tmarsh
// namespace changes
//
// Revision 1.3  2003/03/28 17:46:24  tmarsh
// major updates; rater now works
//
// Revision 1.2  2003/03/27 19:15:22  tmarsh
// rater rewrite
//
//
// =========================================================================

#ifndef XCDR_RATER_IDENT_H
#define XCDR_RATER_IDENT_H

#include "common/slist.h"

#include <cstring>

XCDR_NAMESPACE_BEGIN

#ifdef NEW_IDENTIFIERS
namespace Ident
{
	class IdentifierBase
	{
	public:
		virtual std::size_t count() const = 0;
		virtual std::size_t deepCount() const = 0;
		virtual std::size_t size() const = 0;
		virtual bool match(IdentifierBase &other) const = 0;
	};

	enum ListMode
	{
		L_OR = 0,
		L_AND = 1
	};

	class IdentifierList : public list<IdentifierBase>,
			       public IdentifierBase
	{
	public:
		IdentifierList() : list<IdentifierBase>()
		{
		}

		bool match(IdentifierList &other) const
		{
			bool found = false;
			IdentifierList::iterator i = begin();
			while (i != end())
			{
				IdentifierBase &b = *i;
				if (b.match())
					if (mode == L_OR)
						return true;
				else
					if (mode == L_AND)
						return false;
				i++;
			}
		}
	private:
		ListMode mode;
	};

	template <unsigned int ID, typename T>
	class Identifier : public IdentifierBase
	{
	public:
		typedef T ValueType;
		Identifier(T _value) { value = _value; }
		inline unsigned int getID() { return ID; }
		T getValue() { return value; }

		inline std::size_t count() const { return 1; }
		inline std::size_t deepCount() const { return 1; }

		inline std::size_t size() const;
	private:
		T value;
	};


	template <unsigned int ID, typename T>
	inline std::size_t Identifier<ID, T>::size() const
	{
		std::size_t sz = sizeof(Identifier<ID,T>);
		return sz;
	}

	template <unsigned int ID, char*>
	inline std::size_t Identifier<ID, char*>::size() const
	{
		std::size_t sz = sizeof(Identifier<ID,char*>);
		if (value) sz += sizeof(char) * std::strlen(value);
		return sz;
	}

	typedef Identifier<0, char*> Any;
	typedef Identifier<1, char*> ReportingGateway;
	typedef Identifier<2, char*> IngressIP;
	typedef Identifier<3, char*> EgressIP;
	typedef Identifier<4, char*> IngressTrunk;
	typedef Identifier<5, char*> EgressTrunk;
	typedef Identifier<6, char*> CLD;
	typedef Identifier<7, char*> CLI;
	typedef Identifier<8, char*> OrigCountry;
	typedef Identifier<9, char*> TermCountry;
	typedef Identifier<10, char*> OrigProvince;
	typedef Identifier<11, char*> TermProvince;
	typedef Identifier<12, char*> OrigCity;
	typedef Identifier<13, char*> TermCity;
	typedef Identifier<14, char*> OrigLATA;
	typedef Identifier<15, char*> TermLATA;
	typedef Identifier<16, char*> OrigOCN;
	typedef Identifier<17, char*> TermOCN;
	typedef Identifier<18, bool> Payphone;
	typedef Identifier<19, bool> Prison;
	typedef Identifier<20, unsigned int> Activity;
}
#endif // NEW_IDENTIFIERS

struct Identifier
{
	enum Type
	{
		ANY=0,
		REPORTING_GW=1,
		INGRESS_IP=2,
		EGRESS_IP=3,
		INGRESS_TRUNK=4,
		EGRESS_TRUNK=5,
		DIALED_NUMBER=6,
		ORIGIN_NUMBER=7,
		ORIG_COUNTRY=8,
		TERM_COUNTRY=9,
		ORIG_PROVINCE=10,
		TERM_PROVINCE=11,
		ORIG_CITY=12,
		TERM_CITY=13,
		ORIG_LATA=14,
		TERM_LATA=15,
		ORIG_OCN=16,
		TERM_OCN=17,
		PAYPHONE=18,
		PRISON=19,
		ACTIVITY=20
	};

	Type i_type;
	char *value;
	union _value
	{
		char *char_value;
		bool bool_value;
		unsigned int uint_value;
	};

	Identifier()
	{
		i_type = ANY;
		value = 0;
	}

	~Identifier()
	{
		if (value) delete value;
	}

	inline std::size_t count() const { return 1; }
	inline std::size_t deepCount() const { return 1; }

	inline std::size_t size() const
	{
		std::size_t sz = sizeof(Identifier);
		if (value) sz += sizeof(char) * std::strlen(value);
		return sz;
	}

	inline bool match(Identifier *other) const
	{
		return ((i_type == other->i_type) &&
			(((value == 0) && (other->value == 0)) ||
			 (strcmp(value, other->value) == 0)));
	}
};

typedef slist<Identifier> IdentifierList;

bool hasMatch(const IdentifierList &a, const IdentifierList &b);

XCDR_NAMESPACE_END

#endif // XCDR_RATER_IDENT_H
