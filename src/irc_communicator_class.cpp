#include "irc_communicator_class.hpp"


IRCCommunicator::IRCCommunicator(const std::string& some_server_name,
	const std::string& some_port_str)
{
	run_getaddrinfo(some_server_name, some_port_str);
	run_socket_and_connect();

	// Go ahead and do this now 
	free_res();




	
}

void IRCCommunicator::clean_up()
{
	free_res();
	close_sock_fd();
}


void IRCCommunicator::run_getaddrinfo(const std::string& some_server_name,
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

void IRCCommunicator::run_socket_and_connect()
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
