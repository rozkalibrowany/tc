#include <tc/server/http/CacheServer.h>
#include <tc/server/http/CacheSession.h>
#include <tc/server/http/Cache.h>

namespace tc::server::http {

HTTPCacheServer::HTTPCacheServer(const std::shared_ptr<asio::AsioService>& service, int port, const std::shared_ptr< Cache > &cache)
 : CppServer::HTTP::HTTPServer(service, port)
 , iCache(cache)
{
	// nothing to do
}

std::shared_ptr<CppServer::Asio::TCPSession> HTTPCacheServer::CreateSession(const std::shared_ptr<CppServer::Asio::TCPServer>& server)
{
	auto session = std::make_shared<HTTPCacheSession>(std::dynamic_pointer_cast<CppServer::HTTP::HTTPServer>(server), iCache);
	return session;
}

void HTTPCacheServer::onError(int error, const std::string& category, const std::string& message)
{
	//LG_ERR(this->logger(), "HTTPS server caught an error with code: {}, cat: {}, msg: {}", error, category, message)
}

} // tc::server::http {