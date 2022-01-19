#include <tc/server/tcp/server/Server.h>
#include <tc/parser/Packet.h>
#include <tc/common/SysTime.h>
#include <tuple>

namespace tc::server::tcp {

Server::Server(const std::shared_ptr<Service>& service, int port)
 : CppServer::Asio::TCPServer(service, port, CppServer::Asio::InternetProtocol::IPv4)
 , common::LogI("console")
{

}
int Server::sessionsSize()
{
	SPDLOG_LOGGER_INFO(this->logger(), "________SESSIONS____________");
	for (auto &s : iSessionPacketMap) {
		SPDLOG_LOGGER_INFO(this->logger(), "uuid: {}   packet.imei: {}  packet.size: {}", s.first.string(), s.second.imei(), (int) s.second.size());
	}
	SPDLOG_LOGGER_INFO(this->logger(), "____________________________\n");

	SPDLOG_LOGGER_INFO(this->logger(), "________PACKET_MAP____________");
	for (auto &s : iPacketMap) {
		SPDLOG_LOGGER_INFO(this->logger(), "packet.imei: {}  packetSet.size: {}", s.first, (int) s.second.size());
	}
	SPDLOG_LOGGER_INFO(this->logger(), "____________________________\n");

	return _sessions.size();
}

result_t Server::get(parser::Packet &packet, const UUID uuid)
{
	auto it = iSessionPacketMap.find(uuid);
	if (it != iSessionPacketMap.end()) {
		packet = iSessionPacketMap.at(uuid);
		return RES_OK;
	}

	return RES_NOENT;
}

result_t Server::add(const PacketImei &imei, parser::Packet &packet)
{
	auto it = iPacketMap.find(imei);
	if (it != iPacketMap.end()) {
		iPacketMap.at(imei).insert(packet);
		return RES_OK;
	}

	PacketSet set;
	iPacketMap.insert(std::make_pair(imei, std::move(set)));
	iPacketMap.at(imei).insert(packet);

	return RES_OK;
}

/*result_t Server::add(const Packet::Imei &imei, const UUID uuid)
{
	if (iImeiMap.contains(uuid) == true) {
		return RES_INVARG;
	}

	iImeiMap.insert(std::make_pair(uuid, imei));
	return RES_OK;
}

result_t Server::del(const UUID uuid)
{
	if (iImeiMap.contains(uuid) != true) {
		return RES_INVARG;
	}

	auto it = iImeiMap.find(uuid);
	if (it != iImeiMap.end()) {
		iImeiMap.erase(it);
		return RES_OK;
	}

	return RES_NOENT;
}

result_t Server::add(parser::PacketUPtr packet)
{
	if (packet == nullptr) {
		return RES_INVARG;
	}

	auto it = iPacketMap.find(packet->imei());
	if (it != iPacketMap.end()) {
		(*it).second.insert(std::move((packet)));
		return RES_OK;
	}

	PacketUPtrSet set;
	set.insert(std::move(packet));
	iPacketMap.insert(std::make_pair(packet->imei(), std::move(set)));

	return RES_OK;
}

result_t Server::del(parser::PacketUPtr packet)
{
	if (packet == nullptr) {
		return RES_INVARG;
	}

	auto it = iPacketMap.find(packet->imei());
	if (it != iPacketMap.end()) {
		iPacketMap.erase(it);
		return RES_OK;
	}
	return RES_NOENT;
}*/

Server::SessionPacketMap &Server::sessionPackets()
{
	return iSessionPacketMap;
}

std::shared_ptr<TCPSession> Server::CreateSession(const std::shared_ptr<TCPServer> &server)
{
	return std::make_shared<Session>(server);
}

void Server::onConnected(std::shared_ptr< Session > &session)
{
	auto it = iSessionPacketMap.find(session->id());
	if (it != iSessionPacketMap.end()) {
		session->Disconnect();
		return;
	}

	parser::Packet packet(true);
	iSessionPacketMap.insert(std::make_pair(session->id(), std::move(packet)));
	SPDLOG_LOGGER_INFO(this->logger(), "TCP session connected, ID: {}", session->ids());

	for (auto &s : this->_sessions) {
		auto _session = std::dynamic_pointer_cast<Session>(s.second);
		auto its = iSessionPacketMap.find(_session->id());
		if (its == iSessionPacketMap.end()) {
			continue;
		}
		auto timeout = SysTime(true) - _session->iTimestamp;
		if (timeout > 15000 && (iSessionPacketMap.at(_session->id()).size() == 0 || iSessionPacketMap.at(_session->id()).imei().empty())) {
			_session->Disconnect();
		}
	}
}

void Server::onConnected(std::shared_ptr<TCPSession> &session)
{
	auto s = std::dynamic_pointer_cast<Session>(session);
	return onConnected(s);
}

void Server::onDisconnected(std::shared_ptr< Session > &session)
{
	if (session == nullptr) {
		return;
	}

	auto it = iSessionPacketMap.find(session->id());
	if (it != iSessionPacketMap.end()) {
		iSessionPacketMap.erase(session->id());
		session->Disconnect();
	}
	SPDLOG_LOGGER_INFO(this->logger(), "TCP session disconnected, ID: {}", session->ids());
}

void Server::onDisconnected(std::shared_ptr<TCPSession> &session)
{
	auto s = std::dynamic_pointer_cast<Session>(session);
	return onDisconnected(s);
}

void Server::onError(int error, const std::string& category, const std::string& message)
{
	SPDLOG_LOGGER_ERROR(this->logger(), "Server caught an error[{}][{}]: {}", error, category, message);
}

} // namespace tc::server::tcp