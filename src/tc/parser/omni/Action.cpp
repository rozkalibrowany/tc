#include <tc/parser/omni/Action.h>
#include <tc/parser/omni/packet/Payload.h>

namespace tc::parser::omni::action {

using namespace parser;

Locker::Instruction Locker::get(const common::Buf &buf)
{
	// getting index of instruction segment
	auto index = buf.find_nth(',', 4);
	if (!index || index + 2 > (int) buf.size()) // index + size of instruction
		return eUnknown;
	// getting instruction
	std::string type((const char *)buf.cdata() + index, 2);
	return (Instruction) strtoul(tohex(type).c_str(), 0, 16);
}

Server::Instruction Server::get(const common::Buf &buf)
{
	// getting index of instruction segment
	auto index = buf.find_nth(',', 4);
	if (!index || index + 2 > (int) buf.size()) // index + size of instruction
		return eUnknown;
	// getting instruction
	std::string type((const char *)buf.cdata() + index, 2);
	return (Instruction) strtoul(tohex(type).c_str(), 0, 16);
}

} // namespace tc::parser::omni::action
