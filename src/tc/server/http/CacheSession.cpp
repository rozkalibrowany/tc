#include <tc/server/http/CacheSession.h>
#include <tc/server/http/Cache.h>
#include <string/string_utils.h>
#include <tc/server/http/RequestFactory.h>
#include <regex>

namespace tc::server::http {

void HTTPCacheSession::setCache(const std::shared_ptr< Cache > &cache)
{
	iCache = std::move(cache);
}

result_t HTTPCacheSession::handle(const Action &action)
{
	result_t res = RES_OK;

	if (iCache == nullptr) {
		return RES_NOENT;
	}

	parser::Buf buf;
	RequestFactory req_fac;
	if ((res = req_fac.create(action, buf)) != RES_OK) {
		LG_ERR(this->logger(), "Unable to create request factory.");
		return res;
	}

	size_t len = buf.size() / 2;
	auto out = new char[len];
	tc::hex2bin((char*) buf.data(), out);

	return SendAsync(out, len) != true ? RES_ERROR : RES_OK;
}

void HTTPCacheSession::onReceivedRequest(const CppServer::HTTP::HTTPRequest& request)
{
	// Show HTTP request content
	LG_NFO(this->logger(), "request: {}", request.string());
	if (iCache == nullptr) {
		return;
	}
	// Process HTTP request methods
	if (request.method() == "HEAD") {
		SendResponseAsync(response().MakeHeadResponse());
		LG_NFO(this->logger(), "request.method() == HEAD");
	}
	else if (request.method() == "GET") {
		auto action = request.string();
		action.erase(std::remove(action.begin(), action.end(), '/'), action.end());
		SendResponseAsync(response().MakeGetResponse(iCache->getDevices().toStyledString(), "application/json; charset=UTF-8"));

	}
	else if ((request.method() == "POST") || (request.method() == "PUT")) {
			Action action;
			result_t res = RES_OK;
			if (action.parse(request) != RES_OK) {
				LG_ERR(this->logger(), "Bad POST request: {}", request.url());
				SendResponseAsync(response().MakeErrorResponse(400, "Bad request"));
				return;
			}

			if (action.iType == Action::Device) {
				res |= iCache->addCommand(action.iID, action.iAction);
			} else {
				res |= iCache->set(action.iID, action.iQueryParam);
			}

			if (res == RES_OK)
				SendResponseAsync(response().MakeOKResponse());
			else
				SendResponseAsync(response().MakeErrorResponse(400, "Bad request"));
	} else if (request.method() == "DELETE") {
		// TODO
	}
	else if (request.method() == "OPTIONS")
			SendResponseAsync(response().MakeOptionsResponse());
	else if (request.method() == "TRACE")
			SendResponseAsync(response().MakeTraceResponse(request.cache()));
	else
			SendResponseAsync(response().MakeErrorResponse("Unsupported HTTP method: " + std::string(request.method())));
}

void HTTPCacheSession::onReceivedRequestError(const CppServer::HTTP::HTTPRequest &request, const std::string &error)
{
	// cout << "Request error: " << error << std::endl;
}

void HTTPCacheSession::onError(int error, const std::string& category, const std::string& message)
{
	// cout << "HTTPS session caught an error with code " << error << " and category '" << category << "': " << message << std::endl;
}

} // namespace tc::server::http