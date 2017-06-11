#include "irc_communicator_class.hpp"


IRCCommunicator::IRCCommunicator(const std::string& server_name,
	const std::string& port_str)
{
	memset(&__hints, 0, sizeof(__hints));
	__hints.ai_family = specific_family();
	__hints.ai_socktype = specific_socktype();

	if (getaddrinfo(server_name.c_str(), port_str.c_str(), &__hints, 
		&__res) != 0)
	{
		printerr("There was an error getting address information.\n");
		exit(1);
	}
}

void IRCCommunicator::clean_up()
{
	free_res();
	close_sock_fd();
}
