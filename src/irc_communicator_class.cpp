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


#include "irc_communicator_class.hpp"
#include "saveybot_class.hpp"
#include "select_stuff.hpp"
#include <errno.h>

namespace saveybot
{

IrcConfiguration::IrcConfiguration()
{
	Json::Value config_root, servers;
	std::string errs;

	parse_json(IrcCommunicator::config_file_name, &config_root, &errs);

	servers = config_root["servers"];


	const auto& member_names = servers.getMemberNames();

	for (const auto& name_iter : member_names)
	{
		Server to_push;

		to_push.set_name(name_iter);
		const auto& iter = servers[name_iter];
		to_push.set_bot_name(iter["bot_name"].asString());
		to_push.set_address(iter["address"].asString());
		to_push.set_port_str(iter["port"].asString());
		to_push.set_password(iter["password"].asString());

		for (const auto& join_iter : iter["joins_list"])
		{
			to_push.joins_list().push_back(join_iter.asString());
		}

		for (const auto& command_iter : iter["startup_commands"])
		{
			std::string full_command;

			for (const auto& inner_iter : command_iter)
			{
				full_command += inner_iter.asString();
			}

			to_push.startup_commands().push_back(std::move(full_command));
		}

		__server_vec.push_back(to_push);
	}

	//for (const auto& iter : __server_vec)
	//{
	//	printout(iter);
	//}
	//printout("\n");

}

std::ostream& operator << (std::ostream& os, 
	const IrcConfiguration::Server& to_print)
{
	osprintout(os, "name:  ", to_print.name(), "\n");
	osprintout(os, "\tbot_name:  ", to_print.bot_name(), "\n");
	osprintout(os, "\taddress:  ", to_print.address(), "\n");
	osprintout(os, "\tport_str:  ", to_print.port_str(), "\n");
	osprintout(os, "\tpassword:  ", to_print.password(), "\n");

	osprintout(os, "\tjoins_list:  \n");
	for (const auto& iter : to_print.joins_list())
	{
		osprintout(os, "\t\t", iter, "\n");
	}

	osprintout(os, "\tstartup_commands:  \n");
	for (const auto& iter : to_print.startup_commands())
	{
		osprintout(os, "\t\t", iter, "\n");
	}

	return os;
}

const std::string IrcCommunicator::config_file_name("config.json"),
	IrcCommunicator::msg_suffix("\r\n"),
	IrcCommunicator::ping_suffix(":FL4SHK_IS_A_WALRUS");


//IrcCommunicator::IrcCommunicator(const std::string& some_server_name, 
//	const std::string& some_port_str, const std::string& nick_command,
//	const std::string& user_command, 
//	const std::vector<std::string>& joins_list)
IrcCommunicator::IrcCommunicator(SaveyBot* s_bot_ptr, 
	const IrcConfiguration::Server* s_config_server_ptr)
	: __bot_ptr(s_bot_ptr), __config_server_ptr(s_config_server_ptr)
{
	__reinit();
}

void IrcCommunicator::iterate(fd_set* readfds)
{
	//do_select_and_also_full_read();
	do_full_read_if_fd_isset(readfds);
	update_line();

	if (line().size() == 0)
	{
		//printout("Debug:  AAAA\n");
		return;
	}

	//printout("Here is line():  ", line(), "\n");

	std::string first_substr, second_substr, third_substr,
		other_substr;
	size_t i, temp_i;
	next_non_blank_substr(line(), 0, first_substr, i);

	//printout("substr Debug:  ", line(), "\t\t", first_substr, " ", 
	//	first_substr.size(), "\n");

	auto attempt_do_joins = [&]() -> void
	{
		if (!__state.did_joins)
		{
			sleep(1);

			__state.did_joins = true;

			for (auto iter : config_server().joins_list())
			{
				send_raw_msg("JOIN ", iter);
			}
		}
	};

	// Handle PING
	if (first_substr == "PING")
	{
		next_non_blank_substr(line(), i, second_substr, i);
		//printout("PING Debug:  ", second_substr, "\n");
		send_raw_msg("PONG ", second_substr);

		attempt_do_joins();
	}
	else if (first_substr == "PONG")
	{
		if (__state.did_ping)
		{
			next_non_blank_substr(line(), i, second_substr, i);
			if (second_substr.find(ping_suffix) == 0)
			{
				__state.did_ping = false;
			}
			else
			{
				err("PING PONG Eek 0!");
			}
		}
		else
		{
			err("PING PONG Eek 1!");
		}
	}
	else
	{
		const auto exclam_index = first_substr.find("!");
		const auto space_index = first_substr.find(" ");

		std::string user_nick;

		// Detect if the server was the one sending the message
		if (exclam_index > space_index)
		{
			//printout("exclam_index > space_index\n");
			return;
		}

		// Messages start with ":"
		user_nick = first_substr.substr(1, (exclam_index - 1));

		next_non_blank_substr(line(), i, second_substr, i);

		
		if (second_substr != "PRIVMSG")
		{
			//printout("! PRIVMSG\n");
			return;
		}

		next_non_blank_substr(line(), i, third_substr, i);

		// Ignore PMs directly to the bot for now (besides CTCP VERSION)
		if (third_substr == config_server().bot_name())
		{
			const std::string ctcp_version_str(":\001VERSION\001");
			
			next_non_blank_substr(line(), i, other_substr, i);

			// Respond to CTCP VERSION
			if (other_substr == ctcp_version_str)
			{
				send_raw_msg("NOTICE ", user_nick, "\001VERSION SaveyBot",
					"Version 3 (by FL4SHK)\001");
			}

			attempt_do_joins();

			return;
		}
		
		const std::string& some_channel = third_substr;

		bot().parse_command(*this, some_channel, user_nick, 
			line().substr(i));
	}
}

void IrcCommunicator::__reinit()
{
	__state.did_joins = false;
	__state.did_ping = false;
	__state.wants_select = true;

	//printout("__reinit()\n");
	do_getaddrinfo(config_server().address(), config_server().port_str());
	do_socket_and_connect();

	// Go ahead and do this now 
	free_res();

	sleep(1);

	do_select_and_also_full_read();
	printout("test\n");

	for (auto iter : config_server().startup_commands())
	{
		send_raw_msg(iter);
		sleep(1);
		//printout("__reinit():  ", do_select_and_also_full_read(), "\n");
		//sleep(1);
	}

	//do_select_and_also_full_read();

	//printout("__reinit():  ", do_select_and_also_full_read(), "\n");
	//sleep(1);

}

void IrcCommunicator::do_full_read()
{
	while (1)
	{
		// Null terminate
		raw_buf.fill('\0');
		const auto num_read = read(sock_fd(), raw_buf.data(), 
			(raw_buf.size() - 2));

		//if (num_read <= 0)
		//{
		//	clean_up_then_reinit();
		//}

		if (num_read == 0)
		{
			clean_up_then_reinit();
			continue;
		}
		if ((num_read == -1) 
			&& (!((errno == EAGAIN) || (errno == EWOULDBLOCK))))
		{
			clean_up_then_reinit();
			continue;
		}

		break;
	}

	std::string packet;


	for (auto iter : raw_buf)
	{
		if (iter == '\0')
		{
			break;
		}
		
		packet += iter;
		buf_str += iter;
	}

}

bool IrcCommunicator::do_select_and_also_full_read()
{
	fd_set temp_readfds;
	do_select_for_read(this, &temp_readfds);

	return do_full_read_if_fd_isset(&temp_readfds);
}

bool IrcCommunicator::do_full_read_if_fd_isset(fd_set* readfds)
{
	if (check_select_result(this, readfds))
	{
		//printout("FD_ISSET():  ");
		//err("Something wrong with the connection?");
		do_full_read();
		return true;
	}
	//printout("!FD_ISSET():  ");

	__line = "";

	return false;
}


void IrcCommunicator::update_line()
{
	const auto suffix_index = buf_str.find(msg_suffix);
	
	//printout(suffix_index, "\t");

	if (suffix_index != std::string::npos)
	{
		__line = buf_str.substr(0, suffix_index);
		//printout("update_line():  line():  ", line(), "\n");
		printout(line(), "\n");
		
		if ((suffix_index + msg_suffix.size()) > buf_str.size())
		{
			__state.wants_select = true;
			buf_str.clear();
		}
		else
		{
			buf_str = buf_str.substr(suffix_index + msg_suffix.size());

			if (buf_str.find(msg_suffix) != std::string::npos)
			{
				__state.wants_select = false;
			}
			else
			{
				__state.wants_select = true;
			}
		}
		

		//printout("buf_str Debug:  ", line(), "\n", buf_str, "\n\n\n");
	}
	else
	{
		printout("EGGS\n");
		__state.wants_select = true;
		__line = "";
	}
}


void IrcCommunicator::do_getaddrinfo(const std::string& some_address,
	const std::string& some_port_str)
{
	printout(some_address, " ", some_port_str, "\n");
	
	memset(&__hints, 0, sizeof(__hints));
	__hints.ai_family = specific_family();
	__hints.ai_socktype = specific_socktype();

	const int gai_result = getaddrinfo(some_address.c_str(), 
		some_port_str.c_str(), &hints(), &res());

	if (gai_result != 0)
	{
		printerr(gai_result, ":  ");
		printerr(gai_strerror(gai_result), ":  ");

		printerr("There was an error getting address information.\n");
		set_did_open_sock_fd(false);
		clean_up();
		exit(1);
	}

	set_did_alloc_res(true);
}

void IrcCommunicator::do_socket_and_connect()
{
	// The last parameter of socket() is set to zero because doing so
	// causes socket() to use an unspecified default protocol appropriate
	// for the requested socket type (SOCK_STREAM).
	set_sock_fd(socket(res()->ai_family, specific_socktype(), 0));


	const auto connect_result = connect(sock_fd(), res()->ai_addr, 
		res()->ai_addrlen);
	
	printout("connect_result:  ", connect_result, "\n");

	if (connect_result != 0)
	{
		printerr("There was an error connecting to the server.\n");
		set_did_open_sock_fd(false);
		clean_up();
		exit(1);
	}
}


void IrcCommunicator::inner_send_regular_msg(std::string&& full_msg)
{
	send_raw_msg("PRIVMSG ", channel(), " :", full_msg);
}

void IrcCommunicator::inner_send_raw_msg(std::string&& full_msg) const
{
	std::string temp = std::move(full_msg);

	// The "\r\n" IRC message suffix
	temp += msg_suffix;

	write(sock_fd(), temp.c_str(), temp.size());
}


void IrcCommunicator::clean_up()
{
	free_res();
	close_sock_fd();
}

void IrcCommunicator::check_timeout_with_ping()
{
	if (__state.did_ping)
	{
		clean_up_then_reinit();
	}
	else // if (!__state.did_ping)
	{
		send_raw_msg("PING ", ping_suffix);
	}
}

}
