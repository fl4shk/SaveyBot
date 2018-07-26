// This file is part of SaveyBot.
// 
// Copyright 2017-2018 Andrew Clark (FL4SHK).
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


#include "real_main_class.hpp"
#include "select_stuff.hpp"

namespace saveybot
{

RealMain::RealMain(int argc, char** argv)
{
	//if (argc != 1)
	//{
	//	printerr("Usage:  ", argv[0], "\n");
	//	exit(1);
	//}

	//// Temporary thing for converting the original database to a new one.
	//if (argc != 3)
	//{
	//	printerr("Usage:  ", argv[0], " input_json_file "
	//		"output_json_file\n");
	//	exit(1);
	//}

	gen_args_vec(argc, argv);
	//parse_args_vec();
	
	//__convert_original_database(args_vec().at(1), args_vec().at(2));
}

RealMain::~RealMain()
{
	for (size_t i=0; i<irc_comm_vec().size(); ++i)
	{
		delete irc_comm_vec()[i];
	}
}

int RealMain::operator () ()
{
	for (auto& iter : irc_config().server_vec())
	{
		irc_comm_vec().push_back(new IrcCommunicator(&__bot, 
			&iter));
	}

	fd_set readfds;

	for (;;)
	{
		do_select_for_read(irc_comm_vec(), readfds);
		for (auto iter : irc_comm_vec())
		{
			printout("RealMain:  Server:  ", iter->config_server().name(), 
				"\n");

			//iter->update_buf_str(&readfds);
			//iter->iterate();

			if (!iter->__state.did_connect)
			{
				iter->__reinit();
				continue;
			}


			if (iter->update_buf_str(&readfds))
			{
				//printout("egg\n");
				while (iter->can_iterate())
				{
					iter->iterate();
				}
				//printout("bert\n");
			}

			//if (iter->__state.did_joins)
			//{
			//	if (iter->update_buf_str(&readfds))
			//	{
			//		printout("egg\n");
			//		//while (iter->can_iterate(&readfds))
			//		//{
			//		//	iter->iterate(&readfds);
			//		//}
			//		while (iter->can_iterate())
			//		{
			//			iter->iterate();
			//		}
			//	}
			//}
			//else // if (!iter->__state.did_joins)
			//{
			//	iter->update_buf_str(&readfds);
			//	iter->iterate();
			//}
		}
	}

	

	return 0;
}


void RealMain::gen_args_vec(int argc, char** argv)
{
	for (int i=0; i<argc; ++i)
	{
		__args_vec.push_back(std::string(argv[i]));
	}
}

//void RealMain::__parse_args_vec_old()
//{
//	//if (args_vec().size() == 1)
//	//{
//	//	err("Need at least one argument (besides program name)");
//	//}
//
//	//bot().parse_command_basic(args_vec());
//
//	if (args_vec().size() <= 2)
//	{
//		err("Need at least two arguments (besides program name).");
//	}
//	bot().parse_command(args_vec().at(1), args_vec().at(2));
//}

//void RealMain::__convert_original_database
//	(const std::string& input_file_name, 
//	const std::string& output_file_name)
//{
//	Json::Value input_root, output_root;
//	std::string errs;
//
//	parse_json(input_file_name, &input_root, &errs);
//
//	{
//		Json::ArrayIndex i = 0;
//		
//		for (auto& in_iter : input_root)
//		{
//			//auto& out_iter = output_root[i] = in_iter;
//			std::string temp;
//			std::stringstream ostm;
//			ostm << i;
//			ostm >> temp;
//
//			//auto& out_iter = output_root["savestates"][temp] = in_iter;
//			auto& out_iter = output_root[temp] = in_iter;
//			out_iter["datetime"] = "classic";
//			out_iter["classic_index"] = i++;
//		}
//
//		//output_root["num_savestates"] = i;
//	}
//
//	std::fstream temp_out_file(output_file_name, std::ios_base::out);
//	write_json(temp_out_file, &output_root);
//}

}
