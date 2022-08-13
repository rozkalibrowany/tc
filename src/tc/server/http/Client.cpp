#include <tc/server/http/Client.h>

namespace tc::server::http {

void Client::setCache(const std::shared_ptr< Cache > &cache)
{
	iCache = std::move(cache);
}

result_t Client::handle(const Action &action)
{
	result_t res = RES_OK;

	if (iCache == nullptr) {
		return RES_NOENT;
	}

	parser::Buf buf;
	RequestFactory req_fac;
	if ((res = req_fac.create(action, buf)) != RES_OK) {
		LG_ERR(this->logger(), "Unable to process action within request factory");
		return res;
	}

	size_t len = buf.size() / 2;
	auto out = new char[len];
	tc::hex2bin((char*) buf.data(), out);

	return SendAsync(out, len) != true ? RES_ERROR : RES_OK;
}

result_t Client::handle(std::shared_ptr< parser::Command > command)
{
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

/*result_t Client::handle(std::shared_ptr< parser::Buf > command)
{
	if (command == nullptr) {
		return RES_INVARG;
	}

	size_t len = command->size() / 2;
	auto out = new char[len];
	tc::hex2bin((char*) command->data(), out);

	auto ok = SendAsync(out, len);
	delete out;
	return ok == true ? RES_OK : RES_ERROR;
}*/

void Client::onReceived(const void *buffer, size_t size)
{
	if (iCache != nullptr) {
		return iCache->onReceived(buffer, size);
	}
}

} // namespace tc::server::http