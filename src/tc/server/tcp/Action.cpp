#include <tc/server/tcp/Action.h>
#include <tc/parser/packet/PacketPayload.h>
#include <tc/parser/packet/PacketCommand.h>

namespace tc::server::tcp {

Action::Type Action::get(const uchar* buffer, size_t size)
{
	bool has_imei = hasImei(buffer, size);

	if (size == 1) {
		return Type::standby;
	}

	if (!has_imei && parser::PacketPayload::hasPayload(buffer, size)) {
		return Type::payload;
	}

	if (has_imei && size == 17) {
		return Type::imei;
	}

	if (has_imei && parser::PacketCommand::hasCommand(buffer, size)) {
		return Type::command;
	}

	if (size < 5) {
		return Type::incomplete_payload;
	}

	return Type::unknown;
}

bool Action::hasImei(const uchar *cbuf, size_t size)
{
	if (size < 15) {
		return false;
	}

	return ((cbuf[0]) << 8) | ((cbuf[1]) << 0) == 15 ? true : false;
}

result_t Action::parseImei(const uchar* cbuf, size_t size, Imei &imei)
{
	auto len = ((cbuf[0]) << 8) | ((cbuf[1]) << 0);
	if (len <= 2 || len > 17) {
		return RES_INVARG;
	}

	auto buf = cbuf + 2;
	auto from_buf = toImei(buf, len);

	imei = from_buf;
	return RES_OK;
}

const Action::Imei Action::toImei(const uchar* cbuf, int len)
{
	auto hex_str = tc::uchar2string(cbuf, len);

	Imei imei;
	for (Imei::size_type i = 1; i < hex_str.size(); i += 2) {
		char c = hex_str[i];
		imei.push_back(c);
	}

	return imei;
}

} // namespace tc::server::tcp
