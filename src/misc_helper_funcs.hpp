#ifndef misc_helper_funcs_hpp
#define misc_helper_funcs_hpp

#include "misc_includes.hpp"

template<typename... arg_types>
void err(arg_types&&... args)
{
	printerr("Error:  ", args..., "\n");
	exit(1);
}


#endif		// misc_helper_funcs_hpp
