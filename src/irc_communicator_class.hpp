// This file is part of SaveyBot.
// 
// Copyright 2017 Andrew Clark (FL4SHK).
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


#ifndef irc_communicator_class_hpp
#define irc_communicator_class_hpp


//#include "json_stuff.hpp"
#include "misc_includes.hpp"
#include "communicator_class.hpp"

//#include "select_stuff.hpp"

namespace saveybot
{

class SaveyBot;

// This should be moved to some IRC-related thing
class IRCConfiguration
{
public:		// classes
	class Server
	{
	private:		// variables
		std::string __name,
			__bot_name,
			__address,
			__port_str,
			__password;

		std::vector<std::string> __joins_list,
			__startup_commands;

	public:		// functions
		inline Server()
		{
		}

		Server(const Server& to_copy) = default;
		Server(Server&& to_move) = default;

		inline ~Server()
		{
		}
		
		Server& operator = (const Server& to_copy) = default;
		Server& operator = (Server&& to_move) = default;
		
		
		gen_getter_by_con_ref(name);
		gen_getter_by_con_ref(bot_name);
		gen_getter_by_con_ref(address);
		gen_getter_by_con_ref(port_str);
		gen_getter_by_con_ref(password);
		gen_getter_by_con_ref(joins_list);
		gen_getter_by_con_ref(startup_commands);

		gen_getter_by_ref(joins_list);
		gen_getter_by_ref(startup_commands);
		

		gen_setter_by_con_ref(name);
		gen_setter_by_con_ref(bot_name);
		gen_setter_by_con_ref(address);
		gen_setter_by_con_ref(port_str);
		gen_setter_by_con_ref(password);
		gen_setter_by_con_ref(joins_list);
		gen_setter_by_con_ref(startup_commands);
		
		gen_setter_by_rval_ref(name);
		gen_setter_by_rval_ref(bot_name);
		gen_setter_by_rval_ref(address);
		gen_setter_by_rval_ref(port_str);
		gen_setter_by_rval_ref(password);
		gen_setter_by_rval_ref(joins_list);
		gen_setter_by_rval_ref(startup_commands);
		
		
	};
	

private:		// variables
	std::vector<Server> __server_vec;

public:		// functions
	IRCConfiguration();

	IRCConfiguration(const IRCConfiguration& to_copy) = default;
	IRCConfiguration(IRCConfiguration&& to_move) = default;


	IRCConfiguration& operator = (const IRCConfiguration& to_copy) 
		= default;
	IRCConfiguration& operator = (IRCConfiguration&& to_move) = default;

	gen_getter_by_con_ref(server_vec);
	
};


std::ostream& operator << (std::ostream& os, 
	const IRCConfiguration::Server& to_print);


//// Here is the addrinfo struct.
//struct addrinfo {
//
//	// This field specifies additional options, described below. Multiple
//	// flags are specified by bitwise OR-ing them together.
//	int              ai_flags;
//
//	// This field specifies the desired address family for the returned
//	// addresses. Valid values for this field include AF_INET and AF_INET6.
//	// The value AF_UNSPEC indicates that getaddrinfo() should return
//	// socket addresses for any address family (either IPv4 or IPv6, for
//	// example) that can be used with node and service.
//	int              ai_family;
//
//	// This field specifies the preferred socket type, for example
//	// SOCK_STREAM or SOCK_DGRAM. Specifying 0 in this field indicates that
//	// socket addresses of any type can be returned by getaddrinfo().
//	int              ai_socktype;
//
//
//	// This field specifies the protocol for the returned socket addresses.
//	// Specifying 0 in this field indicates that socket addresses with any
//	// protocol can be returned by getaddrinfo().
//	int              ai_protocol;
//
//	// All the other fields in the structure pointed to by hints must
//	// contain either 0 or a null pointer, as appropriate.
//	socklen_t        ai_addrlen;
//	struct sockaddr *ai_addr;
//	char            *ai_canonname;
//	struct addrinfo *ai_next;
//};


class IRCCommunicator : public Communicator
{
public:		// static variables
	static const std::string config_file_name,
		msg_suffix;

private:		// variables
	// Allow IPv4 or IPv6
	static constexpr auto __specific_family = AF_UNSPEC;

