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

#include "string_stuff.hpp"

namespace neosaveybot
{

bool find_next_non_blank_index(const std::string& some_str, 
	const size_t test_start_index, size_t& i)
{
	i = test_start_index;

	if (i >= some_str.size())
	{
		return false;
	}
	
	// Eat leading whitespace
	while (isspace(some_str.at(i)))
	{
		++i;

		if (i >= some_str.size())
		{
			return false;
		}
	}

	return true;
}
bool next_non_blank_substr(const std::string& some_str, 
	const size_t test_start_index, std::string& ret, size_t& i)
{
	//i = test_start_index;

	//// Eat leading whitespace
	//while (isspace(some_str.at(i)))
	//{
	//	++i;

	//	if (i >= some_str.size())
	//	{
	//		ret = "";
	//		return false;
	//	}
	//}

	if (!find_next_non_blank_index(some_str, test_start_index, i))
	{
		ret = "";
		return false;
	}

	const size_t start_index = i;

	while (i < some_str.size() && !isblank(some_str.at(i)))
	{
		++i;

		if (i >= some_str.size())
		{
			break;
		}
	}

	const size_t end_index = i;

	ret = std::move(some_str.substr(start_index, (end_index 
		- start_index)));
	return true;
}

}
