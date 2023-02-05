#include <tc/client/tcp/Client.h>

namespace tc::client::tcp {

Client::Client(Signal<const void *, size_t> &signal, const std::shared_ptr<Service> &service, const std::string &address, int port)
 : TCPClient(service, address, port)
 , iSignal(signal)
{
	// nothing to do
}

result_t Client::send(const parser::Buf &buf)
{
	result_t res = RES_OK;

	size_t len = buf.iBuf.size() / 2;
	auto out = new char[len];
	tc::hex2bin((char*)  buf.iBuf.data(), out);

	res = SendAsync(out, len) != true ? RES_ERROR : RES_OK;
	delete out;
	return res;
}

result_t Client::send(std::shared_ptr< parser::Command > command)
{
	LG_NFO(this->logger(), "Sending command: [{}]", tc::uchar2string(command->iBuf.data(), command->iBuf.size()));

	if (command == nullptr) {
		return RES_INVARG;
	}

	size_t len = command->iBuf.size() / 2;

	auto out = new char[len];
	tc::hex2bin((char*) command->iBuf.data(), out);

	auto ok = SendAsync(out, len);
	delete out;

	return ok == true ? RES_OK : RES_ERROR;
}

result_t Client::send(const Imei &imei, const std::string command)
{
	result_t res = RES_OK;

	auto cmd = std::make_shared< parser::Command >(imei);
	if ((res = cmd->create(command)) != RES_OK) {
		LG_ERR(this->logger(), "Unable to create command");
		return res;
	}

	return send(std::move(cmd));
}

void Client::onReceived(const void* buffer, size_t size)
{
	iSignal.emit(buffer, size);
}

void Client::onDisconnected()
{
	LG_ERR(this->logger(), "TCP client disconnected. Reconnecting...");
	this->ReconnectAsync();
}

} // namespace tc::client::tcp