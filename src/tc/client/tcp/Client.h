#ifndef A2838638_F778_440C_82F6_9C262ECB07BD
#define A2838638_F778_440C_82F6_9C262ECB07BD

#include <server/asio/tcp_client.h>
#include <tc/common/Signal.h>
#include <tc/common/SysMutex.h>
#include <tc/parser/packet/InternalRequest.h>
#include <tc/parser/Buf.h>

namespace tc::client::tcp {

	using namespace CppServer::Asio;
	using namespace parser;
	class Client : public CppServer::Asio::TCPClient, public tc::LogI
	{
	public:
		using TCPClient::TCPClient;

		Client(Signal<const void *, size_t> &signal, const std::shared_ptr<Service> &service, const std::string &address, int port);

		result_t send(const uchar *data, size_t size);
		result_t send(const parser::Buf &buf);
		result_t send(const packet::InternalRequest &internal);
		result_t send(const Imei &imei, const std::string command, timestamp t = 0);
		result_t sendInternal(const Imei &imei, const std::string command, timestamp t);

	protected:
		void onReceived(const void *buffer, size_t size) override;
		void onDisconnected() override;

	private:
		Signal<const void *, size_t> &iSignal;
		mutable SysMutex iMutex;
};

} // tc::client::tcp

#endif /* A2838638_F778_440C_82F6_9C262ECB07BD */
