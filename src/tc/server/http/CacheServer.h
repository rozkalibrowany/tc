#ifndef A0C705C8_1DB9_4170_908B_4E2D88AD03BF
#define A0C705C8_1DB9_4170_908B_4E2D88AD03BF

#include <server/http/https_server.h>

namespace tc::server::http {

class HTTPSCacheServer : public CppServer::HTTP::HTTPSServer
{
public:
    using CppServer::HTTP::HTTPSServer::HTTPSServer;

protected:
	std::shared_ptr<CppServer::Asio::SSLSession> CreateSession(const std::shared_ptr<CppServer::Asio::SSLServer> &server) override;

protected:
	void onError(int error, const std::string &category, const std::string &message) override;
};

} // namespace tc::server::http

#endif /* A0C705C8_1DB9_4170_908B_4E2D88AD03BF */
