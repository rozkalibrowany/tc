#ifndef DEFE5823_FB5C_4F70_9A29_B3E2AE5E66D1
#define DEFE5823_FB5C_4F70_9A29_B3E2AE5E66D1

#include <tc/parser/packet/Packet.h>
#include <server/asio/tcp_session.h>
#include <tc/common/Common.h>
#include <tc/server/tcp/Action.h>
#include <tc/server/tcp/telematics/LockGuard.h>

namespace tc::server::tcp {

using namespace CppServer::Asio;

class TelematicsServer;
class TelematicsSession;
class SessionHandler;

class TelematicsSession : public TCPSession, public tc::LogI
{
	friend class TelematicsServer;

public:
	enum Response
	{
		refuse = 0,
		accept = 1
	};

	using TCPSession::TCPSession;

	TelematicsSession(const std::shared_ptr< TCPServer > &server);
	TelematicsSession(const TelematicsSession&) = delete;
	TelematicsSession(TelematicsSession&&) = delete;

	TelematicsSession& operator=(const TelematicsSession&) = delete;
	TelematicsSession& operator=(TelematicsSession&&) = delete;

	~TelematicsSession();

	SysTime iTimestamp;
protected:
	void onReceived(const void *buffer, size_t size) override;

private:
	result_t handleDataBuffer(const void *buffer, size_t size, Action &action);
	result_t handlePayload(const void *buffer, size_t size, Action &action);
	result_t handleCommand(const void *buffer, size_t size);
	result_t handleUnknown(const void *buffer, size_t size);

	std::string ids() const;
	// void handleResponse(Action action, bool async = false);

	SysMutex iMutex;
	std::string iImei;
	Action iAction;
	std::shared_ptr< TelematicsServer > iServer;
};

} // namespace tc::server::tcp

#endif /* DEFE5823_FB5C_4F70_9A29_B3E2AE5E66D1 */
