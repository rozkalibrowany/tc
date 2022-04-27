#include <tc/parser/packet/PacketCommand.h>
#include <algorithm>

namespace tc::parser {

size_t PacketCommand::DATA_SIZE = 42;

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

	return (((cbuf[0]) << 8) | (((cbuf[1]) << 0) > 0)) > 0 ? true : false;
}

result_t PacketCommand::parse(const uchar* cbuf, size_t size)
{
	result_t res = RES_OK;

	if((res = parseImei(cbuf, size)) != RES_OK) {
		LG_NFO(this->logger(), "Error parsing command: {}", tc::unsigned_char_to_string(cbuf, size));
		return res;
	}

	auto offset = imei().size() % 2 != 0 ? (imei().size() / 2) + 1 : imei().size() / 2;
	iCommand = (uchar*) cbuf + offset + 2;
	iSize = size - offset - 2;
	
	return RES_OK;
}

result_t PacketCommand::parseImei(const uchar* cbuf, size_t size)
{
	auto imei_len = cbuf[1] << 0;
	if (imei_len <= 0) {
		return RES_NOENT;
	}

	auto buf = cbuf + 2;
	auto hex_str = tc::unsigned_char_to_string(buf, size - 2);

	std::string imei;
	for (std::string::size_type i = 0; i < imei_len; i++)
	{
		char c = hex_str[i];
		imei.push_back(c);
	}

	LG_NFO(this->logger(), "Got IMEI: {} len: {}", imei, imei_len);

	iImei = imei;
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

uchar* PacketCommand::command()
{
	return iCommand;
}

} // namespace tc::parser