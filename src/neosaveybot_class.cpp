#include "neosaveybot_class.hpp"

namespace neosaveybot
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
	datetime = to_copy[Database::key_datetime].asString();
	classic_index_str = to_copy[Database::key_classic_index].asString();
	message = to_copy[Database::key_message].asString();
	name = to_copy[Database::key_name].asString();
	slot = to_copy[Database::key_slot].asString();
}
void Database::Value::add_to_json(Json::Value& output_root) const
{
	output_root[slot][Database::key_datetime] = datetime;

	s64 classic_index;
	std::stringstream sstm(classic_index_str);
	sstm >> classic_index;
	output_root[slot][Database::key_classic_index] = classic_index;

	output_root[slot][Database::key_message] = message;
	output_root[slot][Database::key_name] = name;
	output_root[slot][Database::key_slot] = slot;
}

void Database::save(const std::string& message, const std::string& name, 
	const std::string& slot)
{
	// Blank slot, so save using __lowest_available_slot
	if (slot.size() == 0)
	{
		std::string another_slot;
		std::stringstream slot_sstm;
		slot_sstm << lowest_available_slot();
		slot_sstm >> another_slot;

		save(message, name, another_slot);
		update_lowest_available_slot();

		return;
	}

	std::string datetime;
	std::stringstream dt_sstm;
	osprintout(dt_sstm, liborangepower::time::put_now_as_localtime(),
		"\n");
	getline(dt_sstm, datetime);

	savestates[slot] = std::move(Value(datetime, "-1", message, name,
		slot));
	
	write_file();
}

void Database::write_file() const
{
	Json::Value output_root;

	for (const auto& iter : savestates)
	{
		iter.second.add_to_json(output_root);
	}

	//std::fstream out_file(std::string("temp_") + database_file_name(), 
	//	std::ios_base::out);
	std::fstream out_file(database_file_name(), std::ios_base::out);
	write_json(out_file, &output_root);
}
void Database::load_from_file()
{
	Json::Value input_root;
	std::string errs;
	
	parse_json(database_file_name(), &input_root, &errs);

	for (auto& in_iter : input_root)
	{
		Value to_insert(in_iter);
		savestates[to_insert.slot] = std::move(to_insert);
	}

	set_lowest_available_slot(0);
	update_lowest_available_slot();
}

void Database::update_lowest_available_slot()
{
	printout("I started with this lowest_available_slot:  ",
		lowest_available_slot(), "\n");

	for ( ; 
		//lowest_available_slot()<max_automatic_slot; 
		;
		__lowest_available_slot++)
	{
		//if (lowest_available_slot() == max_automatic_slot)
		//{
		//	err("Can't have automatic slot as high as",
		//		lowest_available_slot(), "!");
		//}

		std::string las_str;
		std::stringstream las_sstm;
		las_sstm << lowest_available_slot();
		las_sstm >> las_str;

		const auto search = savestates.find(las_str);

		if (search == savestates.end())
		{
			break;
		}
	}

	printout("I ended with this lowest available slot:  ",
		lowest_available_slot(), "\n\n");
}

const std::string NeoSaveyBot::config_file_name("config.json"),
	NeoSaveyBot::database_file_name("neosaveybot.json");


NeoSaveyBot::Configuration::Configuration()
{
	Json::Value config_root;
	std::string errs;

	parse_json(config_file_name, &config_root, &errs);

	const auto& member_names = config_root.getMemberNames();

	for (const auto& name_iter : member_names)
	{
		Server to_push;

		to_push.name = name_iter;
		const auto& iter = config_root[name_iter];
		to_push.bot_name = iter["bot_name"].asString();
		to_push.address = iter["address"].asString();
		to_push.port_str = iter["port"].asString();
		to_push.password = iter["password"].asString();

		for (const auto& join_iter : iter["joins_list"])
		{
			to_push.joins_list.push_back(join_iter.asString());
		}

		for (const auto& command_iter : iter["startup_commands"])
		{
			//to_push.startup_commands.push_back(command_iter.asString());

			std::string full_command;

			for (const auto& inner_iter : command_iter)
			{
				full_command += inner_iter.asString();
			}

			to_push.startup_commands.push_back(std::move(full_command));
		}

		server_vec.push_back(to_push);
	}

	for (const auto& iter : server_vec)
	{
		printout(iter);
	}
	printout("\n");

}


NeoSaveyBot::NeoSaveyBot() : __database(database_file_name)
{
}

NeoSaveyBot::~NeoSaveyBot()
{
}



void NeoSaveyBot::parse_command_basic
	(const std::vector<std::string>& args_vec)
{
	const size_t start_index = 1;
	const size_t size = args_vec.size() - start_index;

	const std::string& command = args_vec.at(start_index);


	auto print_found_command = [&command]() -> void
	{
		printout("Found a \"", command, "\".\n");
	};
	auto say_invalid_num_params = [&command]() -> void
	{
		printout("Invalid number of parameters for \"", command, "\".\n");
	};

	if (command == ".road")
	{
		print_found_command();
	}

	else if (command == ".save")
	{
		print_found_command();
		
		//if (size != 2)
		//{
		//	say_invalid_num_params();
		//	return;
		//}

		if ((size < 2) || (size > 3))
		{
			say_invalid_num_params();
			return;
		}

		else if (size == 2)
		{
			__database.save(args_vec.at(start_index + 1), 
				"--Command Line Test--", "");
		}

		else // if (size == 3)
		{
			{
				mpz_class slot;
				if (convert_str_to_bignum(args_vec.at(start_index + 1), 
					slot))
				{
					printout("Invalid save slot\n");
					return;
				}
			}

			__database.save(args_vec.at(start_index + 2), 
				"--Command Line Test--", args_vec.at(start_index + 1));
			
		}


	}

	else
	{
		printout("Unknown command.\n");
	}

}


std::ostream& operator << (std::ostream& os, 
	const NeoSaveyBot::Configuration::Server& to_print)
{
	osprintout(os, "name:  ", to_print.name, "\n");
	osprintout(os, "\tbot_name:  ", to_print.bot_name, "\n");
	osprintout(os, "\taddress:  ", to_print.address, "\n");
	osprintout(os, "\tport_str:  ", to_print.port_str, "\n");
	osprintout(os, "\tpassword:  ", to_print.password, "\n");

	osprintout(os, "\tjoins_list:  \n");
	for (const auto& iter : to_print.joins_list)
	{
		osprintout(os, "\t\t", iter, "\n");
	}

	osprintout(os, "\tstartup_commands:  \n");
	for (const auto& iter : to_print.startup_commands)
	{
		osprintout(os, "\t\t", iter, "\n");
	}

	return os;
}

}
