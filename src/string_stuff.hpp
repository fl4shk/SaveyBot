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

typedef std::function<int(int)> CharTesterFunc;

bool eat_specific_chars(const std::string& some_str,
	const size_t test_start_index, size_t& i,
	const CharTesterFunc& lead_tester);

bool next_non_specific_chars_substr(const std::string& some_str,
	const size_t test_start_index, std::string& ret, size_t& i,
	const CharTesterFunc& lead_tester,
	const CharTesterFunc& trail_tester);

inline bool eat_whitespace(const std::string& some_str,
	const size_t test_start_index, size_t& i)
{
	return eat_specific_chars(some_str, test_start_index, i,
		CharTesterFunc(isspace));
}
inline bool next_non_blank_substr(const std::string& some_str, 
	const size_t test_start_index, std::string& ret, size_t& i)
{
	//return next_non_specific_chars_substr(some_str, test_start_index, ret, 
	//	i, CharTesterFunc(isspace), CharTesterFunc(isblank));
	return next_non_specific_chars_substr(some_str, test_start_index, ret, 
		i, CharTesterFunc(isspace), CharTesterFunc(isblank));
}


}


#endif		// string_stuff_hpp
