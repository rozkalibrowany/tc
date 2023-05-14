#include <tc/client/tcp/Client.h>
#include <tc/parser/teltonika/Command.h>
#include <tc/parser/omni/Command.h>

namespace tc::client::tcp {

Client::Client(Signal<const void *, size_t> &signal, const std::shared_ptr<Service> &service, const std::string &address, int port)
 : TCPClient(service, address, port)
 , iSignal(signal)
{
	// nothing to do
}

Client::~Client()
{
	Disconnect();
}

result_t Client::send(const common::Buf &buf)
{
	result_t res = RES_OK;

	size_t len = buf.iBuf.size() / 2;
	auto out = new char[len];
	tc::hex2bin((char*)  buf.iBuf.data(), out);

	res = SendAsync(out, len) != true ? RES_ERROR : RES_OK;
	delete out;
	return res;
}

result_t Client::send(std::shared_ptr< parser::CommandI > command)
{
	LG_NFO(this->logger(), "Sending command: [{}]", tc::uchar2string(command->buf().data(), command->size()));

	if (command == nullptr) {
		return RES_INVARG;
	}

	size_t len = command->buf().size() / 2;

	auto out = new char[len];
	tc::hex2bin((char*) command->buf().data(), out);

	auto ok = SendAsync(out, len);
	delete out;

	return ok == true ? RES_OK : RES_ERROR;
}

void Client::onReceived(const void* buffer, size_t size)
{
	LG_NFO(this->logger(), "Client received: [{}]", size);
	iSignal.emit(buffer, size);
}

void Client::onDisconnected()
{
	LG_ERR(this->logger(), "TCP client disconnected. Reconnecting...");
	this->ReconnectAsync();
}

} // namespace tc::client::tcp