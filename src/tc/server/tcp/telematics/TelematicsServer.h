#ifndef D1A0B0F8_F8ED_4A8E_AB96_417C353E7EA7
#define D1A0B0F8_F8ED_4A8E_AB96_417C353E7EA7

#include <tc/server/tcp/telematics/TelematicsSession.h>
#include <tc/parser/packet/PacketPayload.h>
#include <tc/common/Common.h>
#include <server/asio/tcp_server.h>
#include <set>

namespace tc::server::tcp {

using namespace CppCommon;
using namespace parser;

class TelematicsServer : public CppServer::Asio::TCPServer, public tc::LogI
{
public:
	using CppServer::Asio::TCPServer::TCPServer;

	using Imei = std::string;
	using Packets = std::vector< std::shared_ptr< PacketPayload > >;
	using PayloadPackets = std::map< Imei, Packets >;

	TelematicsServer(const std::shared_ptr<Service>& service, int port);

	virtual ~TelematicsServer() = default;

	int sessionsSize();
	result_t get(const Imei &imei, std::shared_ptr< PacketPayload > &packet);
	result_t add(const Imei imei, const std::shared_ptr<PacketPayload> &packet);

	void sendCmd();

	PayloadPackets &payloadPackets();

protected:
	virtual std::shared_ptr< TCPSession > CreateSession(const std::shared_ptr<TCPServer> &server);

	virtual void onConnected(std::shared_ptr< TCPSession > &session);
	virtual void onDisconnected(std::shared_ptr< TCPSession > &session);
	virtual void onError(int error, const std::string &category, const std::string &message) override;

private:
	// void onConnected(std::shared_ptr< TelematicsSession > &session);
	//void onDisconnected(std::shared_ptr< TelematicsSession > &session);
	SysMutex iMutex;
	PayloadPackets iPayloadPackets;
};

} // namespace tc::server::tcp

#endif /* D1A0B0F8_F8ED_4A8E_AB96_417C353E7EA7 */
