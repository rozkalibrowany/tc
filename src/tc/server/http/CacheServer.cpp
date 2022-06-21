#include <tc/server/http/CacheServer.h>
#include <tc/server/http/CacheSession.h>
#include <tc/server/http/Cache.h>

namespace tc::server::http {

void HTTPSCacheServer::setCache(const std::shared_ptr< Cache > &cache)
{
	iCache = std::move(cache);
}

std::shared_ptr<CppServer::Asio::SSLSession> HTTPSCacheServer::CreateSession(const std::shared_ptr<CppServer::Asio::SSLServer>& server)
{
	auto session = std::make_shared<HTTPSCacheSession>(std::dynamic_pointer_cast<CppServer::HTTP::HTTPSServer>(server));
	session->setCache(std::move(iCache));
	return session;
}

void HTTPSCacheServer::onError(int error, const std::string& category, const std::string& message)
{
	// cout << "HTTPS server caught an error with code " << error << " and category '" << category << "': " << message << std::endl;
}

} // tc::server::http {