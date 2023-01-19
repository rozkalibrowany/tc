#ifndef DEFE5823_FB5C_4F70_9A29_B3E2AE5E66D1
#define DEFE5823_FB5C_4F70_9A29_B3E2AE5E66D1

#include <server/asio/tcp_session.h>
#include <tc/server/tcp/Action.h>
#include <tc/common/LockGuard.h>
#include <tc/server/iot/Device.h>
#include <tc/parser/packet/PacketRequest.h>


namespace tc::server::tcp {

class TelematicsServer;
using namespace parser;

class TelematicsSession : public CppServer::Asio::TCPSession, public tc::LogI, public parser::JsonI
{

public:
	enum Response {
		eInvalid = 0,
		eOK = 1
	};

	using CppServer::Asio::TCPSession::TCPSession;

	bool hasImei(const Imei imei) const;
	Action::Type type() const;

	result_t send(int buffer, const bool async = false);
	result_t send(const uchar* buffer, size_t size, const bool async = false);
	result_t send(const void *buffer, size_t size, const bool async = false);

protected:
	result_t toJsonImpl(Json::Value &rhs, bool root) const override;
	void onReceived(const void *buffer, size_t size) override;

private:
	void handleDataBuffer(const uchar* buffer, size_t size, Action::Type type);

	result_t handleImei(const uchar *buffer, size_t size);
	result_t handleIncomplete(const uchar *buffer, size_t size);
	result_t handlePayload(const uchar *buffer, size_t size);
	result_t handleStandby(const uchar *buffer, size_t size);

	result_t savePacket(const std::shared_ptr<parser::PacketPayload> &packet);

	std::shared_ptr<TelematicsServer> telematicsServer();

	Imei iImei{"unknown"};
	SysMutex iMutex;
	SysTime iTimestamp;
	Action::Type iType{Action::unknown};
	std::unique_ptr<iot::Device> iDevice{nullptr};
	std::shared_ptr<parser::Buf> iBufferIncomplete{nullptr};
};

} // namespace tc::server::tcp

#endif /* DEFE5823_FB5C_4F70_9A29_B3E2AE5E66D1 */