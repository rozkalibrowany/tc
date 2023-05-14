#ifndef A2838638_F778_440C_82F6_9C262ECB07BD
#define A2838638_F778_440C_82F6_9C262ECB07BD

#include <server/asio/tcp_client.h>
#include <tc/common/Signal.h>
#include <tc/common/Buf.h>
#include <tc/parser/CommandI.h>

namespace tc::client::tcp {

	using namespace CppServer::Asio;

	class Client : public CppServer::Asio::TCPClient, public tc::LogI
	{
	public:
		using TCPClient::TCPClient;

		Client(Signal<const void *, size_t> &signal, const std::shared_ptr<Service> &service, const std::string &address, int port);
		~Client();
		
		result_t send(const common::Buf &buf);
		result_t send(std::shared_ptr<parser::CommandI> command);
		
	protected:
		void onReceived(const void *buffer, size_t size) override;
		void onDisconnected() override;

	private:
		Signal<const void *, size_t> &iSignal;
};

} // tc::client::tcp

#endif /* A2838638_F778_440C_82F6_9C262ECB07BD */
