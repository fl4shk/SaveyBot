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


#ifndef select_stuff_hpp
#define select_stuff_hpp

#include "irc_communicator_class.hpp"

namespace neosaveybot
{

void do_select_for_read(const std::vector<IRCCommunicator>& comm_vec, 
	fd_set* readfds);

inline void do_select_for_read(const IRCCommunicator& comm, 
	fd_set* readfds)
{
	std::vector<IRCCommunicator> temp_vec;
	temp_vec.push_back(comm);

	do_select_for_read(temp_vec, readfds);
}

inline auto check_select_result(const IRCCommunicator& comm,
	fd_set* readfds)
{
	return FD_ISSET(comm.sock_fd(), readfds);
}

}



#endif		// select_stuff_hpp
