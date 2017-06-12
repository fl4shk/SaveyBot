#ifndef neosaveybot_class_hpp
#define neosaveybot_class_hpp

#include "json_stuff.hpp"
#include "irc_communicator_class.hpp"

namespace neosaveybot
{


class NeoSaveyBot
{
public:		// classes
	class Database
	{
	public:		// constants
		// Constant keys (all keys are constant, actually)
		static const std::string key_datetime, key_index, key_message,
			key_name, key_slot, key_num_savestates;

		// Constant values
		static const std::string value_datetime_classic;

	public:		// classes
		class Value
		{
		public:		// variables
			// Might need to use an std::stringstream to get the value for
			// datetime.
			std::string datetime, index_str, message, name, slot;

		public:		// functions
			inline Value(std::string&& s_datetime, 
				std::string&& s_index_str, std::string&& s_message, 
				std::string&& s_name, std::string&& s_slot)
				: datetime(std::move(s_datetime)),
				index_str(std::move(s_index_str)),
				message(std::move(s_message)), name(std::move(s_name)), 
				slot(std::move(s_slot))
			{
			}
		};

	public:		// variables
		// Here is where the savestates are stored when in RAM.  They are
		// indexed by slot number as an std::string.
		std::map<std::string, Value> savestates;
		
	};
	

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
	Database __database;

public:		// static variables
	static const std::string config_file_name, database_file_name;
	
public:		// functions
	NeoSaveyBot();
	~NeoSaveyBot();

	void parse_command(const std::vector<std::string>& command, 
		size_t start_index=0, size_t end_index=0);

private:		// functions
	gen_getter_by_con_ref(config);
	gen_getter_by_con_ref(database);
	gen_getter_by_ref(database);
	
};


std::ostream& operator << (std::ostream& os, 
	const NeoSaveyBot::Configuration::Server& to_print);

}

#endif // neosaveybot_class_hpp
