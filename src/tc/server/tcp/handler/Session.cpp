#include <handler/Session.h>

namespace server::tcp::handler {

void Session::onConnected()
{
	std::cout << "TCP session with Id " << id() << " connected!" << std::endl;

	
}

void Session::onDisconnected()
{

}

void Session::onReceived(const void* buffer, size_t size)
{

}

void Session::onError(int error, const std::string& category, const std::string& message)
{

}

} // namespace server::tcp::handler