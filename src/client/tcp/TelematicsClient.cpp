#include <client/tcp/TelematicsClient.h>

namespace tc::client::tcp {

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