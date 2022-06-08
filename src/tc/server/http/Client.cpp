#include <tc/server/http/Client.h>

namespace tc::server::http {

void Client::setCache(const std::shared_ptr< Cache > &cache)
{
	iCache = std::move(cache);
}

result_t Client::handle(const Action &action)
{
	result_t res;
	if (action.iReq.empty()) {
		return RES_NOENT;
	}

	parser::Buf buf;
	RequestFactory req_fac;
	if ((res = req_fac.create(action, buf)) != RES_OK) {
		return res;
	}

	size_t len = buf.size() / 2;
	auto out = new char[len];
	tc::hex2bin((char*) buf.data(), out);

	return SendAsync(out, len) != true ? RES_ERROR : RES_OK;
}

void Client::onReceived(const void *buffer, size_t size)
{
	if (iCache != nullptr) {
		return iCache->onReceived(buffer, size);
	}
}

} // namespace tc::server::http