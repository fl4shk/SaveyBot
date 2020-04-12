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

#ifndef src_discord_communicator_class_hpp
#define src_discord_communicator_class_hpp

// src/discord_communicator_class.hpp

#include "misc_includes.hpp"
#include "sleepy_discord/websocketpp_websocket.h"
//#include "real_main_class.hpp"
#include "communicator_class.hpp"

namespace saveybot
{

class DiscordCommunicator final : virtual public Communicator,
	virtual public SleepyDiscord::DiscordClient
{

friend class RealMain;

private:		// variables
	static Json::Value _json_config;

	// Received message
	SleepyDiscord::Message* _recv_msg;

public:		// functions
	//using SleepyDiscord::DiscordClient::DiscordClient;

	//void onMessage(SleepyDiscord::Message message)
	//{
	//	if (message.startsWith("whcg hello"))
	//	{
	//		sendMessage(message.channelID, "Hello "
	//			+ message.author.username);
	//	}
	//}

	DiscordCommunicator(SaveyBot* s_bot_ptr, size_t s_num_threads=3);

	void onMessage(SleepyDiscord::Message message);

	virtual CommType comm_type() const
	{
		return CommType::Discord;
	}

protected:		// functions
	static Json::Value& get_config();
	static const std::string get_token_from_config_file();
	static bool get_discord_enabled_from_config_file();

	virtual void inner_send_regular_msg(std::string&& full_msg);
	inline std::string full_name_of_recv_msg_sender()
	{
		return (_recv_msg->author.username
			+ "#" + _recv_msg->author.discriminator
			+ "#" + ((const std::string)_recv_msg->author.ID));
	}
};

} // namespace saveybot

#endif		// src_discord_communicator_class_hpp

