#include <tc/server/tcp/Action.h>
#include <tc/parser/packet/PacketPayload.h>
#include <tc/parser/packet/PacketCommand.h>
#include <tc/parser/packet/PacketRequest.h>
namespace tc::server::tcp {

Action::Type Action::get(const uchar* buffer, size_t size)
{
	bool has_imei = parser::Packet::hasImei(buffer, size);

	if (size == 1) {
		return Type::standby;
	}

	if (parser::PacketPayload::hasPayload(buffer, size)) {
		return Type::payload;
	}

	if (!has_imei && parser::PacketRequest::hasRequest(buffer, size)) {
		return Type::request;
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

} // namespace tc::server::tcp
