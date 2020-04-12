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


#include "saveybot_class.hpp"

namespace saveybot
{


// Constant keys
const std::string Database::key_datetime("datetime"),
	Database::key_classic_index("classic_index"),
	Database::key_message("message"),
	Database::key_name("name"),
	Database::key_slot("slot");

// Constant values
const std::string Database::value_datetime_classic("classic");

void Database::Value::extract_from_json(const Json::Value& to_copy)
{
	datetime() = to_copy[Database::key_datetime].asString();
	classic_index_str() = to_copy[Database::key_classic_index].asString();
	message() = to_copy[Database::key_message].asString();
	name() = to_copy[Database::key_name].asString();
	slot() = to_copy[Database::key_slot].asString();
}
void Database::Value::add_to_json(Json::Value& output_root) const
{
	output_root[slot()][Database::key_datetime] = datetime();

	Json::Value::Int64 classic_index;
	std::stringstream sstm(classic_index_str());
	sstm >> classic_index;
	output_root[slot()][Database::key_classic_index] = classic_index;

	output_root[slot()][Database::key_message] = message();
	output_root[slot()][Database::key_name] = name();
	output_root[slot()][Database::key_slot] = slot();
}

void Database::save(const std::string& message, const std::string& name, 
	const std::string& slot)
{
	//if (use_lowest)
	//{
	//	std::string&& temp_slot = convert_bignum_to_str
	//		(lowest_available_slot());
	//	
	//	save(message, name, temp_slot, false);
	//	return;
	//}

	std::string datetime;
	std::stringstream dt_sstm;
	osprintout(dt_sstm, liborangepower::time::put_now_as_localtime(),
		"\n");
	getline(dt_sstm, datetime);

	savestates()[slot] = std::move(Value(datetime, "-1", message, name,
		slot));
	
	// Just do this after EVERY save()
	update_lowest_available_slot();
	
	write_file();
}

void Database::remove(const std::string& slot)
{
	BigNum slot_bignum;
	str_is_integer_bignum(slot, slot_bignum);

	if ((slot_bignum >= 0) && (slot_bignum < lowest_available_slot()))
	{
		set_lowest_available_slot(slot_bignum);
	}

	savestates().erase(slot); 

	write_file();
}

void Database::write_file() const
{
	Json::Value output_root;

	for (const auto& iter : savestates())
	{
		iter.second.add_to_json(output_root);
	}

	//std::fstream out_file(std::string("temp_") + database_file_name(), 
	//	std::ios_base::out);
	std::fstream out_file(database_file_name(), std::ios_base::out);
	write_json(out_file, &output_root);
}

bool Database::slot_owned_by(const BigNum& slot_bignum, 
	const std::string& name)
{
	return slot_owned_by(convert_bignum_to_str(slot_bignum), name);
}

void Database::load_from_file()
{
	Json::Value input_root;
	std::string errs;
	
	parse_json(database_file_name(), &input_root, &errs);

	for (auto& in_iter : input_root)
	{
		Value to_insert(in_iter);
		savestates()[to_insert.slot()] = std::move(to_insert);
	}

	set_lowest_available_slot(0);
	update_lowest_available_slot();
}

void Database::update_lowest_available_slot()
{
	printout("I started with this lowest_available_slot:  ",
		lowest_available_slot(), "\n");

	for ( ; ; ++_lowest_available_slot)
	{
		const std::string las_str 
			= convert_bignum_to_str(lowest_available_slot());

		const auto search = savestates().find(las_str);

		if (search == savestates().end())
		{
			break;
		}
	}

	printout("I ended with this lowest available slot:  ",
		lowest_available_slot(), "\n\n");
}

std::mutex SaveyBot::_database_barrier;
const std::string SaveyBot::database_file_name("saveybot.json");




SaveyBot::SaveyBot() : _database(database_file_name)
{
}

SaveyBot::~SaveyBot()
{
}


void SaveyBot::parse_command(Communicator& comm, 
	const std::string& channel, const std::string& name,
	const std::string& whole_cmd_str)
{
	std::lock_guard<std::mutex> block_threads_until_finish_this_job
		(_database_barrier);

	//printout("SaveyBot::parse_command():  ", strappcom2(channel, name,
	//	whole_cmd_str), "\n");

	typedef std::function<void()> CommandClauseFunc;


	// First things first
	comm.set_channel(channel);

	//printout("I'm here!\n");
	//printout("Here's the command:  ", whole_cmd_str, "\n");

	size_t i = 0, temp_i = 0;
	BigNum slot_bignum;
	std::string cmd, slot, message;
	std::vector<std::string> temp_slot_vec;

	next_non_blank_substr(whole_cmd_str, 0, cmd, i);

	if (comm.comm_type() == Communicator::CommType::Irc)
	{
		// Strip leading ":"
		cmd = cmd.substr(1);
	}


	//printout("Here's the prefix:  ", cmd, "\n");

	static const std::string wow_suffix("! wow !!! :D/"), 
		happy_suffix("!!! :D/"), sad_suffix(" )));");
	
	auto print_found_command = [&]() -> void
	{
		printout("Found a \"", cmd, "\".\n");
		//comm.send_saveybot_msg("Found a \"", cmd, "\".\n");
	};
	auto say_invalid_num_params = [&]() -> void
	{
		comm.send_saveybot_msg("Wrong parameters for \"", 
			cmd, "\"", sad_suffix, "!");
		//comm.send_saveybot_msg("Wrong parameters for \"", 
		//	cmd, "\"", "!");
		//comm.send_saveybot_msg("Wrong parameters for ",
		//	cmd, "!");
	};

	auto inner_next_non_blank_substr = [&]() -> bool
	{
		return next_non_blank_substr(whole_cmd_str, i, slot, temp_i);
	};

	auto show_msg = [&](const Database::Value& to_show) -> void
	{
		comm.send_saveybot_msg(to_show.name(), "[", to_show.slot(), 
			"]:  ", to_show.message());
	};

	auto show_datetime = [&](const Database::Value& to_show) -> void
	{
		if (to_show.datetime() == "classic")
		{
			comm.send_saveybot_msg("That savestate was sav'd using ",
				"original SaveyBot, so datetime unknown", happy_suffix);
		}
		else
		{
			comm.send_saveybot_msg("That savestate was sav'd on ",
				to_show.datetime(), happy_suffix);
		}
	};

	auto say_cant_find_owned_by = [&](const std::string& some_name) 
		-> void
	{
		comm.send_saveybot_msg("Can't find any savestates owned ",
			"by ", some_name, "!");
	};
	
	auto say_slot_doesnt_exist = [&]() -> void
	{
		//comm.send_saveybot_msg("That slot doesn't exist!");
		comm.send_saveybot_msg("no one owns that savestate!!! (u should",
			" change that!)");
	};

	auto say_message_saved = [&]() -> void
	{
		comm.send_saveybot_msg("Your savestate was sav'd to slot number ", 
			slot, "!");
	};

	auto say_owned_by = [&]() -> void
	{
		comm.send_saveybot_msg("That slot is owned by ", 
			database().at(slot_bignum).name(), "!");
	};

	auto say_rip = [&]() -> void
	{
		comm.send_saveybot_msg("rip ur msg", sad_suffix);
	};

	auto say_database_empty = [&]() -> void
	{
		comm.send_saveybot_msg("The database is empty!", sad_suffix);
	};
	auto say_need_slot_number = [&]() -> void
	{
		comm.send_saveybot_msg("Need a slot number for that!");
	};
	auto say_need_slot_number_or_username = [&]() -> void
	{
		comm.send_saveybot_msg("Need a slot number (or username) for ",
			"that!");
	};

	auto say_number_of_slots_owned_by = [&]
		(const std::string& some_name, const size_t amount) -> void
	{
		comm.send_saveybot_msg(some_name, " owns ", amount, 
			" savestates", happy_suffix);
	};

	auto fill_temp_slot_vec = [&]
		(const std::string& some_name) -> void
	{
		temp_slot_vec.clear();
		temp_slot_vec.shrink_to_fit();

		for (auto iter : database())
		{
			if (iter.second.name() == some_name)
			{
				temp_slot_vec.push_back(iter.second.slot());
			}
		}
	};


	// ".save"
	auto exec_save_like_command = [&]
		(const CommandClauseFunc& given_slot_and_message_clause,
		const CommandClauseFunc& given_message_clause) -> void
	{
		print_found_command();

		if (!inner_next_non_blank_substr())
		{
			say_invalid_num_params();
			return;
		}

		// If we were given a slot
		if (str_is_integer_bignum(slot, slot_bignum))
		{
			i = temp_i;

			// We need both a slot AND a message
			if (!eat_whitespace(whole_cmd_str, temp_i, i))
			{
				say_invalid_num_params();
				return;
			}
			
			given_slot_and_message_clause();
		}
		else
		{
			// Don't need to check return value of this because earlier if
			// statement already checked if anything came after the command
			// name
			given_message_clause();
		}
	};


	// ".load"
	auto exec_takes_slot_or_username_command = [&]
		(const CommandClauseFunc& given_slot_clause,
		const CommandClauseFunc& given_username_clause) -> void
	{
		print_found_command();

		if (!inner_next_non_blank_substr())
		{
			say_need_slot_number_or_username();
			return;
		}
		
		// If we were given a slot
		else if (str_is_integer_bignum(slot, slot_bignum))
		{
			given_slot_clause();
		}
		// If we were given a username
		else
		{
			given_username_clause();
		}
	};

	// ".road"
	auto exec_takes_nothing_or_username_command = [&]
		(const CommandClauseFunc& given_nothing_clause,
		const CommandClauseFunc& given_username_clause) -> void
	{
		print_found_command();

		// If we weren't given a user
		if (!eat_whitespace(whole_cmd_str, i, temp_i))
		{
			given_nothing_clause();
		}

		// If we were given a user
		else
		{
			given_username_clause();
		}
	};

	// ".remove", ".date", ".datetime", ".whois"
	auto exec_takes_slot_command = [&]
		(const CommandClauseFunc& given_slot_clause) -> void
	{
		print_found_command();

		if (!inner_next_non_blank_substr())
		{
			say_invalid_num_params();
			return;
		}

		// REQUIRE a slot to delete
		if (!str_is_integer_bignum(slot, slot_bignum))
		{
			//say_invalid_num_params();
			say_need_slot_number();
			return;
		}

		given_slot_clause();

	};


	if (cmd == ".help")
	{
		//comm.send_regular_msg("~ lol u n00b (^: ",
		//	"http://savestate.info/upload/IRCHelp.png");
		//comm.send_regular_msg("~ lol u n00b (^: ",
		//	"http://overcocked.penis.systems/commands.html");
		comm.send_saveybot_msg("lol u n00b (^: ",
			"https://github.com/fl4sh%6B/SaveyBot/blob/master/README.md");
	}

	else if (cmd == ".save")
	{
		exec_save_like_command([&]() -> void
			{
				// If we were given a slot, then we need both a slot AND a
				// message
				if (database().slot_exists_but_not_owned_by(slot_bignum,
					name))
				{
					say_owned_by();
				}
				else
				{
					message = substr_trimmed(whole_cmd_str, i);
					_database.save(message, name, slot);
					say_message_saved();
				}
			},
		
			[&]() -> void
			{
				// Don't need to check return value of this because earlier
				// if statement already checked if anything came after the
				// ".save"
				eat_whitespace(whole_cmd_str, i, i);

				
				slot = convert_bignum_to_str(database()
					.lowest_available_slot());
				message = substr_trimmed(whole_cmd_str, i);
				_database.save(message, name, slot);
				say_message_saved();
			});
	}

	else if (cmd == ".load")
	{
		exec_takes_slot_or_username_command([&]() -> void
			{
				// If we were given a slot
				if (database().contains(slot_bignum))
				{
					show_msg(database().at(slot_bignum));
				}
				else
				{
					say_slot_doesnt_exist();
				}
			},

			[&]() -> void
			{
				// If we were given a username
				const std::string& some_name = slot;

				fill_temp_slot_vec(some_name);

				say_number_of_slots_owned_by(some_name, 
					temp_slot_vec.size());

				return;
			});
	}

	else if (cmd == ".remove")
	{
		exec_takes_slot_command([&]() -> void
			{
				if (!database().contains(slot_bignum))
				{
					say_slot_doesnt_exist();
				}
				else if (!database().slot_owned_by(slot_bignum, name))
				{
					say_owned_by();
				}
				else
				{
					_database.remove(slot);
					say_rip();
				}
			});
	}

	else if (cmd == ".road")
	{
		exec_takes_nothing_or_username_command([&]() -> void
			{
				// If we weren't given a user
				if (database().size() == 0)
				{
					say_database_empty();
					return;
				}
				
				const auto offset = prng(database().size());
				
				auto iter = database().begin();

				for (size_t j=0; j<offset; ++j)
				{
					++iter;
				}

				show_msg(iter->second);
			},

			[&]() -> void
			{
				// If we were given a user
				i = temp_i;

				const std::string& some_name 
					= substr_trimmed(whole_cmd_str, i);

				fill_temp_slot_vec(some_name);

				if (temp_slot_vec.size() == 0)
				{
					say_cant_find_owned_by(some_name);
				}
				else // if (temp_slot_vec.size() > 0)
				{
					// Almost duplicate code
					const auto offset = prng(temp_slot_vec.size());
					auto iter = temp_slot_vec.begin();

					iter += offset;

					show_msg(database().at(*iter));
				}
			});
	}

	else if ((cmd == ".date") || (cmd == ".datetime"))
	{
		exec_takes_slot_command([&]() -> void
			{
				if (!database().contains(slot_bignum))
				{
					say_slot_doesnt_exist();
				}
				else
				{
					show_datetime(database().at(slot_bignum));
				}
			});
	}

	else if (cmd == ".whois")
	{
		exec_takes_slot_command([&]() -> void
			{
				if (!database().contains(slot_bignum))
				{
					say_slot_doesnt_exist();
				}
				else
				{
					say_owned_by();
				}
			});
	}

	else if (cmd == ".low")
	{
		comm.send_saveybot_msg("SaveyBot's lowest free slot is #",
			database().lowest_available_slot(), wow_suffix);
	}

	//else
	//{
	//	//comm.send_saveybot_msg("Unknown command.");
	//}
}



} // namespace saveybot

