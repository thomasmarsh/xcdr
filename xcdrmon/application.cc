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

/*** gnomehello-app */
#define ENABLE_NLS
#include <cstdio>

#include <algorithm>

#include <gnome.h>
#include <libgnome/gnome-i18n.h>
#include <gtk--/menu.h>
#include <gtk--/menubar.h>
#include <gnome--/main.h>
#include <gnome--/about.h>
#include <gnome--/dialog.h>
#include "application.h"

// Keep a list of all open application windows

list<XCDRMon*> XCDRMon::instances_;

XCDRMon::XCDRMon(const string &message)
  : Gnome::App("XCDRMon", "XCDRMon"),
           label_(_("Hello, World!")),
           status_(false, true, GNOME_PREFERENCES_NEVER),
           about_(0)
{
  init();
  show_all();
}

XCDRMon::XCDRMon(const string &message, 
		     const string &geometry,
		     const vector<string> &greet)
  : Gnome::App("XCDRMon", "XCDRMon"),
           status_(false, true, GNOME_PREFERENCES_NEVER),
           about_(0)
{

  if(message.length())
    label_.set_text(message);
  else
    label_.set_text("Hello, World!");

  // gnomehello-widgets
  //
  init();

  // gnomehello-geometry
  //
  if (geometry.length()) 
    {
      gint x, y, w, h;
      if ( gnome_parse_geometry( geometry.c_str(), 
                                 &x, &y, &w, &h ) ) 
        {
          if (x != -1)
            set_uposition(x, y);

          if (w != -1) 
            set_default_size(w, h);
        }
      else 
        g_error(_("Could not parse geometry string `%s'"), geometry.c_str());
    }
  
  // Greetings
  //
  if (greet.size())
    {
      string greetings(_("Special Greetings to:\n"));
      vector<string>::const_iterator i;
      for(i = greet.begin(); i != greet.end(); i++)
        {
          greetings += *i;
          greetings += '\n';
        }
      
      Gnome::Dialog *dialog = Gnome::Dialogs::ok(greetings);
      dialog->set_parent(*this);
    }

  show_all();
  
}

XCDRMon::~XCDRMon()
{
  instances_.remove(this);
  if(instances_.empty())
    Gtk::Main::quit();      
}


void
XCDRMon::init()
{
  set_policy(false, true, false);
  set_default_size(250, 350);
  set_wmclass("hello", "XCDRMon");

  frame_.set_shadow_type(GTK_SHADOW_IN);

  button_.set_border_width(10);

  button_.add(label_);
  frame_.add(button_);

  set_contents(frame_);

  set_statusbar(status_);

  install_menus_and_toolbar();
  
  // gnomehello-signals
  //
  button_.clicked.connect(bind(slot(this, &XCDRMon::button_click_cb), &label_));

  // Register an instance of myself
  //
  instances_.push_back(this);
}

void
XCDRMon::install_menus_and_toolbar()
{
  vector<Gnome::UI::SubTree> menus;
  vector<Gnome::UI::Info> file_menu, edit_menu, help_menu;
  {
    // This limits use of stock MenuItems to a small scope.
    using namespace Gnome::MenuItems;

    // File menu 
//  list_file.push_back(New(N_("New Hello"), N_("Create a new hello"),
//				       slot(this, &Hello_App::new_app_cb)));
    file_menu.push_back(Open());
    file_menu.push_back(Save());
    file_menu.push_back(SaveAs());
    file_menu.push_back(Gnome::UI::Separator());
    file_menu.push_back(Close(slot(this, &XCDRMon::close)));
    file_menu.push_back(Exit(slot(this, &XCDRMon::exit_cb)));
  
    // Edit Menu (stubs)
    edit_menu.push_back(Cut());
    edit_menu.push_back(Copy());
    edit_menu.push_back(Paste());
    edit_menu.push_back(SelectAll());
    edit_menu.push_back(Clear());
    edit_menu.push_back(Undo());
    edit_menu.push_back(Redo());
    edit_menu.push_back(Find());
    edit_menu.push_back(FindAgain());
    edit_menu.push_back(Replace());
    edit_menu.push_back(Properties());
  
    // Help Menu
      // this pulls in all the helps items  
//    help_menu.push_back(Gnome::UI::Help("Gnome-- hello"));
    help_menu.push_back(About(slot(this, &XCDRMon::about_cb)));
  }

  {
    // This limits use of stock MenuItems to a small scope.
    using namespace Gnome::Menus;
    menus.push_back(File(file_menu));
    menus.push_back(Edit(edit_menu));
    menus.push_back(Help(help_menu));
  }

  create_menus(menus);
  install_menu_hints();


  // Toolbar
  //

  vector<Gnome::UI::Info> toolbar;
  { 
    using namespace Gnome::UI;
    toolbar.push_back(Item(Gnome::UI::Icon(GNOME_STOCK_PIXMAP_NEW),
                           N_("New Hello"),
                           slot(this, &XCDRMon::new_app_cb),
                           N_("Create a new hello")));
    toolbar.push_back(Separator());
    toolbar.push_back(Item(Gnome::UI::Icon(GNOME_STOCK_PIXMAP_BACK),
                           N_("Prev"),
                           slot(this, &XCDRMon::nothing_cb),
                           N_("Previous hello")));
    toolbar.push_back(Item(Gnome::UI::Icon(GNOME_STOCK_PIXMAP_FORWARD),
                           N_("Next"),
                           slot(this, &XCDRMon::nothing_cb),
                           N_("Next hello")));
    create_toolbar(toolbar);
  }

}

//////////////////////////////////////////////////////////////////////
//                Callbacks
//////////////////////////////////////////////////////////////////////


void       
XCDRMon::close()
{
  destroy();
}

gint 
XCDRMon::delete_event_impl(GdkEventAny* e)
{
  return false; 
}

void 
XCDRMon::button_click_cb(Gtk::Label *label)
{
  // reverse the string.
  string s(label->get());
  reverse(s.begin(), s.end());
  label->set(s);
}

void
XCDRMon::nothing_cb()
{
  message(_("This does nothing; it is only a demonstration."));
}


void
XCDRMon::exit_cb()
{
  // we don't want to quite directly as we should save our work
  // therefore we need to send close to each window.
  list<XCDRMon*>::iterator i,j;
  i=instances_.begin();
  while (i!=instances_.end())
    {
      j=i;
      ++i;
      (*j)->close();
    }
}

void
XCDRMon::new_app_cb()
{
  manage(new XCDRMon(_("Hello, World!")));
}

gint
XCDRMon::about_close_cb()
{
  delete about_; //Destroys and closes it.
  about_ = 0;

  return TRUE;
}

void
XCDRMon::about_cb()
{

  if(about_) // "About" box hasn't been closed, so just raise it
    {
      Gdk_Window about_win(about_->get_window());
      about_win.show();
      about_win.raise();
    }
  else
    {
      vector<string> authors;
      authors.push_back("Thomas D. Marsh <tmarsh@portaone.com>");

      // not yet wrapped - sorry
      gchar *logo_char;
      string logo;
      logo_char = gnome_pixmap_file("my_logo.png");

      if (logo_char != NULL)
        logo = logo_char;

      about_ = new Gnome::About(_("XCDRMon"), "0.1",
                               "(C) 2000 PortaOne s.r.o",
                               authors,
                               _("A monitoring application for XCDR."),
                               logo);

      about_->set_parent(*this);
      about_->close.connect(slot(this, &XCDRMon::about_close_cb));
      about_->show();
    }
}

