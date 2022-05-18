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

	rm(session->id());
}

void TelematicsServer::onError(int error, const std::string& category, const std::string& message)
{
	LG_ERR(this->logger(), "Telematics Server caught an error[{}][{}]: {}", error, category, message);
}

result_t TelematicsServer::get(const CppCommon::UUID uuid, Action::Imei &imei)
{
	auto it = iVerifiedSessions.find(uuid);
	if (it == iVerifiedSessions.end()) {
		return RES_NOENT;
	}
	imei = iVerifiedSessions.at(uuid);
	return RES_OK;
}

result_t TelematicsServer::get(const Action::Imei &imei, parser::PacketPayload &packet)
{
	auto it = iPayloadPackets.find(imei);
	if (it == iPayloadPackets.end()) {
		return RES_NOENT;
	}
	packet = *iPayloadPackets.at(imei).back();
	return RES_OK;
}

result_t TelematicsServer::add(const Action::Imei &imei, const std::shared_ptr< parser::PacketPayload > &packet)
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

result_t TelematicsServer::add(const CppCommon::UUID uuid, const Action::Imei &imei)
{
	if (iVerifiedSessions.find(uuid) != iVerifiedSessions.end()) {
		return RES_INVARG;
	}

	iVerifiedSessions.insert(std::make_pair(uuid, imei));
	return RES_OK;
}

bool TelematicsServer::has(const CppCommon::UUID &uuid)
{
	return iVerifiedSessions.find(uuid) != iVerifiedSessions.end();
}


bool TelematicsServer::has(const Action::Imei &imei)
{
	return iPayloadPackets.find(imei) != iPayloadPackets.end();
}

result_t TelematicsServer::rm(const CppCommon::UUID uuid)
{
	auto pos = iVerifiedSessions.find(uuid);
	if (pos != iVerifiedSessions.end()) {
		iVerifiedSessions.erase(pos);
		return RES_OK;
	}

	return RES_NOENT;
}

result_t TelematicsServer::sendCommand(const Action::Imei &imei, std::shared_ptr<parser::PacketCommand> &command)
{
	for (const auto& [key, value] : iVerifiedSessions) {
		if (value == imei) {
			auto elem = _sessions.find(key);
			if (elem != _sessions.end()) {
				auto session = dynamic_pointer_cast<TelematicsSession>(elem->second);
				session->send(command->command(), command->size());
			}
		}
	}
	return RES_OK;
}

} // namespace tc::server::tcp