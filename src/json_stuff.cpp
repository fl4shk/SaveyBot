#include "json_stuff.hpp"


std::string get_json_value_type_as_str(const Json::Value& some_value)
{
	switch (some_value.type())
	{
		case Json::nullValue:
			return std::string("nullValue");
		case Json::intValue:
			return std::string("intValue");
		case Json::uintValue:
			return std::string("uintValue");
		case Json::realValue:
			return std::string("realValue");
		case Json::stringValue:
			return std::string("stringValue");
		case Json::booleanValue:
			return std::string("booleanValue");
		case Json::arrayValue:
			return std::string("arrayValue");
		case Json::objectValue:
			return std::string("objectValue");
		default:
			return std::string();
	}
}


bool parse_json_file(Json::CharReaderBuilder& rbuilder, 
	const std::string& input_fname, Json::Value* root, std::string* errs)
{
	rbuilder["collectComments"] = false;

	std::fstream infile(input_fname, std::ios_base::in);

	if (!infile.is_open())
	{
		err("Can't open file called \"", input_fname, "\"!");
	}

	const bool ret = Json::parseFromStream(rbuilder, infile, root, errs);

	infile.close();

	return ret;
}


void debug_print_json(const Json::Value& some_value, std::ostream& os, 
	size_t tabs_level)
{
	auto print_leading_tabs = [&]() -> void
	{
		for (size_t i=0; i<tabs_level; ++i)
		{
			osprintout(os, "\t");
		}
	};

	auto print_leading_tabs_2 = [&]() -> void
	{
		print_leading_tabs();
		osprintout(os, "\t");
	};

	auto print_arrayValue = [&]() -> void
	{
		osprintout(os, "[\n");
		
		for (const auto& inner_iter : some_value)
		{
			print_leading_tabs_2();
			debug_print_json(inner_iter, os, tabs_level + 1);
		}

		print_leading_tabs();
		osprintout(os, "],\n");
	};
	
	auto print_objectValue = [&]() -> void
	{
		const auto& member_names = some_value.getMemberNames();

		osprintout(os, "{\n");

		for (const auto& name_iter : member_names)
		{
			print_leading_tabs_2();
			osprintout(os, "\"", name_iter, "\" : ");
			debug_print_json(some_value[name_iter], os, tabs_level + 1);
		}

		print_leading_tabs();
		osprintout(os, "},\n");
	};
	
	switch (some_value.type())
	{
		case Json::nullValue:
		case Json::intValue:
		case Json::uintValue:
		case Json::realValue:
		case Json::booleanValue:
			osprintout(os, some_value.asString(), ",\n");
			break;
		
		case Json::stringValue:
			osprintout(os, "\"", some_value.asString(), "\",\n");
			break;

		case Json::arrayValue:
			print_arrayValue();
			break;

		case Json::objectValue:
			print_objectValue();
			break;

		default:
			err("debug_print_json():  Unknown Json::Value type()!");
	}
}
