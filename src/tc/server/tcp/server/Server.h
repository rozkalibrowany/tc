#ifndef D1A0B0F8_F8ED_4A8E_AB96_417C353E7EA7
#define D1A0B0F8_F8ED_4A8E_AB96_417C353E7EA7

#include <tc/server/tcp/session/Session.h>
#include <tc/parser/Packet.h>
#include <server/asio/tcp_server.h>
#include <tc/common/Logger.h>

namespace tc::server::tcp {

using namespace CppCommon;
using namespace parser;

class Server : public CppServer::Asio::TCPServer, public common::LogI
{

public:
	using CppServer::Asio::TCPServer::TCPServer;
	using SessionImeiMap = std::map<UUID, Packet::Imei >;

	Server(const std::shared_ptr<Service>& service, int port);

	//using DevicePackets
	virtual ~Server() = default;

	virtual void printSessions();
	//virtual result_t insertImei(const SessionSPtr &session);
	//virtual result_t deleteImei(const SessionSPtr &session);

protected:
	virtual std::shared_ptr<TCPSession> CreateSession(const std::shared_ptr<TCPServer> &server);
	void onError(int error, const std::string &category, const std::string &message) override;

private:
	SessionImeiMap iImeiMap;
};

} // namespace tc::server::tcp

#endif /* D1A0B0F8_F8ED_4A8E_AB96_417C353E7EA7 */
