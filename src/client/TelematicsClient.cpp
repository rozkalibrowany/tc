#include <tc/client/TelematicsClient.h>

namespace tc::client::tcp {


TelematicsClient::TelematicsClient(const std::shared_ptr< AsioService >& service, const std::string& address, int port)
 : CppServer::Asio::TCPClient(service, address, port)
 , tc::LogI("console")
{
	// nothing to do
}

void TelematicsClient::disconnectAndStop()
{
	_stop = true;
	DisconnectAsync();
	while (IsConnected())
		CppCommon::Thread::Yield();
}

void TelematicsClient::onConnected()
{
	LG_NFO(this->logger(), "TCP client connected with session: {}", id().string());
}

void TelematicsClient::onDisconnected()
{
	LG_NFO(this->logger(), "TCP client disconnected with session: {}", id().string());
}

void TelematicsClient::onReceived(const void *buffer, size_t size)
{
	LG_DBG(this->logger(), "Incoming: {}", std::string((const char*)buffer, size));
}

void TelematicsClient::onError(int error, const std::string &category, const std::string &message)
{
	LG_DBG(this->logger(), "Chat TCP client caught an error: {} [{}] : {}", error, category, message);
}

} // namespace tc::client::tcp