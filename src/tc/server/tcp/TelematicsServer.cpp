#include <tc/server/tcp/TelematicsServer.h>
#include <tc/server/iot/Devices.h>
#include <tc/server/http/Request.h>

namespace tc::server::tcp {

TelematicsServer::TelematicsServer(const std::shared_ptr<AsioService>& service, std::shared_ptr<db::Client>& client, size_t cache)
 : TelematicsServer(service, client, cache, 8881)
{
	// nothing to do
}

TelematicsServer::TelematicsServer(const std::shared_ptr<AsioService>& service, std::shared_ptr<db::Client>& client, size_t cache, int port)
 : TelematicsServer(service, client, cache, port, "127.0.0.1")
{
	// nothing to do
}

TelematicsServer::TelematicsServer(const std::shared_ptr<AsioService>& service, std::shared_ptr<db::Client>& client, size_t cache, int port, const std::string& address)
 : CppServer::Asio::TCPServer(service, address, port)
 , iDbClient(std::move(client))
 , iCacheSize(cache)
{
	// nothing to do
}

TelematicsServer::~TelematicsServer()
{
	// nothing to do
}

const std::shared_ptr<db::Client> TelematicsServer::dbClient()
{
	return iDbClient;
}

size_t TelematicsServer::cacheSize() const
{
	return iCacheSize;
}

std::shared_ptr< CppServer::Asio::TCPSession > TelematicsServer::CreateSession(const std::shared_ptr< TCPServer > &server)
{
	return std::make_shared< TelematicsSession >(server);
}

void TelematicsServer::onConnected(std::shared_ptr< CppServer::Asio::TCPSession > &session)
{
	LG_NFO(this->logger(), "TCP session connected, UUID: {}", session->id().string());
}

void TelematicsServer::onDisconnected(std::shared_ptr<CppServer::Asio::TCPSession> &session)
{
	LG_NFO(this->logger(), "TCP session disconnected, UUID: {}", session->id().string());
}

void TelematicsServer::onError(int error, const std::string& category, const std::string& message)
{
	LG_ERR(this->logger(), "Telematics Server caught an error[{}][{}]: {}", error, category, message);
}

result_t TelematicsServer::handleCommand(const uchar *buffer, size_t size)
{
	LG_NFO(this->logger(), "Handle command[{}]", size);

	Imei imei;

	result_t res = parser::Packet::parseImei(buffer, size, imei);
	if (res != RES_OK) {
		LG_ERR(this->logger(), "Parse imei.");
		return res;
	}

	auto packetCommand = std::make_shared< parser::PacketCommand >();
	res = packetCommand->parse((uchar*) buffer, size, imei.length());
	if (res != RES_OK) {
		LG_ERR(this->logger(), "Parse command.");
		return res;
	}

	return sendCommand(imei, packetCommand);
}

result_t TelematicsServer::handleRequest(const uchar *buffer, size_t size, const CppCommon::UUID id)
{
	LG_NFO(this->logger(), "Handle request[{}] id[{}]", size, id.string());

	auto request = std::make_shared< parser::PacketRequest >();
	result_t res = request->parse((uchar*) buffer, size);
	if (res != RES_OK) {
		LG_ERR(this->logger(), "Parse request.");
		return res;
	}

	return dispatchRequest(request, id);
}

result_t TelematicsServer::dispatchRequest(std::shared_ptr< parser::PacketRequest > &request, const CppCommon::UUID id)
{
	using namespace parser;

	result_t res = RES_OK;
	auto type = request->iType;
	auto method = request->iMethod;

	if (type == Packet::eDevices && method == Packet::eGet) {
		Json::Value list;
		auto &el = list["devices"] = Json::arrayValue;
		for (const auto &[key, value] : _sessions) {
			if (key == id) continue;
			const auto &session = dynamic_pointer_cast<TelematicsSession>(value);
			Json::Value val;
			session->toJson(val);
			el.append(val);
		}
		auto hexJson = tc::tohex(list.toStyledString());
		const auto &session = dynamic_pointer_cast<TelematicsSession>(_sessions.at(id));
		if ((res = session->send((const uchar *)hexJson.data(), hexJson.size())) != RES_OK) {
			LG_ERR(this->logger(), "Send hex json");
		}
	}

	return res;
}

result_t TelematicsServer::sendCommand(const Imei &imei, std::shared_ptr<parser::PacketCommand> &command)
{
	for (const auto &[key, value] : _sessions) {
		const auto &session = dynamic_pointer_cast<TelematicsSession>(value);
		if (session->hasImei(imei)) {
			auto elem = _sessions.find(key);
			if (elem != _sessions.end()) {
				session->send(command->command(), command->size());
			}
		}
	}
	return RES_OK;
}

} // namespace tc::server::tcp