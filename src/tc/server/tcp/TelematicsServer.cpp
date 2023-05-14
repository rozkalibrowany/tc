#include <tc/server/tcp/TelematicsServer.h>
#include <tc/parser/teltonika/Command.h>
#include <tc/iot/Devices.h>
#include <tc/server/http/Request.h>
#include <magic_enum.hpp>

namespace tc::server::tcp {

using namespace magic_enum;
using namespace parser;

TelematicsServer::TelematicsServer(const std::shared_ptr<AsioService> &service, std::shared_ptr<mongo::Client> client, size_t cache)
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
 , iCacheSize(cache)
 , iDbClient(client)
{
	SetupReuseAddress(true);
	SetupReusePort(true);
	SetupKeepAlive(false);
	SetupNoDelay(true);
}

TelematicsServer::~TelematicsServer()
{
	this->DisconnectAll();
	this->Stop();
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

	try {
		auto command = std::make_shared< teltonika::Command >((uchar*) buffer, size);
		return sendCommand(command);
	} catch (const std::invalid_argument& e) {
		LG_ERR(this->logger(), "Unable to create command");
	}

	return RES_INVARG;
}

result_t TelematicsServer::handleRequest(const internal::Request &request, const CppCommon::UUID id)
{
	using namespace parser;

	result_t res = RES_OK;
	auto type = request.type();

	Json::Value list;
	if (type == internal::Request::eDevices) {
		auto &el = list["devices"] = Json::arrayValue;
		for (const auto &[key, value] : _sessions) {
			if (key == id) continue;
			const auto session = dynamic_pointer_cast<TelematicsSession>(value);
			if (session == nullptr) continue;
			Json::Value val;
			session->toJson(val);
			el.append(val);
		}
	} else if (type == internal::Request::ePackets) {

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

result_t TelematicsServer::sendCommand(std::shared_ptr<CommandI> command)
{
	for (const auto &[key, value] : _sessions) {
		const auto &session = dynamic_pointer_cast<TelematicsSession>(value);
		if (session->imei() == command->imei()) {
			LG_NFO(this->logger(), "Sending command to session[{}] with imei[{}].", this->id(), command->imei());
			session->send(command->buf().cdata(), command->size());
		}
	}
	return RES_OK;
}

std::shared_ptr<mongo::Client> TelematicsServer::dbClient()
{
	return iDbClient;
}

std::shared_ptr< CppServer::Asio::TCPSession > TelematicsServer::CreateSession(const std::shared_ptr< TCPServer > &server)
{
	return std::make_shared< TelematicsSession >(std::dynamic_pointer_cast<TelematicsServer>(server), iCacheSize);
}

} // namespace tc::server::tcp