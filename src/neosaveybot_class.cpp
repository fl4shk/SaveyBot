#include "neosaveybot_class.hpp"

namespace neosaveybot
{


// Constant keys
const std::string Database::key_datetime("datetime"),
	Database::key_index("index"),
	Database::key_message("message"),
	Database::key_name("name"),
	Database::key_slot("slot");

// Constant values
const std::string Database::value_datetime_classic("classic");

void Database::Value::add_to_json(Json::Value& output_root) const
{
	output_root[slot][Database::key_datetime] = datetime;
	output_root[slot][Database::key_index] = index_str;
	output_root[slot][Database::key_message] = message;
	output_root[slot][Database::key_name] = name;
	output_root[slot][Database::key_slot] = slot;
}

void Database::write_file() const
{
	Json::Value output_root;

	for (const auto& iter : savestates)
	{
		iter.second.add_to_json(output_root);
	}

	std::fstream out_file(std::string("temp_") + database_file_name(), 
		std::ios_base::out);
	write_json(out_file, &output_root);
}
void Database::load_from_file()
{
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


void NeoSaveyBot::parse_command(const std::vector<std::string>& command, 
	size_t start_index, size_t end_index)
{
	if (end_index == 0)
	{
		end_index = command.size();
	}

	const std::string& name = command.at(start_index);

	if (name == ".road")
	{
		printout("Found a .road.\n");
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
