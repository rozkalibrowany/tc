#include <tc/parser/teltonika/packet/PacketCommand.h>
#include <algorithm>

namespace tc::parser::teltonika {

PacketCommand::PacketCommand(const std::string imei)
	: Packet(imei)
	, iCommand(nullptr)
{
		// nothing to do
}

bool PacketCommand::hasCommand(const uchar* cbuf, size_t size)
{
	auto val = (((cbuf[0]) << 8) | ((cbuf[1]) << 0));

	return (val == IMEI_LENGTH) && size > 17 ? true : false;
}

result_t PacketCommand::parse(const uchar* cbuf, size_t size, size_t offset)
{
	result_t res = RES_OK;

	auto begin = offset + 2;
	iCommand = (uchar *)cbuf + begin;
	iSize = size - begin;

	LG_NFO(this->logger(), "command: {}", tc::uchar2string(iCommand, iSize));

	return res;
}

const size_t PacketCommand::size()
{
	return iSize;
}

const uchar* PacketCommand::command()
{
	return iCommand;
}

} // namespace tc::parser::teltonika