// This file is part of SaveyBot.
// 
// Copyright 2017 Andrew Clark (FL4SHK).
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


#ifndef misc_includes_hpp
#define misc_includes_hpp

#include <string>
#include <sstream>
#include <fstream>
#include <memory>
#include <map>
#include <functional>
#include <cstring>
#include <cctype>


// POSIX headers
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>


// jsoncpp headers
#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>


// GMP headers
#include <gmpxx.h>



#include "liborangepower_src/misc_includes.hpp"
#include "liborangepower_src/misc_output_classes.hpp"
#include "liborangepower_src/misc_types.hpp"
#include "liborangepower_src/gen_getter_setter_defines.hpp"
#include "liborangepower_src/time_stuff.hpp"
#include "liborangepower_src/memwhatever_wrappers.hpp"


using namespace liborangepower::printout_etc;
using namespace liborangepower::integer_types;
using namespace liborangepower::misc_util;




#endif		// misc_includes_hpp
