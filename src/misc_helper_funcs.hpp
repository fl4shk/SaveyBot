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


#ifndef src_misc_helper_funcs_hpp
#define src_misc_helper_funcs_hpp

// src/misc_helper_funcs.hpp

#include "misc_includes.hpp"

namespace saveybot
{

template<typename... arg_types>
void err(arg_types&&... args)
{
	printerr("Error:  ", args..., "\n");
	exit(1);
}

} // namespace saveybot



#endif		// src_misc_helper_funcs_hpp
