#include <tc/server/tcp/telematics/TelematicsSession.h>
#include <tc/server/tcp/telematics/TelematicsServer.h>
#include <tc/parser/packet/PacketCommand.h>
#include <cstring>

namespace tc::server::tcp {

TelematicsSession::TelematicsSession(const std::shared_ptr< TCPServer > &server)
 : TCPSession(server)
 , tc::LogI("console")
 , iTimestamp(SysTime(true))
 , iServer(std::move(std::dynamic_pointer_cast< TelematicsServer >(server)))
{
	// nothing to do
}

TelematicsSession::~TelematicsSession()
{
	// nothing to do
}

void TelematicsSession::onReceived(const void* buffer, size_t size)
{
	LG_NFO(this->logger(), "Telematics Session [{}] size[{}] got buffer[{}]", ids(),
		(int) size, fmt::ptr(buffer));

	LockGuard g(iMutex);

	Action action;
	if (action.parse((uchar*) buffer, size) != RES_OK) { // !!!!!!!!!!!!
		LG_ERR(this->logger(), "Unable to parse action");
		return;
	}

	if (handleDataBuffer(buffer, size, action) != RES_OK) {
		LG_ERR(this->logger(), "Unable to handle data buffer");
	}
}

result_t TelematicsSession::handleDataBuffer(const void *buffer, size_t size, Action &action)
{
	result_t res = RES_ERROR;

	if (action.type() == Action::payload || action.type() == Action::payload_imei) {
		return handlePayload(buffer, size, action);
	}

	if (action.type() == Action::command) {
		return handleCommand(buffer, size);
	}

	if (action.type() == Action::unknown) {
		return handleUnknown(buffer, size);
	}

	return res;
}

result_t TelematicsSession::handlePayload(const void *buffer, size_t size, Action &action)
{
	result_t res = RES_OK;
	int response = 0;

	/*if (action.type() == Action::payload_imei) {
		response = 1;
	}*/

	if (action.type() == Action::payload_imei) {
		LG_DBG(this->logger(), "Handle payload - imei. size {}", (int)size);
		auto packetPayload = std::make_shared< PacketPayload >(true);
		if ((res = packetPayload->parseImei((uchar *)buffer, size)) != RES_OK) {
			Send(static_cast<void *>(&response), sizeof(response));
			LG_ERR(this->logger(), "Parse imei.");
			return res;
		}
		iImei = packetPayload->imei();
		iServer->add(iImei, std::move(packetPayload));
		response = 1;
	}

	if (size >= 3 && size <= 5) {
		response = 0;
	}
		/*if (action.type() == Action::payload) {
			LG_DBG(this->logger(), "Handle payload - payload. size {}", (int)size);
			std::shared_ptr<PacketPayload> packet;
			if (iImei.empty() == true || (res = iServer->get(iImei, packet) != RES_OK)) {
				LG_ERR(this->logger(), "Failed getting packet (imei does not exists).");
				return res;
			}
			auto cbuffer = reinterpret_cast<uchar *>(const_cast<void *>(buffer));
			if ((res = packet->parse(cbuffer, size)) != RES_OK) {
				LG_ERR(this->logger(), "Parse payload packet: size[{}], data: {}", size, tc::unsigned_char_to_string((uchar*) buffer, size));
				return res;
			}
			response = packet->totalRecords();
		}*/

		SendAsync(static_cast<void *>(&response), sizeof(response));
		return res;
}

result_t TelematicsSession::handleCommand(const void *buffer, size_t size)
{
	result_t res = RES_INVARG;
	auto packetCommand = std::make_shared< PacketCommand >();

	res = packetCommand->parse((uchar *)buffer, size);

	return res;
}

result_t TelematicsSession::handleUnknown(const void *buffer, size_t size)
{
	int response = (int) size;
	Send(static_cast<void *>(&response), sizeof(response));

	return RES_OK;
}

std::string TelematicsSession::ids() const
{
	auto id_str = id().string();
	auto id_str_out = std::string("...-");
	id_str_out.append(id_str.substr(0, id_str.find("-")));
	return id_str_out;
}

/*void TelematicsSession::handleResponse(Action action, bool async)
{
	if (action == A_NO_ACTION) {
		return;
	}

	int response = (int) R_REFUSE;

	if (action == A_PARSE_DATA) {
		auto it = iServer->payloadSessions().find(id());
		if (it != iServer->payloadSessions().end()) {
			//auto &packet = iServer->sessionPackets().at(id());
			response = (int) it->second.size();
		}
	} else {
		response = (int) R_ACCEPT;
	}

	if (!async) {
		Send(static_cast<void *>(&response), sizeof(response));
		if (action == A_PARSE_DATA) {
			CppCommon::Thread::Sleep(200);
			iServer->sendCmd();
		}

		return;
	}

	SendAsync(static_cast<void *>(&response), sizeof(response));

	CppCommon::Thread::Sleep(200);
	iServer->sendCmd();

}*/

/* CRC od 0C do 01
0C 010500000007676574696E666F 01

length:

  od 18 (packet length)
	do 01
	"00000000000000140C01050000000773636C6F636B6374726C203001000019a4"
	00000000000000160C010500000010736574706172616D2032333920310100007B78 //setparam 239 1
*/

} // namespace tc::server::tcp