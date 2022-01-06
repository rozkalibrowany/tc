#ifndef D1A0B0F8_F8ED_4A8E_AB96_417C353E7EA7
#define D1A0B0F8_F8ED_4A8E_AB96_417C353E7EA7

#include <tc/server/tcp/session/Session.h>
#include <server/asio/tcp_server.h>

namespace tc::server::tcp {

using namespace CppServer::Asio;

class Server : public TCPServer
{
public:
	using TCPServer::TCPServer;

protected:
	virtual std::shared_ptr<TCPSession> CreateSession(const std::shared_ptr<TCPServer> &server);

protected:
	void onError(int error, const std::string &category, const std::string &message) override;
};

} // namespace tc::server::tcp

#endif /* D1A0B0F8_F8ED_4A8E_AB96_417C353E7EA7 */
