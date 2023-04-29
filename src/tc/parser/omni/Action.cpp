#include <tc/parser/omni/Action.h>

namespace tc::parser::omni::action {

using namespace parser;

Locker::Event Locker::get(const common::Buf &buf)
{
	// getting index of instruction segment
	auto index = buf.find_nth(',', 4);
	if (!index || index + 2 > (int) buf.size()) // index + size of instruction
		return Event::eUnknown;
	// getting instruction
	std::string type((const char *)buf.cdata() + index, 2);
	return (Event) strtoul(tohex(type).c_str(), 0, 16);
}

Server::Event Server::get(const common::Buf &buf)
{
	// getting index of instruction segment
	auto index = buf.find_nth(',', 4);
	if (!index || index + 2 > (int) buf.size()) // index + size of instruction
		return eUnknown;
	// getting instruction
	std::string type((const char *)buf.cdata() + index, 2);
	return (Event) strtoul(tohex(type).c_str(), 0, 16);
}

} // namespace tc::parser::omni::action