	// SOCK_STREAM - Provides sequenced, reliable, bidirectional,
	// connection- mode byte streams, and may provide a transmission
	static constexpr auto __specific_socktype = SOCK_STREAM;

	static constexpr size_t raw_buf_size = 9001;

	// "Whois" stuff (getaddrinfo())
	addrinfo __hints, * __res = nullptr;

	// Socket file descriptor
	int __sock_fd;

	// Stuff for clean_up()
	bool __did_alloc_res = false, __did_open_sock_fd = false;


	
	SaveyBot* __bot_ptr;
	
	// This is a pointer to a constant IRCConfiguration::Server.
	const IRCConfiguration::Server* __config_server_ptr;

	// The array of characters read() sends its data to
	std::array<char, raw_buf_size> raw_buf;

	bool did_joins = false;

	std::string __line = "", buf_str = "";

	//size_t last_index = 0;


public:		// functions
	//IRCCommunicator(const std::string& some_server_name, 
	//	const std::string& some_port_str, const std::string& nick_command,
	//	const std::string& user_command, 
	//	const std::vector<std::string>& joins_list);
	IRCCommunicator(SaveyBot* s_bot_ptr, 
		const IRCConfiguration::Server* s_config_server_ptr);
	
	IRCCommunicator(const IRCCommunicator& to_copy) = default;
	IRCCommunicator(IRCCommunicator&& to_move) = default;
	
	inline ~IRCCommunicator()
	{
		clean_up();
	}

	IRCCommunicator& operator = (const IRCCommunicator& to_copy) = default;
	IRCCommunicator& operator = (IRCCommunicator&& to_move) = default;

	gen_getter_by_val(specific_family);
	gen_getter_by_val(specific_socktype);

	gen_getter_by_con_ref(hints);
	gen_getter_by_con_ref(res);
	gen_getter_by_val(sock_fd);

	gen_getter_by_val(did_alloc_res);
	gen_getter_by_val(did_open_sock_fd);

	inline const IRCConfiguration::Server& config_server() const
	{
		return *__config_server_ptr;
	}

	gen_getter_by_con_ref(line);


	void iterate(fd_set* readfds);



protected:		// functions
	virtual void inner_send_regular_msg(std::string&& full_msg);
	


private:		// functions
	gen_getter_by_ref(hints);
	gen_getter_by_ref(res);
	gen_setter_by_val(sock_fd);
	
	gen_setter_by_val(did_alloc_res);
	gen_setter_by_val(did_open_sock_fd);

	inline SaveyBot& bot() const
	{
		return *__bot_ptr;
	}


	void do_full_read();
	bool do_select_and_also_full_read();
	bool do_full_read_if_fd_isset(fd_set* readfds);
	void update_line();

	void inner_send_raw_msg(std::string&& full_msg) const;

	template<typename FirstType, typename... RemArgTypes>
	inline void send_raw_msg(const FirstType& first_val, 
		RemArgTypes&&...  rem_args)
	{
		std::string temp = sconcat(first_val, rem_args...);
		printerr("Sending to server \"", config_server().name(), "\":  ",
			temp, "\n");

		inner_send_raw_msg(std::move(temp));
	}


	void do_getaddrinfo(const std::string& some_server_name, 
		const std::string& some_port_str);
	void do_socket_and_connect();


	void clean_up();

	inline void free_res()
	{
		if (did_alloc_res())
		{
			set_did_alloc_res(false);
			freeaddrinfo(res());
		}
	}
	inline void close_sock_fd()
	{
		if (did_open_sock_fd())
		{
			set_did_open_sock_fd(false);
			close(sock_fd());
		}
	}
};



}


#endif		// irc_communicator_class_hpp
