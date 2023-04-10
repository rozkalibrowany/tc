#ifndef DEFE5823_FB5C_4F70_9A29_B3E2AE5E66D1
#define DEFE5823_FB5C_4F70_9A29_B3E2AE5E66D1

#include <server/asio/tcp_session.h>
#include <tc/server/tcp/handler/HandlerI.h>
#include <tc/parser/internal/Request.h>
#include <tc/parser/Protocol.h>
#include <tc/iot/Device.h>

namespace tc::server::tcp {

class TelematicsServer;
using namespace parser;

class TelematicsSession : public CppServer::Asio::TCPSession, public tc::LogI, public parser::JsonI
{
	friend class HandlerI;
	friend class TeltonikaHandler;
public:
	using CppServer::Asio::TCPSession::TCPSession;

	const Imei imei() const;

	result_t send(int buffer, const bool async = false);
	result_t send(const uchar* buffer, size_t size, const bool async = false);
	result_t send(const void *buffer, size_t size, const bool async = false);

	result_t lastPacketJson(Json::Value &rhs);

protected:
	result_t toJsonImpl(Json::Value &rhs, bool root) const override;
	
	void onReceived(const void *buffer, size_t size) override;
	std::shared_ptr<TelematicsServer> server();

private:
	result_t savePacket(std::shared_ptr<parser::teltonika::Payload> &packet);

	Protocol iProtocol;
	std::unique_ptr<HandlerI> iHandler{nullptr};
	std::unique_ptr<iot::Device> iDevice{nullptr};
};

} // namespace tc::server::tcp

#endif /* DEFE5823_FB5C_4F70_9A29_B3E2AE5E66D1 */