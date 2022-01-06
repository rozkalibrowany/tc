#include <tc/server/tcp/server/Server.h>
#include <tc/parser/Packet.h>
#include <spdlog/spdlog.h>

namespace tc::server::tcp {

std::shared_ptr<TCPSession> Server::CreateSession(const std::shared_ptr<TCPServer> &server)
{
	 return std::make_shared<Session>(server);
}

void Server::onError(int error, const std::string& category, const std::string& message)
{
	spdlog::critical("TCP server caught an error with code {} and category {}", error, category);

}

} // namespace tc::server::tcp {