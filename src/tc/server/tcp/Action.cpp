#include <tc/server/tcp/Action.h>
#include <tc/parser/teltonika/packet/PacketPayload.h>
#include <tc/parser/teltonika/packet/PacketCommand.h>
#include <tc/parser/InternalRequest.h>

namespace tc::server::tcp {

using namespace parser;

Action::Type Action::get(const uchar* buffer, size_t size)
{
	bool has_imei = parser::Packet::hasImei(buffer, size);

	if (size == 1) {
		return Type::standby;
	}

	if (!has_imei && parser::teltonika::PacketPayload::hasPayload(buffer, size)) {
		return Type::payload;
	}

	if (!has_imei && parser::InternalRequest::hasRequest(buffer, size)) {
		return Type::request;
	}

	if (has_imei && size == 17) {
		return Type::imei;
	}

	if (has_imei && teltonika::PacketCommand::hasCommand(buffer, size)) {
		return Type::command;
	}

	return Type::unknown;
}

} // namespace tc::server::tcp
