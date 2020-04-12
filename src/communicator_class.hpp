// This file is part of SaveyBot.
// 
// Copyright 2017-2020 Andrew Clark (FL4SHK).
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


#ifndef src_communicator_class_hpp
#define src_communicator_class_hpp

// src/communicator_class.hpp


//#include "json_stuff.hpp"
#include "misc_includes.hpp"

namespace saveybot
{

class SaveyBot;

// Here is the base class for the various protocols, including outputting
// to the command line.
class Communicator
{
public:		// enums
	enum class CommType
	{
		Irc,
		Discord,
	};

public:		// static variables
	static const std::string config_file_name;

private:		// variables
	// This is controlled by a SaveyBot instance, but it isn't relevant for
	// DiscordCommunicator.
	std::string _channel;

protected:		// variables
	SaveyBot* _bot_ptr;

protected:		// functions
	// In IRC's case, send a PRIVMSG 
	virtual void inner_send_regular_msg(std::string&& full_msg) = 0;

	SaveyBot& bot() const;


public:		// functions
	Communicator(SaveyBot* s_bot_ptr);
	virtual inline ~Communicator()
	{
	}

	template<typename FirstType, typename... RemArgTypes>
	void send_regular_msg(const FirstType& first_val, 
		const RemArgTypes&... rem_args)
	{
		std::string orig_to_send, to_send;

		if (comm_type() == CommType::Discord)
		{
			orig_to_send += "``` ";
		}

		orig_to_send += sconcat(first_val, rem_args...);


		if (comm_type() == CommType::Discord)
		{
			orig_to_send += " ```";

		}


		for (auto iter : orig_to_send)
		{
			//if ((comm_type() == CommType::Discord)
			//	&& ((iter == '\"') || (iter == '\\')
			//	|| (iter == '`')))
			if ((comm_type() == CommType::Discord)
				&& ((iter == '\"') || (iter == '\\')))
			{
				to_send += '\\';
				//if (iter == '`')
				//{
				//	to_send += '\\';
				//}
			}
			to_send += iter;
		}

		//if (comm_type() == CommType::Discord)
		//{
		//	to_send += "\n\\`\\`\\`";
		//}


		printout("Communicator::send_regular_msg():  ", to_send, "\n");
		inner_send_regular_msg(std::move(to_send));

	}

	template<typename FirstType, typename... RemArgTypes>
	inline void send_saveybot_msg(const FirstType& first_val,
		const RemArgTypes&... rem_args)
	{
		send_regular_msg("~ ", first_val, rem_args...);
		//send_regular_msg(first_val, rem_args...);
	}

	virtual CommType comm_type() const = 0;

	GEN_SETTER_BY_RVAL_REF(channel);
	GEN_GETTER_AND_SETTER_BY_CON_REF(channel);
};

} // namespace saveybot




#endif		// src_communicator_class_hpp
