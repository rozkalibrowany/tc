#include <tc/parser/Protocol.h>
#include <tc/parser/teltonika/packet/Payload.h>

namespace tc::parser {

result_t Protocol::parse(const uchar *buffer, size_t size)
{
	if (teltonika::Payload::isTeltonika(buffer, size)) {
		iType = eTeltonika;
		return RES_OK;
	}

	return RES_NOENT;
}

Protocol::Type Protocol::type() const
{
	return iType;
}

} // namespace tc::parser
