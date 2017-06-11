#include "irc_communicator_class.hpp"


IRCCommunicator::IRCCommunicator(const std::string& server_name,
	const std::string& port_str)
{
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = specific_family;
	hints.ai_socktype = specific_socktype;

	if (getaddrinfo(some_server_name.c_str(), some_port_str.c_str(),
		&hints, &res) != 0)
	{
		printerr("There was an error getting address information.\n");
		exit(1);
	}
}

void IRCCommunicator::clean_up()
{
	free_res();
}
