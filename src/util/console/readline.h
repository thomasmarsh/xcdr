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
// $Source: /usr/local/cvs/xcdr/src/util/console/readline.h,v $
// $Revision: 1.2 $
// $Date: 2003/04/04 16:06:36 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: readline.h,v $
// Revision 1.2  2003/04/04 16:06:36  tmarsh
// namespace changes
//
// Revision 1.1  2003/03/18 17:48:10  tmarsh
// added
//
//
// =========================================================================

#ifndef XCDR_UTIL_READLINE_H
#define XCDR_UTIL_READLINE_H

#include <cstdlib>
#include <string>

#include <readline/readline.h>
#include <readline/history.h>

#undef RETURN

XCDR_NAMESPACE_BEGIN

using namespace std;

class History
{
public:
	History()
	{
		using_history();
	}

	void setFile(const string &file)
	{
		path = file;
	}

	void read()
	{
		read_history(path.c_str());
	}

	void write()
	{
		write_history(path.c_str());
	}

	void add(const char *line)
	{
		add_history(line);
	}

private:
	string path;
};

class Readline
{
public:
	Readline(const string &history_file = "/home/tmarsh/.xcdr_history")
	{
		history.setFile(history_file);
		history.read();
		line = 0;
		_eof = false;
	}

	~Readline()
	{
		history.write();
	}

	bool eof() { return _eof; }

	string readline(const string &prompt)
	{
		free(line); line = 0;

		line = ::readline(prompt.c_str());

		if (line == 0)
		{
			_eof = true;
			return "";
		}

		if (line && *line)
			history.add(line);

		return string(line);
	}

private:
	History history;
	char *line;
	bool _eof;
};

XCDR_NAMESPACE_END

#endif // XCDR_UTIL_READLINE_H
