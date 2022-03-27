#include <tc/parser/packet/PacketCommand.h>
#include <algorithm>

namespace tc::parser {

size_t PacketCommand::DATA_MIN_SIZE = 75;

PacketCommand::PacketCommand(const std::string imei)
	: Packet(imei)
{
		// nothing to do
}

bool PacketCommand::hasCommand(const uchar* cbuf, size_t size)
{
	return false;
	bool isCommand = false;
	if (size < PacketCommand::DATA_MIN_SIZE) {
		return isCommand;
	}

	isCommand = ((int)cbuf[0] > 0);

	return isCommand;
}

result_t PacketCommand::parse(const uchar* cbuf, size_t size)
{
	return RES_NOENT;
}

const size_t PacketCommand::size()
{
	return 0;
}

} // namespace tc::parser