#ifndef DEFE5823_FB5C_4F70_9A29_B3E2AE5E66D1
#define DEFE5823_FB5C_4F70_9A29_B3E2AE5E66D1

#include <server/asio/tcp_session.h>
#include <tc/server/tcp/Action.h>
#include <tc/server/tcp/LockGuard.h>

namespace tc::server::tcp {

class TelematicsServer;

class TelematicsSession : public CppServer::Asio::TCPSession, public tc::LogI
{
public:

	using CppServer::Asio::TCPSession::TCPSession;

	virtual std::shared_ptr<TelematicsServer> tcServer();

protected:
	void onReceived(const void *buffer, size_t size) override;

private:
	void handleDataBuffer(const void* buffer, size_t size, Action::Type type);
	result_t handlePayloadImei(const uchar *buffer, size_t size);
	result_t handleIncomplete(const uchar *buffer, size_t size, bool &crc_ok);
	result_t handlePayload(const uchar *buffer, size_t size, bool &crc_ok);
	result_t handleCommand(const uchar *buffer, size_t size);

	result_t checkCrc(std::shared_ptr< parser::Buf > buf, size_t size, bool &crc_ok);

	result_t send(void *buffer, size_t size, const bool async = false);
	result_t send(int buffer, const bool async = false);

	SysTime iTimestamp;
	SysMutex iMutex;
	std::string iImei;
	std::shared_ptr< parser::Buf > iBufferIncomplete {nullptr};
};

} // namespace tc::server::tcp

#endif /* DEFE5823_FB5C_4F70_9A29_B3E2AE5E66D1 */
