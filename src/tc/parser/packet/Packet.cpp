#include <tc/parser/packet/Packet.h>
#include <algorithm>

namespace tc::parser {

Packet::Packet(const ID &id)
	: tc::LogI("console")
	, iID(id)
	, iCodec(0)
	, iTotalRecords(0)
{
	// nothing to do
}

Packet::Packet(bool time_now)
 : Packet(ID(time_now))
{
 // nothing to do
}

int Packet::codec() const
{
	return iCodec;
}

int Packet::totalRecords() const
{
	return iTotalRecords;
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