#ifndef D1A0B0F8_F8ED_4A8E_AB96_417C353E7EA7
#define D1A0B0F8_F8ED_4A8E_AB96_417C353E7EA7

#include <tc/server/tcp/TelematicsSession.h>
#include <tc/parser/packet/PacketPayload.h>
#include <server/asio/tcp_server.h>

namespace tc::server::tcp {

class TelematicsServer : public CppServer::Asio::TCPServer, public tc::LogI
{
public:
	using CppServer::Asio::TCPServer::TCPServer;

	using Imei = std::string;
	using Packets = std::vector< std::shared_ptr< parser::PacketPayload > >;
	using PayloadPackets = std::map< Imei, Packets >;

	int sessionsSize();
	PayloadPackets &payloadPackets();

	virtual bool has(const Imei &imei);
	virtual result_t get(const Imei &imei, parser::PacketPayload &packet);
	virtual result_t add(const Imei imei, const std::shared_ptr<parser::PacketPayload> &packet);

protected:
	std::shared_ptr< CppServer::Asio::TCPSession > CreateSession(const std::shared_ptr<TCPServer> &server) override;

	void onConnected(std::shared_ptr< CppServer::Asio::TCPSession > &session) override;
	void onDisconnected(std::shared_ptr< CppServer::Asio::TCPSession > &session) override;
	void onError(int error, const std::string &category, const std::string &message) override;

private:
	// void onConnected(std::shared_ptr< TelematicsSession > &session);
	//void onDisconnected(std::shared_ptr< TelematicsSession > &session);
	SysMutex iMutex;
	PayloadPackets iPayloadPackets;
};

} // namespace tc::server::tcp

#endif /* D1A0B0F8_F8ED_4A8E_AB96_417C353E7EA7 */
