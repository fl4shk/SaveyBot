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


#ifndef src_real_main_class_hpp
#define src_real_main_class_hpp

// src/real_main_class.hpp

#include "irc_communicator_class.hpp"
#include "discord_communicator_class.hpp"
#include "saveybot_class.hpp"

namespace saveybot
{

class RealMain final
{
private:		// variables
	std::vector<std::string> _args_vec;
	SaveyBot _bot;

	//std::vector<IrcCommunicator> _irc_comm_vec;
	std::vector<IrcCommunicator*> _irc_comm_vec;
	IrcConfiguration _irc_config;
	std::unique_ptr<DiscordCommunicator> _discord_comm;

public:		// functions
	RealMain(int argc, char** argv);
	virtual ~RealMain();

	int operator () ();

	GEN_GETTER_BY_CON_REF(args_vec);
	GEN_GETTER_BY_CON_REF(irc_config);


private:		// functions
	GEN_GETTER_BY_REF(bot);
	GEN_GETTER_BY_REF(irc_comm_vec);

	void gen_args_vec(int argc, char** argv);

	//void _parse_args_vec_old();

	//static void _convert_original_database
	//	(const std::string& input_file_name, 
	//	const std::string& output_file_name);

	void one_thread_func_irc();
	void one_thread_func_discord();

};

} // namespace saveybot


#endif		// src_real_main_class_hpp
