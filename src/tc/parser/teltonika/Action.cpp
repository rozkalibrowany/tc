#include <tc/parser/teltonika/Action.h>
#include <tc/parser/teltonika/packet/Payload.h>
#include <tc/parser/teltonika/Command.h>
#include <tc/parser/internal/Request.h>

namespace tc::parser::teltonika {

using namespace parser;

Action::Type Action::get(const uchar* buffer, size_t size)
{
	bool has_imei = parser::Packet::hasImei(buffer, size);

	if (size == 1) {
		return Type::eStandby;
	}

	if (!has_imei && parser::teltonika::Payload::hasPayload(buffer, size)) {
		return Type::ePayload;
	}

	if (has_imei && size == 17) {
		return Type::eImei;
	}

	return Type::eUnknown;
}

} // namespace tc::parser::teltonika
