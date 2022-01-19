#ifndef E423DB7F_3FF8_475C_9016_B337F1B77F5E
#define E423DB7F_3FF8_475C_9016_B337F1B77F5E

#include <tc/parser/Packet.h>
#include <server/asio/tcp_session.h>
#include <tc/common/Logger.h>

namespace tc::server::tcp {

using namespace CppServer::Asio;

class Server;
class Session;
class SessionHandler;

using SessionSPtr = std::shared_ptr<Session>;

class Session : public TCPSession, public common::LogI
{
	friend class Server;

public:
	enum Action
	{
		A_DEFAULT = 0,
		A_PARSE_IMEI,
		A_PARSE_DATA,
		A_NO_ACTION
	};

	enum Response
	{
		R_REFUSE = 0,
		R_ACCEPT = 1
	};

	using TCPSession::TCPSession;

	Session(const std::shared_ptr<TCPServer> &server);
	Session(const Session&) = delete;
	Session(Session&&) = delete;

	virtual ~Session() = default;

	SysTime iTimestamp;
protected:
	void onReceived(const void *buffer, size_t size) override;

private:
	result_t handleDataBuffer(const void *buffer, size_t size);
	void handleResponse(Action action, bool async = false);

	std::string ids() const;
	Action getAction(size_t size);
	std::shared_ptr<Server> iServer;
};

} // namespace tc::server::tcp

#endif /* E423DB7F_3FF8_475C_9016_B337F1B77F5E */
