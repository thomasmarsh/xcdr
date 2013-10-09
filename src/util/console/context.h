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
// $Source: /usr/local/cvs/xcdr/src/util/console/context.h,v $
// $Revision: 1.2 $
// $Date: 2003/04/04 16:06:36 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: context.h,v $
// Revision 1.2  2003/04/04 16:06:36  tmarsh
// namespace changes
//
// Revision 1.1  2003/03/21 11:05:19  tmarsh
// added
//
// Revision 1.2  2003/03/18 17:53:48  tmarsh
// added contexting; major updates
//
// Revision 1.1  2003/03/15 21:11:04  tmarsh
// added
//
//
// =========================================================================

#ifndef XCDR_UTIL_CONTEXT_H
#define XCDR_UTIL_CONTEXT_H

#include "command.h"

XCDR_NAMESPACE_BEGIN

class SetContextCommand : public Command
{
	typedef ConsoleContext::DNode DNode;
public:
	SetContextCommand(ConsoleContext *context) : Command(context) {}

	void execute();
	string usage();
	bool matches(const string &word);
	void setParams(const list<string> &params);
private:
	void gotoRoot();
	void gotoNode(const string &n);
	string path;
};

XCDR_NAMESPACE_END

#endif // XCDR_UTIL_CONTEXT_H
