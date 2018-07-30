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

#include "discord_communicator_class.hpp"

#include "saveybot_class.hpp"

namespace saveybot
{

DiscordCommunicator::DiscordCommunicator(SaveyBot* s_bot_ptr,
	size_t s_num_threads)
	: Communicator(s_bot_ptr),
	SleepyDiscord::DiscordClient(get_token_from_config_file(),
	s_num_threads)
{
}

void DiscordCommunicator::onMessage(SleepyDiscord::Message message)
{
	__recv_msg = &message;
	const std::string& name = full_name_of_recv_msg_sender();
	//const std::string& channel
	//	= ((const std::string)__recv_msg->channelID);

	// Don't really need to care about the channel with the
	// DiscordCommunicator, but the SaveyBot class will still try to do
	// this.

	printout("DiscordCommunicator::onMessage():  ", name, ":  ",
		__recv_msg->content, ":  ", __recv_msg->length(), "\n");

	Communicator::bot().parse_command(*this, "", name,
		__recv_msg->content);
}
const std::string DiscordCommunicator::get_token_from_config_file()
{
	Json::Value config_root, config;
	std::string errs;
	parse_json(saveybot::Communicator::config_file_name, &config_root,
		&errs);

	config = config_root["discord"];
	return config["token"].asString();
}
void DiscordCommunicator::inner_send_regular_msg(std::string&& full_msg)
{
	//printout("DiscordCommunicator::inner_send_regular_msg():  ", full_msg,
	//	"\n");
	if (full_msg.size() != 0)
	{
		sendMessage(__recv_msg->channelID, std::string(full_msg.c_str()));

		// "Slowdown" unique to Discord.
		// Prevent bypassing the rate limiting stuff.
		usleep(850000);
	}
	else // if (full_msg.size() == 0)
	{
		printerr("DiscordCommunicator::inner_send_regular_msg():  ",
			"blank!\n");
	}
}

} // namespace saveybot
