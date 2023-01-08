#ifndef D1A0B0F8_F8ED_4A8E_AB96_417C353E7EA7
#define D1A0B0F8_F8ED_4A8E_AB96_417C353E7EA7

#include <server/asio/tcp_server.h>
#include <tc/server/tcp/TelematicsSession.h>
#include <tc/parser/packet/Command.h>
#include <tc/server/iot/Devices.h>
#include <tc/asio/AsioService.h>
#include <tc/db/Client.h>

namespace tc::server::tcp {

using namespace parser;
using namespace asio;

class TelematicsSession;

class TelematicsServer : public CppServer::Asio::TCPServer, public tc::LogI
{
public:
	using CppServer::Asio::TCPServer::TCPServer;

	TelematicsServer(const std::shared_ptr<AsioService> &service, std::shared_ptr<db::Client> &client, size_t cache = 10000);
	TelematicsServer(const std::shared_ptr<AsioService>& service, std::shared_ptr<db::Client>& client, size_t cache, int port);
	TelematicsServer(const std::shared_ptr<AsioService>& service, std::shared_ptr<db::Client>& client, size_t cache, int port, const std::string& address);

	virtual ~TelematicsServer();

	result_t handleCommand(const uchar *buffer, size_t size);
	result_t dispatchRequest(std::shared_ptr< packet::InternalRequest > &request, const CppCommon::UUID id);
	result_t handleRequest(const uchar *buffer, size_t size, const CppCommon::UUID id);

	size_t cacheSize() const;
	const std::shared_ptr<db::Client> dbClient();

protected:
	std::shared_ptr< CppServer::Asio::TCPSession > CreateSession(const std::shared_ptr<TCPServer> &server) override;

	void onConnected(std::shared_ptr< CppServer::Asio::TCPSession > &session) override;
	void onDisconnected(std::shared_ptr< CppServer::Asio::TCPSession > &session) override;
	void onError(int error, const std::string &category, const std::string &message) override;

private:
	result_t sendCommand(const Imei &imei, std::shared_ptr< packet::Command > &command);
	std::shared_ptr< db::Client > iDbClient;
	size_t iCacheSize;
};

} // namespace tc::server::tcp

#endif /* D1A0B0F8_F8ED_4A8E_AB96_417C353E7EA7 */