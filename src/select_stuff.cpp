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
	//tv.tv_sec = 5;
	//tv.tv_sec = 2;
	//tv.tv_sec = 10;
	tv.tv_usec = 0;

	//bool any_ignored_last_line = false;

	// Clear the set ahead of time
	FD_ZERO(readfds);

	int amount_for_wants_select = 0;

	// Find highest
	for (auto iter : comm_vec)
	{
		if (!iter->__state.did_connect)
		{
			continue;
		}
		if (iter->__state.wants_select)
		{
			//printout("wants_select\n\n");
			FD_SET(iter->sock_fd(), readfds);
			if (nfds < iter->sock_fd())
			{
				++amount_for_wants_select;
				nfds = iter->sock_fd();
			}

			//if (iter->__state.ignored_last_line)
			//{
			//	any_ignored_last_line = true;
			//	tv.tv_sec = 0;
			//	tv.tv_usec = 20'000;
			//}
		}
		else
		{
			//printout("!wants_select\n\n");
		}
	}

	// We need nfds to be one more than highest socket number
	++nfds;

	if (nfds > 1)
	{
		rv = select(nfds, readfds, NULL, NULL, &tv);

		//if (rv != amount_for_wants_select)
		//if (rv == 0)
		if ((rv == 0) && (amount_for_wants_select != 0))
		{
			// Timeout was reached, so send pings.
			for (auto iter : comm_vec)
			{
				//if (iter->__state.did_joins)
				//if (iter->__state.wants_select && iter->__state.did_joins)
				if (iter->__state.did_connect && iter->__state.wants_select
					&& iter->__state.did_joins)
				{
					iter->check_timeout_with_ping();
				}
				//iter->check_timeout_with_ping();
			}
		}
		 

		if (rv == -1)
		{
			err("do_select_for_read():  There was an error in select()!");
		}
	}
}

} // namespace saveybot

