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

bool eat_specific_chars(const std::string& some_str,
	const size_t test_start_index, size_t& i,
	const CharTesterFunc& lead_tester)
{
	//printout("test_start_index, i:  ", test_start_index, " ", i, "\n");
	i = test_start_index;

	if (i >= some_str.size())
	{
		return false;
	}

	//printout(some_str.at(i), "\t");
	
	// Eat leading specific characters
	while (lead_tester(some_str.at(i)))
	{
		++i;

		if (i >= some_str.size())
		{
			return false;
		}
	}
	//printout("i:  ", i, "\n");

	return true;
}
bool next_non_specific_chars_substr(const std::string& some_str, 
	const size_t test_start_index, std::string& ret, size_t& i,
	const CharTesterFunc& lead_tester,
	const CharTesterFunc& trail_tester)
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

	if (!eat_specific_chars(some_str, test_start_index, i, lead_tester))
	{
		ret = "";
		return false;
	}

	const size_t start_index = i;

	while ((i < some_str.size()) && !trail_tester(some_str.at(i)))
	{
		//printout("i:  ", i, "\n");
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

std::string substr_trimmed(const std::string& some_str, const size_t i,
	const CharTesterFunc& trail_tester)
{
	std::string temp = std::move(some_str.substr(i));

	s64 last = temp.size() - 1;

	for (; last>=0; --last)
	{
		if (!trail_tester(temp.at(last)))
		{
			break;
		}
	}

	return temp.substr(0, last + 1);

}

}
