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
#include "gmp_stuff.hpp"

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
	private:		// variables
		// Might need to use an std::stringstream to get the value for
		// datetime.
		std::string __datetime, 
			__classic_index_str, 
			__message, 
			__name, 
			__slot;

	public:		// functions
		inline Value()
		{
		}
		inline Value(const std::string& s_datetime, 
			const std::string& s_classic_index_str, 
			const std::string& s_message, 
			const std::string& s_name, 
			const std::string& s_slot)
			: __datetime(s_datetime),
			__classic_index_str(s_classic_index_str),
			__message(s_message), 
			__name(s_name), 
			__slot(s_slot)
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

		gen_getter_by_ref(datetime);
		gen_getter_by_ref(classic_index_str);
		gen_getter_by_ref(message);
		gen_getter_by_ref(name);
		gen_getter_by_ref(slot);
		
		gen_getter_by_con_ref(datetime);
		gen_getter_by_con_ref(classic_index_str);
		gen_getter_by_con_ref(message);
		gen_getter_by_con_ref(name);
		gen_getter_by_con_ref(slot);
	};


private:		// variables
	//size_t __lowest_available_slot;
	BigNum __lowest_available_slot;
	std::string __database_file_name;
	
	// Here is where the savestates are stored when in RAM.  They are
	// indexed by slot number as an std::string.
	std::map<std::string, Value> __savestates;


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
		return __savestates.begin();
	}
	inline auto end()
	{
		return __savestates.end();
	}

	inline auto cbegin() const
	{
		return __savestates.cbegin();
	}
	inline auto cend() const
	{
		return __savestates.cend();
	}

	inline auto size() const
	{
		return __savestates.size();
	}

	
	inline Value& at(const BigNum& slot_bignum)
	{
		return savestates()[convert_bignum_to_str(slot_bignum)];
	}
	inline Value& at(const std::string& slot)
	{
		return savestates()[slot];
	}
	
	gen_getter_by_val(lowest_available_slot);
	gen_getter_by_con_ref(database_file_name);
	gen_getter_by_con_ref(savestates);

private:		// functions
	gen_setter_by_val(lowest_available_slot);
	gen_setter_by_con_ref(database_file_name);
	gen_getter_by_ref(savestates);

	void load_from_file();

	void update_lowest_available_slot();
	

	inline bool slot_owned_by(const std::string& slot, 
		const std::string& name)
	{
		//return savestates
		return (savestates()[slot].name() == name);
	}
	
};

class NeoSaveyBot
{



private:		// variables
	Database __database;
	liborangepower::time::Prng __prng;

public:		// static variables
	static const std::string database_file_name;
	
public:		// functions
	NeoSaveyBot();
	~NeoSaveyBot();

	//void parse_command_basic(const std::vector<std::string>& args_vec);
	void parse_command(Communicator& comm, const std::string& channel, 
		const std::string& name, const std::string& whole_cmd_str);

private:		// functions
	gen_getter_by_con_ref(database);
	gen_getter_by_ref(database);
	//gen_getter_by_ref(prng);

	//void save(const std::string& message, const std::string& name,
	//	const std::string&);
	

	//bool convert_str_to_bignum(const std::string& num_str, BigNum& ret)
	//{
	//	std::stringstream num_sstm(num_str);
	//	num_sstm >> ret;

	//	return num_sstm.fail();
	//}


	bool find_next_non_blank_index(const std::string& whole_cmd_str,
		const size_t test_start_index, size_t& i);
	bool next_non_blank_substr(const std::string& whole_cmd_str, 
		const size_t test_start_index, std::string& ret, size_t& i);


	// These are NOT getters
	inline auto prng()
	{
		return __prng();
	}

	inline auto prng(u64 max_val)
	{
		return __prng(max_val);
	}


	// Fake message senders (they just print to the command line)
	inline void fake_send_msg() const
	{
		printout("\n");
	}

	template<typename FirstType, typename... ArgTypes>
	void fake_send_msg(const FirstType& first_val, ArgTypes&&... rem_args)
		const
	{
		printout(first_val);
		fake_send_msg(rem_args...);
	}
	

};



}

#endif // neosaveybot_class_hpp
