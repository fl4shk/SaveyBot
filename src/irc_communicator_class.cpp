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


#include "irc_communicator_class.hpp"

namespace neosaveybot
{

IRCConfiguration::IRCConfiguration()
{
	Json::Value config_root;
	std::string errs;

	parse_json(IRCCommunicator::config_file_name, &config_root, &errs);

	const auto& member_names = config_root.getMemberNames();

	for (const auto& name_iter : member_names)
	{
		Server to_push;

		to_push.name = name_iter;
		const auto& iter = config_root[name_iter];
		to_push.bot_name = iter["bot_name"].asString();
		to_push.address = iter["address"].asString();
		to_push.port_str = iter["port"].asString();
		to_push.password = iter["password"].asString();

		for (const auto& join_iter : iter["joins_list"])
		{
			to_push.joins_list.push_back(join_iter.asString());
		}

		for (const auto& command_iter : iter["startup_commands"])
		{
			std::string full_command;

			for (const auto& inner_iter : command_iter)
			{
				full_command += inner_iter.asString();
			}

			to_push.startup_commands.push_back(std::move(full_command));
		}

		server_vec.push_back(to_push);
	}

	for (const auto& iter : server_vec)
	{
		printout(iter);
	}
	printout("\n");

}

std::ostream& operator << (std::ostream& os, 
	const IRCConfiguration::Server& to_print)
{
	osprintout(os, "name:  ", to_print.name, "\n");
	osprintout(os, "\tbot_name:  ", to_print.bot_name, "\n");
	osprintout(os, "\taddress:  ", to_print.address, "\n");
	osprintout(os, "\tport_str:  ", to_print.port_str, "\n");
	osprintout(os, "\tpassword:  ", to_print.password, "\n");

	osprintout(os, "\tjoins_list:  \n");
	for (const auto& iter : to_print.joins_list)
	{
		osprintout(os, "\t\t", iter, "\n");
	}

	osprintout(os, "\tstartup_commands:  \n");
	for (const auto& iter : to_print.startup_commands)
	{
		osprintout(os, "\t\t", iter, "\n");
	}

	return os;
}

const std::string IRCCommunicator::config_file_name("config.json");


IRCCommunicator::IRCCommunicator(const std::string& some_server_name, 
	const std::string& some_port_str, const std::string& nick_command,
	const std::string& user_command, 
	const std::vector<std::string>& joins_list)
{
	do_getaddrinfo(some_server_name, some_port_str);
	do_socket_and_connect();

	// Go ahead and do this now 
	free_res();

	
}

void IRCCommunicator::clean_up()
{
	free_res();
	close_sock_fd();
}


void IRCCommunicator::do_getaddrinfo(const std::string& some_server_name,
	const std::string& some_port_str)
{
	memset(&__hints, 0, sizeof(__hints));
	__hints.ai_family = specific_family();
	__hints.ai_socktype = specific_socktype();

	if (getaddrinfo(some_server_name.c_str(), some_port_str.c_str(), 
		&hints(), &res()) != 0)
	{
		printerr("There was an error getting address information.\n");
		set_did_open_sock_fd(false);
		clean_up();
		exit(1);
	}

	set_did_alloc_res(true);
}

void IRCCommunicator::do_socket_and_connect()
{
	// The last parameter of socket() is set to zero because doing so
	// causes socket() to use an unspecified default protocol appropriate
	// for the requested socket type (SOCK_STREAM).
	set_sock_fd(socket(res()->ai_family, specific_socktype(), 0));
	
	if (connect(sock_fd(), res()->ai_addr, res()->ai_addrlen) != 0)
	{
		printerr("There was an error connecting to the server.\n");
		set_did_open_sock_fd(false);
		clean_up();
		exit(1);
	}
}

void IRCCommunicator::inner_send_msg(const std::string& channel, 
	std::string&& full_msg)
{
	
}


}
