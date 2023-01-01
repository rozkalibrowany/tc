#ifndef A0C705C8_1DB9_4170_908B_4E2D88AD03BF
#define A0C705C8_1DB9_4170_908B_4E2D88AD03BF

#include <server/http/http_server.h>
#include <tc/server/http/Cache.h>
#include <tc/asio/AsioService.h>

namespace tc::server::http {

class HTTPCacheServer : public CppServer::HTTP::HTTPServer
{
public:
	using CppServer::HTTP::HTTPServer::HTTPServer;

	HTTPCacheServer(const std::shared_ptr<asio::AsioService>& service, int port, const std::shared_ptr<Cache> &cache);

protected:
	std::shared_ptr<CppServer::Asio::TCPSession> CreateSession(const std::shared_ptr<CppServer::Asio::TCPServer>& server) override;
	void onError(int error, const std::string &category, const std::string &message) override;

private:
	std::shared_ptr< Cache > iCache {nullptr};
};

} // namespace tc::server::http

#endif /* A0C705C8_1DB9_4170_908B_4E2D88AD03BF */
