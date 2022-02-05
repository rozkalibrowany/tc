#include <tc/parser/packet/PacketCommand.h>
#include <algorithm>

namespace tc::parser {

size_t PacketCommand::DATA_MIN_SIZE = 75;

PacketCommand::PacketCommand(const ID &id)
	: Packet(id)
{
		// nothing to do
}

PacketCommand::PacketCommand(bool time_now)
 : PacketCommand(ID(time_now))
{
 // nothing to do
}

bool PacketCommand::hasCommand(const uchar* cbuf, size_t size)
{
	bool isCommand = false;
	if (size < PacketCommand::DATA_MIN_SIZE) {
		return isCommand;
	}

	isCommand = ((int)cbuf[0] > 0);

	return isCommand;
}

result_t PacketCommand::parse(uchar* cbuf, size_t size)
{
	return RES_NOENT;
}

} // namespace tc::parser