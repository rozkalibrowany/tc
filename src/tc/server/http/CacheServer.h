#ifndef A0C705C8_1DB9_4170_908B_4E2D88AD03BF
#define A0C705C8_1DB9_4170_908B_4E2D88AD03BF

#include <server/http/http_server.h>
#include <tc/server/http/Cache.h>
#include <tc/asio/AsioService.h>
#include <tc/db/Client.h>

namespace tc::server::http {

using namespace db::mongo;
using namespace asio;
class HTTPCacheServer : public CppServer::HTTP::HTTPServer, public tc::LogI
{
public:
	using CppServer::HTTP::HTTPServer::HTTPServer;

	HTTPCacheServer(const std::shared_ptr<AsioService> &service, const std::shared_ptr<Client> &client, const std::shared_ptr<Cache> &cache, int port);
	HTTPCacheServer(const std::shared_ptr<AsioService> &service, const std::shared_ptr<Client> &client, const std::shared_ptr<Cache> &cache);

	result_t syncDevices(bool sync);

protected:
	std::shared_ptr<CppServer::Asio::TCPSession> CreateSession(const std::shared_ptr<CppServer::Asio::TCPServer> &server) override;
	void onError(int error, const std::string &category, const std::string &message) override;

private:
	std::shared_ptr<Cache> iCache{nullptr};
	std::shared_ptr< Client > iDbClient;
};

} // namespace tc::server::http

#endif /* A0C705C8_1DB9_4170_908B_4E2D88AD03BF */
