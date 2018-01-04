// This file is part of SaveyBot.
// 
// Copyright 2017-2018 Andrew Clark (FL4SHK).
// 
// SaveyBot is free software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as published
// by the Free Software Foundation, either version 3 of the License, or (at
// your option) any later version.
// 
// SaveyBot is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
// 
// You should have received a copy of the GNU General Public License along
// with SaveyBot.  If not, see <http://www.gnu.org/licenses/>.


#include "select_stuff.hpp"
#include "irc_communicator_class.hpp"

namespace saveybot
{

void do_select_for_read(const std::vector<IrcCommunicator*>& comm_vec, 
	fd_set* readfds)
{
	int nfds = 0;

	int rv;
	
	timeval tv;

	tv.tv_sec = 90;
	tv.tv_usec = 0;

	// Clear the set ahead of time
	FD_ZERO(readfds);

	// Find highest
	for (auto iter : comm_vec)
	{
		if (iter->__state.wants_select)
		{
			printout("wants_select\n");
			FD_SET(iter->sock_fd(), readfds);
			if (nfds < iter->sock_fd())
			{
				nfds = iter->sock_fd();
				//printout("sock_fd():  ", nfds, "\n");
			}
			//++nfds;
		}
		else
		{
			printout("!wants_select\n");
		}
	}

	// We need nfds to be one more than highest socket number
	++nfds;

	//printout("do_select_for_read():  ", nfds, "\n");

	if (nfds != 1)
	{
		rv = select(nfds, readfds, NULL, NULL, &tv);

		//printout("rv, comm_vec.size():  ", strappcom2(rv, comm_vec.size()),
		//	"\n");
		if (rv < comm_vec.size())
		{
			// Timeout was reached, so send pings.
			for (auto iter : comm_vec)
			{
				if (iter->__state.did_joins)
				{
					iter->check_timeout_with_ping();
				}
			}
		}
		// 

		if (rv == -1)
		{
			err("do_select_for_read():  There was an error in select()!");
		}

		//for (auto& iter : comm_vec)
		//{
		//	printout(iter->sock_fd(), "\n");
		//}
	}
}

}
