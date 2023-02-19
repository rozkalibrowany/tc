#ifndef B2140A82_BB7E_4CED_995B_D37725E345E8
#define B2140A82_BB7E_4CED_995B_D37725E345E8

#include <server/http/http_server.h>
#include <tc/server/http/CacheHandler.h>
#include <tc/asio/AsioService.h>
#include <tc/db/Client.h>

namespace tc::server::http {

using namespace db;
using namespace asio;
class HTTPCacheServer : public CppServer::HTTP::HTTPServer, public tc::LogI
{

public:
	using CppServer::HTTP::HTTPServer::HTTPServer;

	HTTPCacheServer(const std::shared_ptr<AsioService> service, std::shared_ptr<db::mongo::Client> client, std::shared_ptr<CacheHandler> &cache, int port);
	HTTPCacheServer(const std::shared_ptr<AsioService> service, std::shared_ptr<db::mongo::Client> client, std::shared_ptr<CacheHandler> &cache);

	result_t syncDevices();
	result_t onModified(const Imei &imei);

protected:
	std::shared_ptr<CppServer::Asio::TCPSession> CreateSession(const std::shared_ptr<CppServer::Asio::TCPServer> &server) override;
	void onError(int error, const std::string &category, const std::string &message) override;

private:
	result_t getDeviceData();

	std::shared_ptr<CacheHandler> iCache{nullptr};
	std::shared_ptr<db::mongo::Client> iDbClient;
	std::mutex iMutex;
};

} // namespace tc::server::http

#endif /* B2140A82_BB7E_4CED_995B_D37725E345E8 */
