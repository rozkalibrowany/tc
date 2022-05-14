#include <tc/parser/packet/Packet.h>
#include <algorithm>

namespace tc::parser {

Packet::Packet(const std::string imei)
	: tc::LogI("console")
	, iImei(imei)
	, iCodec(0)
{
	// nothing to do
}

result_t Packet::parseImei(uchar* cbuf, size_t size)
{
	auto len = ((cbuf[0]) << 8) | ((cbuf[1]) << 0);
	if (len <= 2 || len > 17) {
		LG_NFO(this->logger(), "Insufficient data to parse IMEI");
		return RES_INVARG;
	}

	auto buf = cbuf + 2;
	auto imei = toImei(buf, len);

	LG_NFO(this->logger(), "Parse imei. Got IMEI: {} len: {}", imei, len);

	iImei = imei;
	return RES_OK;
}

int Packet::codec() const
{
	return iCodec;
}

void Packet::setImei(const std::string imei)
{
	iImei = imei;
}


const std::string Packet::toImei(const uchar* cbuf, int len)
{
	auto hex_str = tc::uchar2string(cbuf, len);

	std::string imei;
	for (std::string::size_type i = 1; i < hex_str.size(); i += 2) {
		char c = hex_str[i];
		imei.push_back(c);
	}

	return imei;
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