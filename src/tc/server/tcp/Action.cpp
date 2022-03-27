#include <tc/server/tcp/Action.h>
#include <tc/parser/packet/PacketPayload.h>
#include <tc/parser/packet/PacketCommand.h>

namespace tc::server::tcp {

Action::Action(Type type)
 : iType(type)
{
	// nothing to do
}

Action::~Action()
{

}

const Action::Type Action::type() const
{
	return iType;
}

result_t Action::parse(const void* buffer, size_t size)
{
	auto type = Type::unknown;

	if (parser::PacketPayload::hasPayload((const uchar*) buffer, size) == true) {
		type = Type::payload;
	}

	if (parser::PacketPayload::hasPayloadImei((const uchar*) buffer, size) == true) {
		type = Type::payload_imei;
	}

	if (parser::PacketCommand::hasCommand((const uchar*) buffer, size) == true) {
		type = Type::command;
	}

	if (type == Type::unknown && size < 5) {
		type = Type::incomplete_payload;
	}

	iType = type;
	return RES_OK;
}

} // namespace tc::server::tcp
