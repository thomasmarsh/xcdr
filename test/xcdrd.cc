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
// $Source: /usr/local/cvs/xcdr/test/xcdrd.cc,v $
// $Revision: 1.2 $
// $Date: 2003/03/06 11:20:05 $
// $Author: tmarsh $
//
// -------------------------------------------------------------------------
//
// $Log: xcdrd.cc,v $
// Revision 1.2  2003/03/06 11:20:05  tmarsh
// updated headers
//
//
// =========================================================================

void executeTasks()
{
	list<Task> task_list = TaskManager.getPending();

	list<Task>::iterator i;

	while (i = task_list.begin(); i != task_list.end(); i++)
	{
		Task current_task = (*i);

		current_task.execute();
	}
}

void checkProcesses()
{
	list<Process> proc_list ProcessManager.getProcesses();

	list<Process>::iterator i;

	while (i = proc_list.begin(); i != proc_list.end(); i++)
	{
		Process current_proc = (*i);

		if (!current_proc.running())
		{
			current_proc.start();
		}
	}
}

int mainloop()
{
	bool shutdown_called = false;

	while (!shutdown_called)
	{
		checkProcesses();

		executeTasks();
	}
}
