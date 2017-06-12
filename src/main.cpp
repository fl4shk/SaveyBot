#include "real_main_class.hpp"


int main(int argc, char** argv)
{
	neosaveybot::RealMain real_main(argc, argv);
	return real_main();
}


//int old_real_main(const std::vector<std::string>& args_vec);
//
//int old_main(int argc, char** argv)
//{
//	std::vector<std::string> args_vec;
//
//	for (int i=0; i<argc; ++i)
//	{
//		args_vec.push_back(std::string(argv[i]));
//	}
//
//	if (args_vec.size() != 2)
//	{
//		printerr("Usage:  ", args_vec.at(0), " input_json_file\n");
//		return 1;
//	}
//
//
//	return old_real_main(args_vec);
//}
//
//
//int old_real_main(const std::vector<std::string>& args_vec)
//{
//	Json::CharReaderBuilder rbuilder;
//	Json::Value root;
//	std::string errs;
//	parse_json_file(rbuilder, args_vec.at(1), &root, &errs);
//	debug_print_json(root);
//
//	return 0;
//}
