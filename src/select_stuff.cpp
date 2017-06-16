// This file is part of NeoSaveyBot.
// 
// Copyright 2017 Andrew Clark (FL4SHK).
// 
// NeoSaveyBot is free software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as published
// by the Free Software Foundation, either version 3 of the License, or (at
// your option) any later version.
// 
// NeoSaveyBot is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
// 
// You should have received a copy of the GNU General Public License along
// with NeoSaveyBot.  If not, see <http://www.gnu.org/licenses/>.


#include "select_stuff.hpp"

namespace neosaveybot
{

void do_select_for_read(const std::vector<IRCCommunicator>& comm_vec, 
	fd_set* readfds)
{
	int nfds = 0;

	int rv;
	
	timeval tv;
	tv.tv_sec = 0;

	// Stop waiting after 10 milliseconds
	tv.tv_usec = 10000;

	// Clear the set ahead of time
	FD_ZERO(readfds);

	// Find highest
	for (auto& iter : comm_vec)
	{
		FD_SET(iter.sock_fd(), readfds);
		if (nfds < iter.sock_fd())
		{
			nfds = iter.sock_fd();
		}
	}

	rv = select(nfds, readfds, NULL, NULL, &tv);

	if (rv == -1)
	{
		err("There was an error in select()!\n");
	}
}

}
