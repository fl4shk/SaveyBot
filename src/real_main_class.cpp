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

	gen_args_vec(argc, argv);
	//parse_args_vec();
	convert_original_database(args_vec().at(1), args_vec().at(2));
}

RealMain::~RealMain()
{
}

int RealMain::operator () ()
{
	return 0;
}


void RealMain::gen_args_vec(int argc, char** argv)
{
	for (int i=0; i<argc; ++i)
	{
		__args_vec.push_back(std::string(argv[i]));
	}
}

void RealMain::parse_args_vec()
{
	if (args_vec().size() == 1)
	{
		err("Need at least one argument (besides program name)");
	}

	bot().parse_command(args_vec(), 1);
}

void RealMain::convert_original_database
	(const std::string& input_file_name, 
	const std::string& output_file_name)
{
	Json::Value input_root, output_root;
	std::string errs;

	if (!parse_json(input_file_name, &input_root, &errs))
	{
		err("Unable to parse file called \"", input_file_name, "\"!");
	}

	{
		Json::ArrayIndex i = 0;
		
		for (auto& in_iter : input_root)
		{
			//auto& out_iter = output_root[i] = in_iter;
			std::string temp;
			std::stringstream ostm;
			ostm << i;
			ostm >> temp;

			auto& out_iter = output_root["savestates"][temp] = in_iter;
			out_iter["datetime"] = "classic";
			out_iter["index"] = i++;
		}

		output_root["num_savestates"] = i;
	}

	std::fstream temp_out_file(output_file_name, std::ios_base::out);
	write_json(temp_out_file, &output_root);
}
