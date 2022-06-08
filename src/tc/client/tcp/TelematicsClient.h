#ifndef EADCF7E8_6DE0_41F5_9945_A81AEEE0442E
#define EADCF7E8_6DE0_41F5_9945_A81AEEE0442E

#include <server/asio/tcp_client.h>
#include <tc/common/Common.h>

namespace tc::client::tcp {

class TelematicsClient : public CppServer::Asio::TCPClient, public tc::LogI
{
public:
	using CppServer::Asio::TCPClient::TCPClient;

	virtual void disconnectAndStop();

protected:
	void onConnected() override;
	void onDisconnected() override;
	void onReceived(const void *buffer, size_t size) override;
	void onError(int error, const std::string &category, const std::string &message) override;

private:
	std::atomic<bool> _stop{false};
};

} // tc::client::tcp

#endif /* EADCF7E8_6DE0_41F5_9945_A81AEEE0442E */
