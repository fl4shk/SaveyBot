#include "real_main_class.hpp"


RealMain::RealMain(int argc, char** argv)
{
	if (argc != 1)
	{
		printerr("Usage:  ", argv[0], "\n");
		exit(1);
	}

	for (int i=0; i<argc; ++i)
	{
		__args_vec.push_back(std::string(argv[i]));
	}
}

RealMain::~RealMain()
{
}

int RealMain::operator () ()
{
	
	return 0;
}
