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


#ifndef select_stuff_hpp
#define select_stuff_hpp

#include "irc_communicator_class.hpp"

namespace saveybot
{

void do_select_for_read(const std::vector<IrcCommunicator*>& comm_vec, 
	fd_set* readfds, bool was_called_for_just_one=false);

// Wrapper
inline void do_select_for_read
	(const std::vector<IrcCommunicator*>& comm_vec, fd_set& readfds)
{
	do_select_for_read(comm_vec, &readfds);
}

inline void do_select_for_read(IrcCommunicator* comm, fd_set* readfds)
{
	std::vector<IrcCommunicator*> temp_vec;
	temp_vec.push_back(comm);

	do_select_for_read(temp_vec, readfds, true);
}

// Wrapper
inline void do_select_for_read(IrcCommunicator* comm, fd_set& readfds)
{
	do_select_for_read(comm, &readfds);
}


inline auto check_select_result(IrcCommunicator* comm, fd_set* readfds)
{
	return FD_ISSET(comm->sock_fd(), readfds);
}

// Wrapper
inline auto check_select_result(IrcCommunicator* comm, fd_set& readfds)
{
	return check_select_result(comm, &readfds);
}

}



#endif		// select_stuff_hpp
