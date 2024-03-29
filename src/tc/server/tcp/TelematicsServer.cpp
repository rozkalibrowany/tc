#include <tc/server/tcp/TelematicsServer.h>
#include <tc/server/iot/Devices.h>
#include <tc/server/http/Request.h>

namespace tc::server::tcp {

TelematicsServer::TelematicsServer(const std::shared_ptr<AsioService>& service, std::shared_ptr<mongo::Client> client, size_t cache)
 : TelematicsServer(service, client, cache, 8881)
{
	// nothing to do
}

TelematicsServer::TelematicsServer(const std::shared_ptr<AsioService>& service, std::shared_ptr<mongo::Client> client, size_t cache, int port)
 : TelematicsServer(service, client, cache, port, "127.0.0.1")
{
	// nothing to do
}

TelematicsServer::TelematicsServer(const std::shared_ptr<AsioService>& service, std::shared_ptr<mongo::Client> client, size_t cache, int port, const std::string& address)
 : CppServer::Asio::TCPServer(service, address, port)
 , iDbClient(client)
 , iCacheSize(cache)
{
	this->SetupReusePort(true);
	this->SetupReuseAddress(true);
}

TelematicsServer::~TelematicsServer()
{
	this->DisconnectAll();
	this->Stop();
}

std::shared_ptr<mongo::Client> TelematicsServer::dbClient()
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
	session->SetupReceiveBufferLimit(3000);
}

void TelematicsServer::onDisconnected(std::shared_ptr<CppServer::Asio::TCPSession> &session)
{
	LG_NFO(this->logger(), "TCP session disconnected, UUID: {}", session->id().string());
}

void TelematicsServer::onError(int error, const std::string& category, const std::string& message)
{
	LG_ERR(this->logger(), "Server caught an error[{}][{}]: {}", error, category, message);
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
	auto request = std::make_shared< parser::PacketRequest >();
	result_t res = request->parse((uchar*) buffer, size);
	if (res != RES_OK) {
		LG_ERR(this->logger(), "Parse request.");
		return res;
	}

	LG_NFO(this->logger(), "Handle request[{}] Method: {} Type: {}", size, Packet::method2string(request->iMethod),
		Packet::type2string(request->iType));

	return dispatchRequest(request, id);
}

result_t TelematicsServer::dispatchRequest(std::shared_ptr< parser::PacketRequest > request, const CppCommon::UUID id)
{
	using namespace parser;

	result_t res = RES_OK;
	auto type = request->iType;

	Json::Value list;
	if (type == Packet::eDevices) {
		auto &el = list["devices"] = Json::arrayValue;
		for (const auto &[key, value] : _sessions) {
			if (key == id) continue;
			const auto session = dynamic_pointer_cast<TelematicsSession>(value);
			if (session == nullptr) continue;
			Json::Value val;
			session->toJson(val);
			el.append(val);
		}
	} else if (type == Packet::ePackets) {

		auto &el = list["packets"] = Json::arrayValue;
		for (const auto &[key, value] : _sessions) {
			if (key == id) continue;
			auto session = dynamic_pointer_cast<TelematicsSession>(value);
			if (session == nullptr) continue;
			Json::Value val;
			if (session->lastPacketJson(val) != RES_OK)
				continue;
			el.append(val);
		}
	}

	auto hexJson = tc::tohex(list.toStyledString());
	const auto &session = dynamic_pointer_cast<TelematicsSession>(_sessions.at(id));
	if ((res = session->send((const uchar *) hexJson.data(), hexJson.size())) != RES_OK) {
		LG_ERR(this->logger(), "Error while processing internal response.");
	}

	return res;
}

result_t TelematicsServer::sendCommand(const Imei &imei, std::shared_ptr<parser::PacketCommand> &command)
{
	for (const auto &[key, value] : _sessions) {
		const auto &session = dynamic_pointer_cast<TelematicsSession>(value);
		if (session->imei() == imei) {
			LG_NFO(this->logger(), "Sending command to session[{}] with imei[{}].", this->id(), imei);
			session->send(command->command(), command->size());
		}
	}
	return RES_OK;
}

} // namespace tc::server::tcp