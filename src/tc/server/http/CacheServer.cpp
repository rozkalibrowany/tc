#include <tc/server/http/CacheServer.h>
#include <tc/server/http/CacheSession.h>
#include <tc/server/http/Cache.h>

namespace tc::server::http {

HTTPCacheServer::HTTPCacheServer(const std::shared_ptr<AsioService> &service, const std::shared_ptr<Client> &client, const std::shared_ptr<Cache> &cache)
 : HTTPCacheServer(service, client, cache, 8443)
{
	// nothing to do
}

HTTPCacheServer::HTTPCacheServer(const std::shared_ptr<AsioService> &service, const std::shared_ptr<Client> &client, const std::shared_ptr<Cache> &cache, int port)
 : CppServer::HTTP::HTTPServer(service, port)
 , iCache(cache)
 , iDbClient(std::move(client))
{
	// nothing to do
}

result_t HTTPCacheServer::syncDevices(bool sync)
{
	LG_NFO(this->logger(), "syncDevices");
	auto imeis = iCache->devices().imeis();

	for (auto &i : imeis) {
		bsoncxx::document::view doc;
		if (iDbClient->get(i, doc) != RES_OK) {
			LG_NFO(this->logger(), "Unable to get device info from database.");
			Json::Value val;
			auto it = iCache->devices().devices().find(i);
			if (it != iCache->devices().devices().end()) {
				it->second->toJson(val);
				iDbClient->insert(val.toStyledString());
			}
		}
		// TODO ADD MODIFICATION TIMESTAMP
		auto it = iCache->devices().devices().find(i);
		if (it != iCache->devices().devices().end()) {
			auto res = it->second->updateDeviceInfo(doc);
		}
	}
	return RES_OK;
}

std::shared_ptr<CppServer::Asio::TCPSession> HTTPCacheServer::CreateSession(const std::shared_ptr<CppServer::Asio::TCPServer>& server)
{
	auto session = std::make_shared<HTTPCacheSession>(std::dynamic_pointer_cast<CppServer::HTTP::HTTPServer>(server), iCache);
	return session;
}

void HTTPCacheServer::onError(int error, const std::string& category, const std::string& message)
{
	LG_ERR(this->logger(), "HTTPS server caught an error with code: {}, category: {}, msg: {}", error, category, message);
}

} // tc::server::http