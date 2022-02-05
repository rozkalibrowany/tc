#include <tc/server/tcp/Action.h>
#include <tc/parser/packet/PacketPayload.h>
#include <tc/parser/packet/PacketCommand.h>

namespace tc::server::tcp {

Action::Action(Type type)
 : iType(type)
{
	// nothing to do
}

const Action::Type &Action::type() const
{
	return iType;
}

result_t Action::parse(const uchar* buffer, size_t size)
{
	Type type = Type::unknown;

	if (PacketPayload::hasPayload(buffer, size) == true) {
		type = Type::payload;
	}

	if (PacketPayload::hasPayloadImei(buffer, size) == true) {
		type = Type::payload_imei;
	}

	if (PacketCommand::hasCommand(buffer, size) == true) {
		type = Type::command;
	}

	// nie wiem co to za zjebane pakiety 3 bajtowe, więc akceptuje
	if (type == Type::unknown && size == 3) {
		return RES_OK;
	}

	if (type == Type::unknown) {
		return RES_NOENT;
	}

	iType = type;
	return RES_OK;
}

} // namespace tc::server::tcp
