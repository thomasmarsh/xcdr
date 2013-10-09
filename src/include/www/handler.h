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
// $Source: /usr/local/cvs/xcdr/src/include/www/handler.h,v $
// $Revision: 1.1 $
// $Date: 2003/03/09 08:35:32 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: handler.h,v $
// Revision 1.1  2003/03/09 08:35:32  tmarsh
// include file reorganization
//
//
// =========================================================================

#ifndef XCDR_WWW_HANDLER_H
#define XCDR_WWW_HANDLER_H

#include "webutil.h"
#include "session.h"

#include "adaptor.h"
#include "model.h"
#include "menu.h"
#include "page.h"
#include "action.h"

class Handler
{
public:
	Handler();

	~Handler();
private:
	SessionMap session_map;
	Model model;
	ActionMap action_map;
};

#endif // XCDR_WWW_HANDLER_H
