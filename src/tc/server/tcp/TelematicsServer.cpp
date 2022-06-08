#include <tc/server/tcp/TelematicsServer.h>

namespace tc::server::tcp {

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

result_t TelematicsServer::get(const CppCommon::UUID uuid, Imei &imei)
{
	auto it = iActiveSessions.find(uuid);
	if (it == iActiveSessions.end()) {
		return RES_NOENT;
	}
	imei = iActiveSessions.at(uuid);
	return RES_OK;
}

result_t TelematicsServer::add(const Imei &imei)
{
	return iDevices.add(imei);
}

result_t TelematicsServer::add(const Imei &imei, const std::shared_ptr< parser::PacketPayload > &packet)
{
	return iDevices.add(imei, std::move(packet));
}

result_t TelematicsServer::add(const CppCommon::UUID uuid, const Imei &imei)
{
	auto isActive = has(uuid);

	if (isActive == true) {
		return RES_NOENT;
	}

	iActiveSessions.insert(std::make_pair(uuid, imei));
	if (iDevices.add(imei) != RES_OK) {
		LG_ERR(this->logger(), "Unable to add new device {}", imei);
		return RES_NOENT;
	}

	return RES_OK;
}

bool TelematicsServer::has(const Imei &imei)
{
	for (auto& it : iActiveSessions) {
		if (it.second.compare(imei) == 0) {
			return true;
		}
	}

	return false;
}

bool TelematicsServer::has(const CppCommon::UUID &uuid)
{
	return iActiveSessions.find(uuid) != iActiveSessions.end();
}

result_t TelematicsServer::rm(const CppCommon::UUID uuid)
{
	auto pos = iActiveSessions.find(uuid);
	if (pos != iActiveSessions.end()) {
		iActiveSessions.erase(pos);
		return RES_OK;
	}

	return RES_NOENT;
}

result_t TelematicsServer::sendCommand(const Imei &imei, std::shared_ptr<parser::PacketCommand> &command)
{
	for (const auto& [key, value] : iActiveSessions) {
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