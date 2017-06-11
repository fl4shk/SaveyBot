#ifndef irc_communicator_class_hpp
#define irc_communicator_class_hpp


#include "json_stuff.hpp"


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


class IRCCommunicator
{
private:		// variables
	// Allow IPv4 or IPv6
	static constexpr auto __specific_family = AF_UNSPEC;

	// SOCK_STREAM - Provides sequenced, reliable, bidirectional,
	// connection- mode byte streams, and may provide a transmission
	static constexpr auto __specific_socktype = SOCK_STREAM;

	// "Whois" stuff (getaddrinfo())
	addrinfo __hints, * __res = nullptr;

	// Socket file descriptor
	int __sock_fd;

	// Stuff for clean_up()
	bool __did_alloc_res = false, __did_open_sock_fd = false;

public:		// functions

	IRCCommunicator(const std::string& some_server_name, 
		const std::string& some_port_str);
	inline ~IRCCommunicator()
	{
		clean_up();
	}

	gen_getter_by_val(specific_family);
	gen_getter_by_val(specific_socktype);
	gen_getter_by_con_ref(hints);
	gen_getter_by_con_ref(res);
	gen_getter_by_val(sock_fd);
	gen_getter_by_val(did_alloc_res);
	gen_getter_by_val(did_open_sock_fd);

private:		// functions
	gen_getter_by_ref(hints);
	gen_getter_by_ref(res);
	
	gen_setter_by_val(sock_fd);
	gen_setter_by_val(did_alloc_res);
	gen_setter_by_val(did_open_sock_fd);

	void clean_up();

	void run_getaddrinfo(const std::string& some_server_name, 
		const std::string& some_port_str);
	void run_socket_and_connect();

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


#endif		// irc_communicator_class_hpp
