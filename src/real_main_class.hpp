#ifndef real_main_class_hpp
#define real_main_class_hpp

#include "neosaveybot_class.hpp"

class RealMain
{
private:		// variables
	std::vector<std::string> __args_vec;
	NeoSaveyBot __bot;

public:		// functions
	RealMain(int argc, char** argv);
	~RealMain();

	int operator () ();

	gen_getter_by_con_ref(args_vec);


private:		// functions
	gen_getter_by_ref(bot);

	void gen_args_vec(int argc, char** argv);

	void parse_args_vec();

	static void convert_original_database
		(const std::string& input_file_name, 
		const std::string& output_file_name);

};


#endif		// real_main_class_hpp
