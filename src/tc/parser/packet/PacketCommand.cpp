#include <tc/parser/packet/PacketCommand.h>
#include <algorithm>

namespace tc::parser {

size_t PacketCommand::DATA_SIZE = 49; // to FIX - fixed size

PacketCommand::PacketCommand(const std::string imei)
	: Packet(imei)
{
		// nothing to do
}

bool PacketCommand::hasCommand(const uchar* cbuf, size_t size)
{
	if (size != PacketCommand::DATA_SIZE) {
		return false;
	}

	auto val = (((cbuf[0]) << 8) | ((cbuf[1]) << 0));
	return val > 0 ? true : false;
}

result_t PacketCommand::parse(uchar* cbuf, size_t size)
{
	result_t res = RES_OK;

	if((res = parseImei(cbuf, size)) != RES_OK) {
		LG_NFO(this->logger(), "Error parsing command: {}", tc::uchar2string(cbuf, size));
		return res;
	}

	auto offset = imei().length() + 2;
	LG_NFO(this->logger(), "PacketCommand offset: {}", offset);
	iCommand = (uchar *)cbuf + offset;
	iSize = size - offset;

	LG_NFO(this->logger(), "command: {}", tc::uchar2string(iCommand, iSize));

	return RES_OK;
}

const std::string PacketCommand::imei() const
{
	return iImei;
}

const size_t PacketCommand::size()
{
	return iSize;
}

const uchar* PacketCommand::command()
{
	return iCommand;
}

} // namespace tc::parser