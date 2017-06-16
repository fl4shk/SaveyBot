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


#ifndef real_main_class_hpp
#define real_main_class_hpp

#include "irc_communicator_class.hpp"
#include "neosaveybot_class.hpp"

namespace neosaveybot
{

class RealMain
{
private:		// variables
	std::vector<std::string> __args_vec;
	NeoSaveyBot __bot;

	std::vector<IRCCommunicator> __irc_comm_vec;
	IRCConfiguration __irc_config;

public:		// functions
	RealMain(int argc, char** argv);
	~RealMain();

	int operator () ();

	gen_getter_by_con_ref(args_vec);
	gen_getter_by_con_ref(irc_config);


private:		// functions
	gen_getter_by_ref(bot);
	gen_getter_by_ref(irc_comm_vec);

	void gen_args_vec(int argc, char** argv);

	void parse_args_vec();

	static void __convert_original_database
		(const std::string& input_file_name, 
		const std::string& output_file_name);

};

}


#endif		// real_main_class_hpp
