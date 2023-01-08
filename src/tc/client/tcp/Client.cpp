#include <tc/client/tcp/Client.h>
#include <tc/parser/packet/Command.h>
#include <tc/common/LockGuard.h>

namespace tc::client::tcp {

Client::Client(Signal<const void *, size_t> &signal, const std::shared_ptr<Service> &service, const std::string &address, int port)
 : TCPClient(service, address, port)
 , iSignal(signal)
{
	this->SetupNoDelay(true);
}

result_t Client::send(const uchar *data, size_t size)
{
	result_t res = RES_OK;
	auto len = size / 2;
	auto out = new char[len];
	tc::hex2bin((const char*) data, out);

	{
		LockGuard g(iMutex);
		res = SendAsync(out, len) != true ? RES_ERROR : RES_OK;
	}
	delete out;
	return res;
}

result_t Client::send(const parser::Buf &buf)
{
	return send(buf.iBuf.data(), buf.iBuf.size());
}

result_t Client::send(const packet::InternalRequest &internal)
{
	LG_NFO(this->logger(), "Sending internal : size: {}", internal.size());

	return send(internal.cdata(), internal.size());
}

result_t Client::send(const Imei &imei, const std::string command, timestamp t)
{
	result_t res = RES_OK;

	if (t != 0) {
		return sendInternal(imei, command, t);
	}
	packet::Command cmd(imei);
	if ((res = cmd.create(command)) != RES_OK) {
		LG_ERR(this->logger(), "Unable to create command");
		return res;
	}

	LG_NFO(this->logger(), "Sending command : size: {} command: {} timestamp: {}", cmd.size(), command, t);


	return send(cmd.data(), cmd.size());
}

result_t Client::sendInternal(const Imei &imei, const std::string command, timestamp t)
{
	packet::InternalRequest internal;
	if (internal.create(Types::eGet, Types::str2type(command), imei, t) != RES_OK) {
		return RES_NOENT;
	}

	LG_NFO(this->logger(), "Sending internal : size: {} command: {} timestamp: {}", internal.size(), command, t);

	return send(internal.data(), internal.size());
}

void Client::onReceived(const void *buffer, size_t size)
{
	iSignal.emit(buffer, size);
}

void Client::onDisconnected()
{
	LG_ERR(this->logger(), "TCP client disconnected. Reconnecting...");
	this->ReconnectAsync();
}

} // namespace tc::client::tcp