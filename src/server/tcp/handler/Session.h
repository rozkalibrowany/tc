#ifndef E423DB7F_3FF8_475C_9016_B337F1B77F5E
#define E423DB7F_3FF8_475C_9016_B337F1B77F5E

#include <asio/tcp_session.h>
#include <list>

namespace server::tcp::handler {

class Session : public CppServer::Asio::TCPSession
{
public:
	using CppServer::Asio::TCPSession::TCPSession;
	using std::list < 

protected:
	void onConnected() override;
	void onDisconnected() override;

	void onReceived(const void* buffer, size_t size) override;
	void onError(int error, const std::string& category, const std::string& message) override;
private:

};

} // namespace server::tcp::handler

#endif /* E423DB7F_3FF8_475C_9016_B337F1B77F5E */
