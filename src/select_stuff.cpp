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
	fd_set* readfds, bool was_called_for_just_one)
{
	int nfds = 0;

	int rv;
	
	timeval tv;
	//tv.tv_sec = 0;
	//tv.tv_sec = 90;

	if (was_called_for_just_one)
	{
		tv.tv_sec = 5;
		tv.tv_usec = 0;
	}
	else // if (!was_called_for_just_one)
	{
		tv.tv_sec = 0;
		tv.tv_usec = 20'000;
	}

	// Clear the set ahead of time
	FD_ZERO(readfds);

	// Find highest
	for (auto iter : comm_vec)
	{
		FD_SET(iter->sock_fd(), readfds);
		if (nfds < iter->sock_fd())
		{
			nfds = iter->sock_fd();
			//printout("sock_fd():  ", nfds, "\n");
		}
	}

	// We need nfds to be one more than highest socket number
	++nfds;

	rv = select(nfds, readfds, NULL, NULL, &tv);

	if (was_called_for_just_one)
	{
		if (comm_vec.size() != 1)
		{
			err("do_select_for_read():  was_called_for_just_one Eek!");
		}

		// Timeout was reached, so send a ping.
		if (rv == 0)
		{
			comm_vec.front()->check_timeout_with_ping();
		}
	}

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
