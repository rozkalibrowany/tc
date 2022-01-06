#ifndef E423DB7F_3FF8_475C_9016_B337F1B77F5E
#define E423DB7F_3FF8_475C_9016_B337F1B77F5E

#include <server/asio/tcp_session.h>
#include <tc/parser/Packet.h>
#include <chrono>

namespace tc::server::tcp {

using namespace CppServer::Asio;
using namespace std::chrono;

class Session : public TCPSession
{
public:
	using TCPSession::TCPSession;

protected:
	void onConnected() override;
	void onDisconnected() override;

	void onReceived(const void *buffer, size_t size) override;
	void onError(int error, const std::string &category, const std::string &message) override;

	int64_t timestamp() const;

private:
	std::shared_ptr<parser::Packet> iPacket;
	int64_t iTimestamp;
};

} // namespace tc::server::tcp

#endif /* E423DB7F_3FF8_475C_9016_B337F1B77F5E */
