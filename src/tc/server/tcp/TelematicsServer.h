#ifndef D1A0B0F8_F8ED_4A8E_AB96_417C353E7EA7
#define D1A0B0F8_F8ED_4A8E_AB96_417C353E7EA7

#include <tc/server/tcp/TelematicsSession.h>
#include <tc/parser/packet/PacketPayload.h>
#include <tc/parser/packet/PacketCommand.h>
#include <server/asio/tcp_server.h>

namespace tc::server::tcp {

using namespace parser;
class TelematicsSession;

class TelematicsServer : public CppServer::Asio::TCPServer, public tc::LogI
{
public:
	using CppServer::Asio::TCPServer::TCPServer;

	using Packets = std::vector< std::shared_ptr< parser::PacketPayload > >;
	using PayloadPackets = std::map< Imei, Packets >;
	using VerifiedSessions = std::map< CppCommon::UUID, Imei >;

	int sessionsSize();
	PayloadPackets &payloadPackets();

	virtual bool has(const CppCommon::UUID &uuid);
	virtual bool has(const Imei &imei);
	virtual result_t get(const CppCommon::UUID uuid, Imei &imei);
	virtual result_t get(const Imei &imei, parser::PacketPayload &packet);
	virtual result_t add(const CppCommon::UUID uuid, const Imei &imei);
	virtual result_t add(const Imei &imei, const std::shared_ptr<parser::PacketPayload> &packet);
	virtual result_t rm(const CppCommon::UUID uuid);

	virtual result_t sendCommand(const Imei &imei, std::shared_ptr<parser::PacketCommand> &command);

protected:
	std::shared_ptr< CppServer::Asio::TCPSession > CreateSession(const std::shared_ptr<TCPServer> &server) override;

	void onConnected(std::shared_ptr< CppServer::Asio::TCPSession > &session) override;
	void onDisconnected(std::shared_ptr< CppServer::Asio::TCPSession > &session) override;
	void onError(int error, const std::string &category, const std::string &message) override;

private:
	// void onConnected(std::shared_ptr< TelematicsSession > &session);
	// void onDisconnected(std::shared_ptr< TelematicsSession > &session);
	SysMutex iMutex;
	VerifiedSessions iVerifiedSessions;
	PayloadPackets iPayloadPackets;
};

} // namespace tc::server::tcp

#endif /* D1A0B0F8_F8ED_4A8E_AB96_417C353E7EA7 */
