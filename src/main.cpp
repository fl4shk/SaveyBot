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


#include "real_main_class.hpp"


int main(int argc, char** argv)
{
	neosaveybot::RealMain real_main(argc, argv);
	return real_main();
}


//int old_real_main(const std::vector<std::string>& args_vec);
//
//int old_main(int argc, char** argv)
//{
//	std::vector<std::string> args_vec;
//
//	for (int i=0; i<argc; ++i)
//	{
//		args_vec.push_back(std::string(argv[i]));
//	}
//
//	if (args_vec.size() != 2)
//	{
//		printerr("Usage:  ", args_vec.at(0), " input_json_file\n");
//		return 1;
//	}
//
//
//	return old_real_main(args_vec);
//}
//
//
//int old_real_main(const std::vector<std::string>& args_vec)
//{
//	Json::CharReaderBuilder rbuilder;
//	Json::Value root;
//	std::string errs;
//	parse_json_file(rbuilder, args_vec.at(1), &root, &errs);
//	debug_print_json(root);
//
//	return 0;
//}
