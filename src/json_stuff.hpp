#ifndef json_stuff_hpp
#define json_stuff_hpp

#include "misc_helper_funcs.hpp"

std::string get_json_value_type_as_str(const Json::Value& some_value);
inline std::string get_json_value_type_as_str(Json::Value* iter)
{
	return std::move(get_json_value_type_as_str(*iter));
}

inline void print_json_value_type(const Json::Value& some_value,
	std::ostream& os=std::cout)
{
	osprintout(os, std::move(get_json_value_type_as_str(some_value)));
}
inline void print_json_value_type(Json::Value* iter, 
	std::ostream& os=std::cout)
{
	osprintout(os, std::move(get_json_value_type_as_str(iter)));
}

bool parse_json(Json::CharReaderBuilder* rbuilder, std::istream& is,
	Json::Value* root, std::string* errs);
bool parse_json(Json::CharReaderBuilder* rbuilder, 
	const std::string& input_file_name, Json::Value* root, 
	std::string* errs);

void write_json(Json::StreamWriterBuilder* wbuilder,
	std::ostream& os, Json::Value* root);
void write_json(Json::StreamWriterBuilder* wbuilder,
	const std::string& output_file_name, Json::Value* root);


// A recursive function for printing out the contents of a JSON file.
// Note that it does NOT print out valid JSON, just something similar.
// The main difference from real JSON is that every value ends with a
// comma, even when at the end of a list.
void debug_print_json(const Json::Value& some_value, 
	std::ostream& os=std::cout, size_t tabs_level=0);
inline void debug_print_json(Json::Value* iter, std::ostream& os=std::cout,
	size_t tabs_level=0)
{
	debug_print_json(*iter, os, tabs_level);
}



#endif		// json_stuff_hpp
