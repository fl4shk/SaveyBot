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


#ifndef gmp_stuff_hpp
#define gmp_stuff_hpp

#include "misc_includes.hpp"

namespace saveybot
{

typedef mpz_class BigNum;

bool str_is_integer_bignum(const std::string& str, BigNum& ret);

inline std::string convert_bignum_to_str(const BigNum& in_bignum)
{
	std::string ret;
	std::stringstream sstm;
	sstm << in_bignum;
	sstm >> ret;
	return ret;
}

}

#endif		// gmp_stuff_hpp
