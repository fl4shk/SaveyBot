#include "communicator_class.hpp"

#include "saveybot_class.hpp"

namespace saveybot
{

const std::string Communicator::config_file_name("config.json");

Communicator::Communicator(SaveyBot* s_bot_ptr)
	: _bot_ptr(s_bot_ptr)
{
}

SaveyBot& Communicator::bot() const
{
	return *_bot_ptr;
}

} // namespace saveybot
