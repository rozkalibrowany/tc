#ifndef A2838638_F778_440C_82F6_9C262ECB07BD
#define A2838638_F778_440C_82F6_9C262ECB07BD

#include <server/asio/tcp_client.h>
#include <tc/common/Signal.h>
#include <tc/parser/Buf.h>
#include <tc/parser/Command.h>

namespace tc::client::tcp {

	using namespace CppServer::Asio;

	class Client : public CppServer::Asio::TCPClient, public tc::LogI
	{
	public:
		using TCPClient::TCPClient;

		Client(const std::shared_ptr<Service> &service, const std::string &address, int port);
		~Client();
		
		result_t send(const parser::Buf &buf);
		result_t send(std::shared_ptr<parser::Command> command);
		result_t send(const Imei &imei, const std::string command);

		Signal<const void *, size_t> &signal();

	protected:
		void onReceived(const void *buffer, size_t size) override;
		void onDisconnected() override;

	private:
		Signal<const void *, size_t> iSignal;
};

} // tc::client::tcp

#endif /* A2838638_F778_440C_82F6_9C262ECB07BD */
