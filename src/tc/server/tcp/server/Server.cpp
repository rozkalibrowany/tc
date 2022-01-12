#include <tc/server/tcp/server/Server.h>
#include <tc/parser/Packet.h>

namespace tc::server::tcp {

Server::Server(const std::shared_ptr<Service>& service, int port)
 : CppServer::Asio::TCPServer(service, port, CppServer::Asio::InternetProtocol::IPv4)
 , common::LogI("console")
{

}
 void Server::printSessions()
{
	for(auto &s : _sessions) {
		SPDLOG_LOGGER_INFO(this->logger(), "Session: {}", s.first.string());
	}
}

/*
result_t Server::insertSession(const SessionSPtr &session)
{

}

result_t Server::deleteSession(const SessionSPtr &session);
*/

std::shared_ptr<TCPSession> Server::CreateSession(const std::shared_ptr<TCPServer> &server)
{
	 return std::make_shared<Session>(server);
}

void Server::onError(int error, const std::string& category, const std::string& message)
{
	//spdlog::critical("TCP server caught an error with code {} and category {}", error, category);

}

} // namespace tc::server::tcp