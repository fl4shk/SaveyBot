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


#ifndef string_stuff_hpp
#define string_stuff_hpp

#include "misc_includes.hpp"

namespace neosaveybot
{

bool find_next_non_blank_index(const std::string& whole_cmd_str,
	const size_t test_start_index, size_t& i);
bool next_non_blank_substr(const std::string& whole_cmd_str, 
	const size_t test_start_index, std::string& ret, size_t& i);

}


#endif		// string_stuff_hpp
