#include <tc/server/tcp/TelematicsServer.h>

namespace tc::server::tcp {

int TelematicsServer::sessionsSize()
{
//	LG_NFO(this->logger(), "________PACKET_MAP____________");
	for (auto &s : iPayloadPackets) {
		LG_NFO(this->logger(), "packet.imei: {}  packetSet.size: {}", s.first, (int) s.second.size());
	}
//	LG_NFO(this->logger(), "____________________________\n");

	return _sessions.size();
}


TelematicsServer::PayloadPackets &TelematicsServer::payloadPackets()
{
	return iPayloadPackets;
}

std::shared_ptr< CppServer::Asio::TCPSession > TelematicsServer::CreateSession(const std::shared_ptr< TCPServer > &server)
{
	return std::make_shared< TelematicsSession >(server);
}

void TelematicsServer::onConnected(std::shared_ptr< CppServer::Asio::TCPSession > &session)
{
	LG_NFO(this->logger(), "TCP session connected, UUID: {} session: {}", session->id().string(), fmt::ptr(session.get()));
}

void TelematicsServer::onDisconnected(std::shared_ptr<CppServer::Asio::TCPSession> &session)
{
	LG_NFO(this->logger(), "TCP session disconnected, ID: {} session {}", session->id().string(), fmt::ptr(session.get()));
}

void TelematicsServer::onError(int error, const std::string& category, const std::string& message)
{
	LG_ERR(this->logger(), "Telematics Server caught an error[{}][{}]: {}", error, category, message);
}

result_t TelematicsServer::get(const Imei &imei, parser::PacketPayload &packet)
{
	auto it = iPayloadPackets.find(imei);
	if (it == iPayloadPackets.end()) {
		return RES_NOENT;
	}
	packet = *iPayloadPackets.at(imei).back();
	return RES_OK;
}

result_t TelematicsServer::add(const Imei imei, const std::shared_ptr< parser::PacketPayload > &packet)
{
	if (iPayloadPackets.find(imei) != iPayloadPackets.end()) {
		auto &packets = iPayloadPackets.at(imei);
		packets.emplace_back(std::move(packet));
	} else {
		Packets packets;
		packets.emplace_back(std::move(packet));
		iPayloadPackets.insert(std::make_pair(imei, std::move(packets)));
	}

	return RES_OK;
}

bool TelematicsServer::has(const Imei &imei)
{
	return iPayloadPackets.find(imei) != iPayloadPackets.end();
}

} // namespace tc::server::tcp