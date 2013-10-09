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
//
// =========================================================================

#define ENABLE_NLS
#include <gnome.h>
#include <gnome--/client.h>
#include <gnome--/main.h>
#include <iostream>

#include "application.h"
using namespace std;

/*** gnomehello-popttable */
static int greet_mode = FALSE;
static char* message  = "";
static char* geometry = "";

struct poptOption options[] = {
  {
    "greet",
    'g',
    POPT_ARG_NONE,
    &greet_mode,
    0,
    N_("Say hello to specific people listed on the command line"),
    NULL
  },
  { 
    "message",
    'm',
    POPT_ARG_STRING,
    &message,
    0,
    N_("Specify a message other than \"Hello, World!\""),
    N_("MESSAGE")
  },
  { 
    "geometry",
    '\0',
    POPT_ARG_STRING,
    &geometry,
    0,
    N_("Specify the geometry of the main window"),
    N_("GEOMETRY")
  },
  {
    NULL,
    '\0',
    0,
    NULL,
    0,
    NULL,
    NULL
  }
};
/* gnomehello-popttable ***/

class Gnome_XCDRMon : public SigC::Object
// : public Gnome_Client - you wish, but you can't, because you need
// to get the master client, that is you need to specifically
// initialize yourself to gnome_master_client().
{
public:

  Gnome_XCDRMon();

protected:
  // sig handlers

  void session_die(gpointer client_data);

  gint save_session(gint phase, 
		    GnomeSaveStyle save_style,
		    gint is_shutdown, GnomeInteractStyle interact_style,
		    gint is_fast, gpointer client_data);

protected:
  Gnome::Client *client_;
  
};

Gnome_XCDRMon::Gnome_XCDRMon()
  : client_(Gnome::Client::master_client())
{
//   client_->save_yourself.connect(SigC::slot(this, &Gnome_XCDRMon::save_session));
//   client_->die.connect(SigC::slot(this, &Gnome_XCDRMon::session_die));
}

int 
main(int argc, char* argv[])
{
  // gnomehello-parsing */

//   bindtextdomain("GnomeXCDRMon", "");  // GNOMELOCALEDIR
//   textdomain("GnomeXCDRMon");

  poptContext pctx;

  Gnome::Main gnomeMain("GnomeXCDRMon", "0.1", argc, argv, 
		       options, 0, &pctx);  

  // Argument parsing
  //
  const char** args;

  vector<string> greet;

  args = poptGetArgs(pctx);

  if (greet_mode && args)
    {
      int i = 0;
      while (args[i] != NULL) 
        {
          greet.push_back(args[i]);
          ++i;
        }
    }
  else if (greet_mode && args == NULL)
    {
      g_error(_("You must specify someone to greet."));
    }
  else if (args != NULL)
    {
      g_error(_("Command line arguments are only allowed with --greet."));
    }
  else
    { 
      g_assert(!greet_mode && args == NULL);
    }

  poptFreeContext(pctx);


  // gnomehello-client
  //
  Gnome_XCDRMon client;

  
  // Main app
  //
  geometry = "";
  XCDRMon* myApp=manage(new XCDRMon(message, geometry, greet));

//   g_slist_free(greet);

  gnomeMain.run();

  return 0;
  /* gnomehello-main ***/
}

// gnomehello-save-session
gint // should be bool
Gnome_XCDRMon::save_session(gint phase, GnomeSaveStyle save_style,
			  gint is_shutdown, GnomeInteractStyle interact_style,
			  gint is_fast, gpointer client_data)
{
  cout << "save_session" <<endl;
  vector<string> argv(4);

  argv.push_back(static_cast<char*>(client_data)); // yuck

  if (message)
    {
      argv.push_back("--message");
      argv.push_back(message);
    }
  
  client_->set_clone_command (argv);
  client_->set_restart_command (argv);

  return true;
}


// gnomehello-session-die */
void
Gnome_XCDRMon::session_die(gpointer client_data)
{
  cout << "session_die" <<endl;
}
