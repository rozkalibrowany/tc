#include <tc/parser/Protocol.h>
#include <tc/parser/teltonika/packet/Payload.h>
#include <tc/parser/omni/packet/Payload.h>

namespace tc::parser {

Protocol::Protocol(Type type)
 : iType(type)
{
	// nothing to do
}

result_t Protocol::parse(const uchar *buffer, size_t size)
{
	if (teltonika::Payload::valid(buffer, size)) {
		iType = eTeltonika;
		return RES_OK;
	}

	if (omni::Payload::valid(buffer, size)) {
		iType = eOmni;
		return RES_OK;
	}

	return RES_NOENT;
}

Protocol::Type Protocol::type() const
{
	return iType;
}

} // namespace tc::parser
