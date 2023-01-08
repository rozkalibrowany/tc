#include <tc/server/tcp/Action.h>
#include <tc/parser/packet/PacketPayload.h>
#include <tc/parser/packet/Command.h>
#include <tc/parser/packet/InternalRequest.h>
namespace tc::server::tcp {

using namespace parser;

Action::Type Action::get(const uchar* buffer, size_t size)
{
	bool has_imei = packet::Packet::hasImei(buffer, size);

	if (size == 1) {
		return Type::standby;
	}

	if (!has_imei && packet::PacketPayload::hasPayload(buffer, size)) {
		return Type::payload;
	}

	auto has_request = packet::InternalRequest::hasInternalRequest(buffer, size);
	if (!has_imei && has_request) {
		return Type::request;
	}

	if (has_imei && size == 17) {
		return Type::imei;
	}

	if (has_imei && packet::Command::hasCommand(buffer, size)) {
		return Type::command;
	}

	return Type::unknown;
}

} // namespace tc::server::tcp
