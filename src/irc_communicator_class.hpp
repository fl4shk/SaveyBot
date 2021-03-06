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


#ifndef src_irc_communicator_class_hpp
#define src_irc_communicator_class_hpp

// src/irc_communicator_class.hpp


//#include "json_stuff.hpp"
#include "misc_includes.hpp"
#include "communicator_class.hpp"
#include "string_stuff.hpp"
#include "misc_helper_funcs.hpp"

//#include "select_stuff.hpp"

namespace saveybot
{

class RealMain;
class SaveyBot;

// OLD COMMENT:  This should be moved to some IRC-related thing
class IrcConfiguration final
{
public:		// classes
	class Server final
	{
	private:		// variables
		std::string _name,
			_bot_name,
			_address,
			_port_str,
			_password;

		std::vector<std::string> _joins_list,
			_startup_commands;

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


		GEN_GETTER_AND_SETTER_BY_CON_REF(name);
		GEN_GETTER_AND_SETTER_BY_CON_REF(bot_name);
		GEN_GETTER_AND_SETTER_BY_CON_REF(address);
		GEN_GETTER_AND_SETTER_BY_CON_REF(port_str);
		GEN_GETTER_AND_SETTER_BY_CON_REF(password);
		GEN_GETTER_AND_SETTER_BY_CON_REF(joins_list);
		GEN_GETTER_AND_SETTER_BY_CON_REF(startup_commands);

		GEN_GETTER_BY_REF(joins_list);
		GEN_GETTER_BY_REF(startup_commands);

		GEN_SETTER_BY_RVAL_REF(name);
		GEN_SETTER_BY_RVAL_REF(bot_name);
		GEN_SETTER_BY_RVAL_REF(address);
		GEN_SETTER_BY_RVAL_REF(port_str);
		GEN_SETTER_BY_RVAL_REF(password);
		GEN_SETTER_BY_RVAL_REF(joins_list);
		GEN_SETTER_BY_RVAL_REF(startup_commands);


	};


private:		// variables
	std::vector<Server> _server_vec;

public:		// functions
	IrcConfiguration();

	IrcConfiguration(const IrcConfiguration& to_copy) = default;
	IrcConfiguration(IrcConfiguration&& to_move) = default;


	IrcConfiguration& operator = (const IrcConfiguration& to_copy) 
		= default;
	IrcConfiguration& operator = (IrcConfiguration&& to_move) = default;

	GEN_GETTER_BY_CON_REF(server_vec);
	
};


std::ostream& operator << (std::ostream& os, 
	const IrcConfiguration::Server& to_print);


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


class IrcCommunicator final : public Communicator
{
friend class RealMain;
friend void do_select_for_read
	(const std::vector<IrcCommunicator*>& comm_vec, fd_set* readfds);

public:		// static variables
	static const std::string msg_suffix, ping_suffix;

private:		// variables
	// Allow IPv4 or IPv6
	static constexpr auto _specific_family = AF_UNSPEC;

	// SOCK_STREAM - Provides sequenced, reliable, bidirectional,
	// connection- mode byte streams, and may provide a transmission
	static constexpr auto _specific_socktype = SOCK_STREAM;

	static constexpr size_t raw_buf_size = 9001;

	// "Whois" stuff (getaddrinfo())
	addrinfo _hints, * _res = nullptr;

	// Socket file descriptor
	int _sock_fd;

	// Stuff for clean_up()
	bool _did_alloc_res = false, _did_open_sock_fd = false;


	
	
	// This is a pointer to a constant IrcConfiguration::Server.
	const IrcConfiguration::Server* _config_server_ptr;

	// The array of characters read() sends its data to
	std::array<char, raw_buf_size> raw_buf;

	class State
	{
	public:		// variables
		bool did_joins, 
			did_ping, 
			wants_select, 
			seen_server_response_to_join,
			ignored_last_line,
			did_connect;
	
	public:		// functions
		inline State()
		{
			init();
		}

		inline void init()
		{
			did_joins = false;
			did_ping = false;
			wants_select = true;
			seen_server_response_to_join = false;
			ignored_last_line = false;
			did_connect = false;
		}
	} _state;

	std::string _line = "", buf_str = "";

