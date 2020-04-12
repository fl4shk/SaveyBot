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


#ifndef src_saveybot_class_hpp
#define src_saveybot_class_hpp

// src/saveybot_class.hpp

//#include "json_stuff.hpp"
//#include "irc_communicator_class.hpp"
#include "communicator_class.hpp"
#include "gmp_stuff.hpp"
#include "string_stuff.hpp"

namespace saveybot
{

class Database final
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
	private:		// variables
		// Might need to use an std::stringstream to get the value for
		// datetime.
		std::string _datetime, 
			_classic_index_str, 
			_message, 
			_name, 
			_slot;

	public:		// functions
		inline Value()
		{
		}
		inline Value(const std::string& s_datetime, 
			const std::string& s_classic_index_str, 
			const std::string& s_message, 
			const std::string& s_name, 
			const std::string& s_slot)
			: _datetime(s_datetime),
			_classic_index_str(s_classic_index_str),
			_message(s_message), 
			_name(s_name), 
			_slot(s_slot)
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

		GEN_GETTER_BY_REF(datetime);
		GEN_GETTER_BY_REF(classic_index_str);
		GEN_GETTER_BY_REF(message);
		GEN_GETTER_BY_REF(name);
		GEN_GETTER_BY_REF(slot);
		
		GEN_GETTER_BY_CON_REF(datetime);
		GEN_GETTER_BY_CON_REF(classic_index_str);
		GEN_GETTER_BY_CON_REF(message);
		GEN_GETTER_BY_CON_REF(name);
		GEN_GETTER_BY_CON_REF(slot);
	};


private:		// variables
	//size_t _lowest_available_slot;
	BigNum _lowest_available_slot;
	std::string _database_file_name;
	
	// Here is where the savestates are stored when in RAM.  They are
	// indexed by slot number as an std::string.
	std::map<std::string, Value> _savestates;


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
	void save(const std::string& message, const std::string& name, 
		const std::string& slot);
	void remove(const std::string& slot);
	
	void write_file() const;

	
	inline bool contains(const BigNum& slot_bignum) const
	{
		return (savestates().count(convert_bignum_to_str(slot_bignum)) 
			!= 0);
	}
	bool slot_owned_by(const BigNum& slot_bignum, 
		const std::string& name);
	
	inline bool slot_exists_but_not_owned_by(const BigNum& slot_bignum,
		const std::string& name)
	{
		return (contains(slot_bignum) 
			&& !slot_owned_by(slot_bignum, name));
	}


	inline auto begin()
	{
		return _savestates.begin();
	}
	inline auto end()
	{
		return _savestates.end();
	}

	inline auto cbegin() const
	{
		return _savestates.cbegin();
	}
	inline auto cend() const
	{
		return _savestates.cend();
	}

	inline auto size() const
	{
		return _savestates.size();
	}

	
	inline Value& at(const BigNum& slot_bignum)
	{
		return savestates()[convert_bignum_to_str(slot_bignum)];
	}
	inline Value& at(const std::string& slot)
	{
		return savestates()[slot];
	}
	
	GEN_GETTER_BY_VAL(lowest_available_slot);
	GEN_GETTER_BY_CON_REF(database_file_name);
	GEN_GETTER_BY_CON_REF(savestates);

private:		// functions
	GEN_SETTER_BY_VAL(lowest_available_slot);
	GEN_SETTER_BY_CON_REF(database_file_name);
	GEN_GETTER_BY_REF(savestates);

	void load_from_file();

	void update_lowest_available_slot();
	

	inline bool slot_owned_by(const std::string& slot, 
		const std::string& name)
	{
		//return savestates
		return (savestates()[slot].name() == name);
	}
	
};

class SaveyBot final
{
private:		// static variables
	static std::mutex _database_barrier;

private:		// variables
	Database _database;
	liborangepower::time::Prng _prng;

public:		// static variables
	static const std::string database_file_name;
	
public:		// functions
	SaveyBot();
	~SaveyBot();

	//void parse_command_basic(const std::vector<std::string>& args_vec);
	void parse_command(Communicator& comm, const std::string& channel, 
		const std::string& name, const std::string& whole_cmd_str);

private:		// functions
	GEN_GETTER_BY_CON_REF(database);
	GEN_GETTER_BY_REF(database);
	//GEN_GETTER_BY_REF(prng);

	//void save(const std::string& message, const std::string& name,
	//	const std::string&);
	

	// This is NOT a getter
	inline auto prng()
	{
		return _prng();
	}

	// This is NOT a getter
	inline auto prng(u64 max_val)
	{
		return _prng(max_val);
	}


	//// Fake message senders (they just print to the command line)
	//inline void fake_send_msg() const
	//{
	//	printout("\n");
	//}

	//template<typename FirstType, typename... ArgTypes>
	//void fake_send_msg(const FirstType& first_val, ArgTypes&&... rem_args)
	//	const
	//{
	//	printout(first_val);
	//	fake_send_msg(rem_args...);
	//}
	

};



} // namespace saveybot


#endif		// src_saveybot_class_hpp
