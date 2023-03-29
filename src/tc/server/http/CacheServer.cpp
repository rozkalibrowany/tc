#include <tc/server/http/CacheServer.h>
#include <tc/server/http/CacheSession.h>

namespace tc::server::http {

HTTPCacheServer::HTTPCacheServer(const std::shared_ptr<AsioService> service, std::shared_ptr<db::mongo::Client> client, std::shared_ptr<CacheHandler> &cache)
 : HTTPCacheServer(service, client, cache, 8443)
{
	// nothing to do
}

HTTPCacheServer::HTTPCacheServer(const std::shared_ptr<AsioService> service, std::shared_ptr<db::mongo::Client> client, std::shared_ptr<CacheHandler> &cache, int port)
 : CppServer::HTTP::HTTPServer(service, port)
 , iCache(cache)
 , iDbClient(client)
{
	this->SetupReusePort(true);
	this->SetupReuseAddress(true);
	this->SetupNoDelay(true);
}

/**
 * It iterates over all the documents in the database and adds them to the cache
 * @return A result code.
 */
result_t HTTPCacheServer::syncDevices()
{
	using namespace iot;

	if(!iDbClient->enabled()) {
		return RES_NOENT;
	}

	auto cursor = iDbClient->getCursor();
	if (!cursor || ((*cursor).begin() == (*cursor).end())) {
		return RES_NOENT;
	}

	auto &vehicles = iCache->vehicles();
	for(auto doc : *cursor) {
		Json::Value root;
		auto json_doc = bsoncxx::to_json(doc);
		if (Vehicle::fromJsonString(json_doc, root) != RES_OK) {
			continue;
		}

		auto vehicle = std::make_shared<iot::Vehicle>(std::string{"unknown"}, 100, Vehicle::eDatabase);
		if (vehicle->fromJson(root) != RES_OK) {
			return RES_ERROR;
		}
		if (vehicles.add(std::move(vehicle)) != RES_OK) {
			LG_WRN(this->logger(), "Unable to add vehicle.");
			return RES_ERROR;
		}
	}

	return RES_OK;
}

result_t HTTPCacheServer::onModified(const Imei &imei)
{
	if(imei.empty() || !imei.compare("unknown")) {
		return RES_NOENT;
	}

	std::string json_doc;
	if (iDbClient->get(imei, json_doc) != RES_OK) {
		return RES_NOENT;
	}

	auto &vehicles = iCache->vehicles();
	auto it = vehicles.find(imei);
	if(it == vehicles.end()) {
		return RES_NOENT;
	}

	{
		std::scoped_lock lock(iMutex);
		if(iDbClient->replace(json_doc, it->second->toJsonValue(it->second->online()).toStyledString()) != RES_OK) {
			return RES_NOENT;
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