	//size_t last_index = 0;


public:		// functions
	//IrcCommunicator(const std::string& some_server_name, 
	//	const std::string& some_port_str, const std::string& nick_command,
	//	const std::string& user_command, 
	//	const std::vector<std::string>& joins_list);
	IrcCommunicator(SaveyBot* s_bot_ptr, 
		const IrcConfiguration::Server* s_config_server_ptr);

	IrcCommunicator(const IrcCommunicator& to_copy) = default;
	IrcCommunicator(IrcCommunicator&& to_move) = default;

	virtual inline ~IrcCommunicator()
	{
		clean_up();
	}


	IrcCommunicator& operator = (const IrcCommunicator& to_copy) = default;
	IrcCommunicator& operator = (IrcCommunicator&& to_move) = default;

	virtual CommType comm_type() const
	{
		return CommType::Irc;
	}


	GEN_GETTER_BY_VAL(specific_family);
	GEN_GETTER_BY_VAL(specific_socktype);

	GEN_GETTER_BY_CON_REF(hints);
	GEN_GETTER_BY_CON_REF(res);
	GEN_GETTER_BY_VAL(sock_fd);

	GEN_GETTER_BY_VAL(did_alloc_res);
	GEN_GETTER_BY_VAL(did_open_sock_fd);

	inline const IrcConfiguration::Server& config_server() const
	{
		return *_config_server_ptr;
	}

	GEN_GETTER_BY_CON_REF(line);


	bool update_buf_str(fd_set* readfds);
	bool can_iterate() const;
	//void iterate(fd_set* readfds);
	void iterate();




protected:		// functions
	virtual void inner_send_regular_msg(std::string&& full_msg);

	void _reinit();
	inline void clean_up_then_reinit()
	{
		clean_up();
		_reinit();
	}
	


private:		// functions
	GEN_GETTER_BY_REF(hints);
	GEN_GETTER_BY_REF(res);
	GEN_SETTER_BY_VAL(sock_fd);
	
	GEN_SETTER_BY_VAL(did_alloc_res);
	GEN_SETTER_BY_VAL(did_open_sock_fd);



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
	bool do_socket_and_connect();


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

	inline bool _attempt_do_joins()
	{
		if (!_state.did_joins)
		{
			sleep(1);

			_state.did_joins = true;

			for (auto iter : config_server().joins_list())
			{
				send_raw_msg("JOIN ", iter);
			}

			return true;
		}
		return false;
	}

	//inline void _handle_ping(size_t& i, bool do_attempt_joins=true)
	inline void _handle_ping(size_t& i)
	{
		std::string second_substr;
		next_non_blank_substr(line(), i, second_substr, i);
		//printout("PING Debug:  ", second_substr, "\n");
		send_raw_msg("PONG ", second_substr);

		//if (do_attempt_joins)
		//{
		//	_attempt_do_joins();
		//}
	}

	inline bool _handle_pong(size_t i)
	{
		std::string second_substr, third_substr, fourth_substr;
		next_non_blank_substr(line(), i, second_substr, i);
		//next_non_blank_substr(line(), i, third_substr, i);
		//next_non_blank_substr(line(), i, fourth_substr, i);

		if (second_substr == "PONG")
		{
			if (_state.did_ping)
			{
				next_non_blank_substr(line(), i, third_substr, i);
				next_non_blank_substr(line(), i, fourth_substr, i);
				if (fourth_substr.find(ping_suffix) == 0)
				{
					_state.did_ping = false;
					return true;
				}
				else
				{
					err("PONG Eek 0!");
				}
			}
			else
			{
				err("PONG Eek 1!");
			}
		}
		return false;
	}

	inline bool _substr_is_config_server_address
		(const std::string& substr) const
	{
		return (substr == sconcat(":", config_server().address()));
	}

	bool _can_ignore(const std::string& first_substr, size_t& i);

	int _handle_ctcp_version(const std::string& first_substr, 
		std::string& second_substr, std::string& third_substr, 
		std::string& other_substr, size_t* i, size_t& exclam_index, 
		size_t& space_index, std::string& user_nick);

	void _initial_ignoring();

	void check_timeout_with_ping();


};



} // namespace saveybot



#endif		// src_irc_communicator_class_hpp
