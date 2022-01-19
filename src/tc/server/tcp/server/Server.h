#ifndef D1A0B0F8_F8ED_4A8E_AB96_417C353E7EA7
#define D1A0B0F8_F8ED_4A8E_AB96_417C353E7EA7

#include <tc/server/tcp/session/Session.h>
#include <tc/parser/Packet.h>
#include <server/asio/tcp_server.h>
#include <tc/common/Logger.h>
#include <set>

namespace tc::server::tcp {

using namespace CppCommon;
using namespace parser;

class Server : public CppServer::Asio::TCPServer, public common::LogI
{

public:
	using CppServer::Asio::TCPServer::TCPServer;

	using PacketImei = std::string;
	using SessionPacketMap = std::map<UUID, parser::Packet>;
	using PacketSet = std::set<parser::Packet>;
	using PacketMap = std::map<PacketImei, PacketSet>;

	Server(const std::shared_ptr<Service>& service, int port);

	virtual ~Server() = default;

	int sessionsSize();
	result_t get(parser::Packet &packet, const UUID uuid);
	result_t add(const PacketImei &imei, parser::Packet &packet);
	/*virtual result_t del(const UUID uuid);

	virtual result_t add(parser::PacketUPtr packet);
	virtual result_t del(parser::PacketUPtr packet);*/

	SessionPacketMap &sessionPackets();

protected:
	virtual std::shared_ptr<TCPSession> CreateSession(const std::shared_ptr<TCPServer> &server);

	void onConnected(std::shared_ptr<TCPSession> &session) override;
	void onDisconnected(std::shared_ptr<TCPSession> &session) override;
	void onError(int error, const std::string &category, const std::string &message) override;

private:
	void onConnected(std::shared_ptr< Session > &session);
	void onDisconnected(std::shared_ptr< Session > &session);

	SessionPacketMap iSessionPacketMap;
	PacketMap iPacketMap;
};

} // namespace tc::server::tcp

#endif /* D1A0B0F8_F8ED_4A8E_AB96_417C353E7EA7 */
