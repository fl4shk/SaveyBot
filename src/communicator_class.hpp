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


#ifndef communicator_class_hpp
#define communicator_class_hpp



#include "json_stuff.hpp"

namespace neosaveybot
{

class Communicator
{
private:		// variables
	// This is controlled by a NeoSaveyBot instance
	std::string __channel;

protected:		// functions
	virtual void inner_send_privmsg(std::string&& full_msg) = 0;

public:		// functions
	template<typename FirstType, typename... RemArgTypes>
	void send_privmsg(const FirstType& first_val, 
		const RemArgTypes&... rem_args)
	{
		inner_send_privmsg(std::move(sconcat(first_val, rem_args...)));
	}

	gen_setter_by_con_ref(channel);
	gen_setter_by_rval_ref(channel);
	gen_getter_by_con_ref(channel);
	
};

}



#endif		// communicator_class_hpp
