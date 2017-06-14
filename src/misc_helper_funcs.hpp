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


#ifndef misc_helper_funcs_hpp
#define misc_helper_funcs_hpp

#include "misc_includes.hpp"

namespace neosaveybot
{

template<typename... arg_types>
void err(arg_types&&... args)
{
	printerr("Error:  ", args..., "\n");
	exit(1);
}

}


#endif		// misc_helper_funcs_hpp
