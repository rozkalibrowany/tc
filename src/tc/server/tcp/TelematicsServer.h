#ifndef D1A0B0F8_F8ED_4A8E_AB96_417C353E7EA7
#define D1A0B0F8_F8ED_4A8E_AB96_417C353E7EA7

#include <tc/server/tcp/TelematicsSession.h>
#include <tc/parser/packet/PacketCommand.h>
#include <tc/server/iot/Devices.h>
#include <server/asio/tcp_server.h>
#include <mongocxx/instance.hpp>
#include <mongocxx/client.hpp>

namespace tc::server::tcp {

using namespace parser;
class TelematicsSession;

class TelematicsServer : public CppServer::Asio::TCPServer, public tc::LogI
{
public:
	using CppServer::Asio::TCPServer::TCPServer;

	result_t handleCommand(const uchar *buffer, size_t size);
	result_t dispatchRequest(std::shared_ptr< PacketRequest > &request, const CppCommon::UUID id);
	result_t handleRequest(const uchar *buffer, size_t size, const CppCommon::UUID id);

	bool iSaveRecords{true};
	mongocxx::client iDbClient {mongocxx::uri("mongodb+srv://ws:TelematicsConnector2021!@cluster0.7fif7ds.mongodb.net/?retryWrites=true&w=majority")};

protected:
	std::shared_ptr< CppServer::Asio::TCPSession > CreateSession(const std::shared_ptr<TCPServer> &server) override;

	void onConnected(std::shared_ptr< CppServer::Asio::TCPSession > &session) override;
	void onDisconnected(std::shared_ptr< CppServer::Asio::TCPSession > &session) override;
	void onError(int error, const std::string &category, const std::string &message) override;

private:
	result_t sendCommand(const Imei &imei, std::shared_ptr<parser::PacketCommand> &command);

};

} // namespace tc::server::tcp

#endif /* D1A0B0F8_F8ED_4A8E_AB96_417C353E7EA7 */