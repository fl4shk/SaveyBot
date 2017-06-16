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
protected:		// variables
	

protected:		// functions
	virtual void inner_send_privmsg(const std::string& channel, 
		std::string&& full_msg) = 0;

	//inline void concat_msg(std::string& full_msg)
	//{
	//}

	//template<typename FirstType, typename... ArgTypes>
	//void concat_msg(std::string& full_msg, const FirstType& first_val,
	//	ArgTypes&&... rem_args)
	//{
	//	std::stringstream sstm;
	//	sstm << first_val;
	//	sstm >> full_msg;
	//	concat_msg(full_msg, rem_args...);
	//}

public:		// functions
	template<typename FirstType, typename... RemArgTypes>
	void send_privmsg(const std::string& channel, 
		const FirstType& first_val, RemArgTypes&&... rem_args)
	{
		inner_send_privmsg(channel, std::move(sconcat(first_val, 
			rem_args...)));
	}
	
};

}



#endif		// communicator_class_hpp
