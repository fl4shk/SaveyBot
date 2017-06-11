#include "real_main_class.hpp"


RealMain::RealMain(int argc, char** argv)
{
	//if (argc != 1)
	//{
	//	printerr("Usage:  ", argv[0], "\n");
	//	exit(1);
	//}

	// Temporary thing for converting the original database to a new one.
	if (argc != 3)
	{
		printerr("Usage:  ", argv[0], " input_json_file "
			"output_json_file\n");
		exit(1);
	}

	for (int i=0; i<argc; ++i)
	{
		__args_vec.push_back(std::string(argv[i]));
	}

	convert_original_database(args_vec().at(1), args_vec().at(2));

}

RealMain::~RealMain()
{
}

int RealMain::operator () ()
{
	return 0;
}

void RealMain::convert_original_database
	(const std::string& input_file_name, 
	const std::string& output_file_name)
{
	//Json::CharReaderBuilder rbuilder;
	Json::Value root;
	std::string errs;

	if (!parse_json(nullptr, input_file_name, &root, &errs))
	{
		err("Unable to parse file called \"", input_file_name, "\"!");
	}

	debug_print_json(root);

	//std::fstream temp_out_file(output_file_name, std::ios_base::out);

	//debug_print_json(root, temp_out_file);
}
