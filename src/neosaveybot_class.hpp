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


#ifndef neosaveybot_class_hpp
#define neosaveybot_class_hpp

#include "json_stuff.hpp"
#include "irc_communicator_class.hpp"

namespace neosaveybot
{

class Database
{
public:		// constants
	// Constant keys (all keys are constant, actually)
	static const std::string key_datetime, 
		key_classic_index, 
		key_message,
		key_name, 
		key_slot;

	// Constant values
	static const std::string value_datetime_classic;


	//// The highest allowed automatic slot
	//static constexpr size_t max_automatic_slot = -2;

public:		// classes
	class Value
	{
	public:		// variables
		// Might need to use an std::stringstream to get the value for
		// datetime.
		std::string datetime, 
			classic_index_str, 
			message, 
			name, 
			slot;

	public:		// functions
		inline Value()
		{
		}
		inline Value(const std::string& s_datetime, 
			const std::string& s_classic_index_str, 
			const std::string& s_message, 
			const std::string& s_name, 
			const std::string& s_slot)
			: datetime(s_datetime),
			classic_index_str(s_classic_index_str),
			message(s_message), 
			name(s_name), 
			slot(s_slot)
		{
		}

		inline Value(const Json::Value& to_copy)
		{
			extract_from_json(to_copy);
		}

		inline Value(const Value& to_copy) = default;
		inline Value(Value&& to_move) = default;

		inline Value& operator = (const Value& to_copy) = default;
		inline Value& operator = (Value&& to_move) = default;

		void extract_from_json(const Json::Value& to_copy);
		void add_to_json(Json::Value& output_root) const;
	};


private:		// variables
	//size_t __lowest_available_slot;
	mpz_class __lowest_available_slot;
	std::string __database_file_name;

public:		// variables
	// Here is where the savestates are stored when in RAM.  They are
	// indexed by slot number as an std::string.
	std::map<std::string, Value> savestates;


public:		// functions
	inline Database()
	{
	}
	inline Database(const std::string& s_database_file_name)
	{
		init(s_database_file_name);
	}
	inline ~Database()
	{
		write_file();
	}

	inline void init(const std::string& s_database_file_name)
	{
		set_database_file_name(s_database_file_name);
		load_from_file();
	}
	void write_file() const;

	void save(const std::string& message, const std::string& name, 
		const std::string& slot, bool use_lowest);
	
	gen_getter_by_val(lowest_available_slot);
	gen_getter_by_con_ref(database_file_name);

private:		// functions
	gen_setter_by_val(lowest_available_slot);
	gen_setter_by_con_ref(database_file_name);

	void load_from_file();

	void update_lowest_available_slot();
	
};

class NeoSaveyBot
{
public:		// classes
	// This should be moved to some IRC-related thing
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

	//void parse_command_basic(const std::vector<std::string>& args_vec);
	void parse_command(const std::string& name, 
		const std::string& whole_cmd_str);

private:		// functions
	gen_getter_by_con_ref(config);
	gen_getter_by_con_ref(database);
	gen_getter_by_ref(database);

	//void save(const std::string& message, const std::string& name,
	//	const std::string&);
	

	//bool convert_str_to_bignum(const std::string& num_str, mpz_class& ret)
	//{
	//	std::stringstream num_sstm(num_str);
	//	num_sstm >> ret;

	//	return num_sstm.fail();
	//}


	bool find_next_non_blank_index(const std::string& whole_cmd_str,
		const size_t test_start_index, size_t& i);
	bool next_non_blank_substr(const std::string& whole_cmd_str, 
		const size_t test_start_index, std::string& ret, size_t& i);

};


std::ostream& operator << (std::ostream& os, 
	const NeoSaveyBot::Configuration::Server& to_print);

}

#endif // neosaveybot_class_hpp
