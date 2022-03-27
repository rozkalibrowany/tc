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

int Packet::codec() const
{
	return iCodec;
}

void Packet::setImei(const std::string imei)
{
	iImei = imei;
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