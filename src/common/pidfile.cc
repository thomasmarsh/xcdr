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
// $Source: /usr/local/cvs/xcdr/src/common/pidfile.cc,v $
// $Revision: 1.3 $
// $Date: 2003/04/04 16:06:36 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: pidfile.cc,v $
// Revision 1.3  2003/04/04 16:06:36  tmarsh
// namespace changes
//
// Revision 1.2  2003/04/02 20:35:51  tmarsh
// added XCDR namespacing; eradicated all using namespace std
//
// Revision 1.1  2003/03/09 08:26:12  tmarsh
// include file reorganization
//
//
// =========================================================================


#include "common/except.h"
#include "common/util.h"

extern "C"
{
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
}

#include "common/pidfile.h"
#include "common/logging.h"

XCDR_NAMESPACE_BEGIN

PIDFile::PIDFile()
{
}

void PIDFile::create(const std::string &name, const unsigned int id)
{
	struct flock fl;

	std::string pid_path(Util::getenv("HOME")
			+ "/var/run/" + name
			+ "." + Util::itoa(id)
			+ ".pid");

	INFO("Creating pid file " << pid_path);


	fd = open(pid_path.c_str(), O_WRONLY | O_TRUNC | O_CREAT, 0644);
	fl.l_whence = SEEK_SET;
	fl.l_start = 0;
	fl.l_len = 0;
	fl.l_type = F_WRLCK;
	INFO("Locking pid file");
	int status = fcntl(fd, F_SETLK, &fl);

	if (status)
	{
		fcntl(fd, F_GETLK, &fl);

		std::string exc = "Cannot attain file lock for file: " + pid_path;
		exc += "; held by process " + Util::itoa(fl.l_pid);

		ERROR(exc);

		throw new Exception(exc);
	}
	write();
}

void PIDFile::write()
{
	std::string pid = Util::itoa(getpid());
	::write(fd, pid.c_str(), pid.length());
}

XCDR_NAMESPACE_END
