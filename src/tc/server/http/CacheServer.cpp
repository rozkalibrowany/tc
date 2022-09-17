#include <tc/server/http/CacheServer.h>
#include <tc/server/http/CacheSession.h>
#include <tc/server/http/Cache.h>

namespace tc::server::http {

void HTTPCacheServer::setCache(const std::shared_ptr< Cache > &cache)
{
	iCache = std::move(cache);
}

std::shared_ptr<CppServer::Asio::TCPSession> HTTPCacheServer::CreateSession(const std::shared_ptr<CppServer::Asio::TCPServer>& server)
{
	auto session = std::make_shared<HTTPCacheSession>(std::dynamic_pointer_cast<CppServer::HTTP::HTTPServer>(server));
	session->setCache(std::move(iCache));
	return session;
}

void HTTPCacheServer::onError(int error, const std::string& category, const std::string& message)
{
	// cout << "HTTPS server caught an error with code " << error << " and category '" << category << "': " << message << std::endl;
}

} // tc::server::http {