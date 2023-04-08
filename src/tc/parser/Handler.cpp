#include <tc/parser/Handler.h>
#include <tc/parser/teltonika/packet/Payload.h>

namespace tc::parser {

result_t Handler::parse(const uchar *buffer, size_t size)
{
	if (teltonika::Payload::isTeltonika(buffer, size)) {
		iProtocol = eTeltonika;
		return RES_OK;
	}

	return RES_NOENT;
}

Handler::Protocol Handler::protocol() const
{
	return iProtocol;
}

} // namespace tc::parser
