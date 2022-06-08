#ifndef A2838638_F778_440C_82F6_9C262ECB07BD
#define A2838638_F778_440C_82F6_9C262ECB07BD

#include <server/asio/tcp_client.h>
#include <tc/server/http/Action.h>
#include <tc/server/http/RequestFactory.h>
#include <tc/parser/Buf.h>

namespace tc::server::http {

class Client : public CppServer::Asio::TCPClient, public tc::LogI
{
public:
	using CppServer::Asio::TCPClient::TCPClient;
	
	result_t handle(const Action &action);

protected:
	void onReceived(const void *buffer, size_t size) override;

private:
	std::atomic<bool> _stop{false};
};

} // tc::client::tcp

#endif /* A2838638_F778_440C_82F6_9C262ECB07BD */
