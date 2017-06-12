#ifndef neosaveybot_class_hpp
#define neosaveybot_class_hpp

#include "json_stuff.hpp"
#include "irc_communicator_class.hpp"



class NeoSaveyBot
{
public:		// classes
	class Configuration
	{
	public:		// classes
		class Server
		{
		public:		// variables
			std::string name, 
				bot_name,
				address,
				port_str,
				password;

			std::vector<std::string> joins_list,
				startup_commands;

		public:		// functions
			
		};
		

	public:		// variables
		std::vector<Server> server_vec;

	public:		// functions
		Configuration();
		
	};

private:		// variables
	Configuration __config;

public:		// static variables
	static const std::string config_file_name, database_file_name;
	
public:		// functions
	NeoSaveyBot();
	~NeoSaveyBot();

	void parse_command(const std::vector<std::string>& command, 
		size_t start_index=0, size_t end_index=0);

private:		// functions
	gen_getter_by_con_ref(config);
	
};


std::ostream& operator << (std::ostream& os, 
	const NeoSaveyBot::Configuration::Server& to_print);

#endif // neosaveybot_class_hpp
