#include <tc/parser/packet/Packet.h>
#include <algorithm>

namespace tc::parser {

Packet::Packet(const std::string imei)
	: tc::LogI("console")
	, iCodec(0)
{
	// nothing to do
}

bool Packet::hasImei(const uchar *cbuf, size_t size)
{
	if (size < 15) {
		return false;
	}

	return ((cbuf[0]) << 8) | ((cbuf[1]) << 0) == 15 ? true : false;
}

result_t Packet::parseImei(const uchar* cbuf, size_t size, Imei &imei)
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

const Imei Packet::toImei(const uchar* cbuf, int len)
{
	auto hex_str = tc::uchar2string(cbuf, len);

	Imei imei;
	for (Imei::size_type i = 1; i < hex_str.size(); i += 2) {
		char c = hex_str[i];
		imei.push_back(c);
	}

	return imei;
}

int Packet::codec() const
{
	return iCodec;
}

const Packet::ID &Packet::id() const
{
	return iID;
}

bool Packet::operator<(const Packet &rhs) const
{
	return iID.timestamp.timestamp() < rhs.iID.timestamp.timestamp();
}

bool Packet::operator<=(const Packet &rhs) const
{
	return iID.timestamp.timestamp() <= rhs.iID.timestamp.timestamp();
}

bool Packet::operator>(const Packet &rhs) const
{
	return iID.timestamp.timestamp() > rhs.iID.timestamp.timestamp();
}

bool Packet::operator>=(const Packet &rhs) const
{
	return iID.timestamp.timestamp() >= rhs.iID.timestamp.timestamp();
}

bool Packet::operator==(const Packet &rhs) const
{
	return iID.timestamp.timestamp() == rhs.iID.timestamp.timestamp();
}

} // namespace tc::parser