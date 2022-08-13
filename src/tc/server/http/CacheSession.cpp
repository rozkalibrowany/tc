#include <tc/server/http/CacheSession.h>
#include <tc/server/http/Cache.h>
#include <string/string_utils.h>
#include <tc/server/http/RequestFactory.h>
#include <regex>

namespace tc::server::http {

void HTTPSCacheSession::setCache(const std::shared_ptr< Cache > &cache)
{
	iCache = std::move(cache);
}

result_t HTTPSCacheSession::handle(const Action &action)
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

void HTTPSCacheSession::onReceivedRequest(const CppServer::HTTP::HTTPRequest& request)
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
			if (action.parse(request) != RES_OK) {
				LG_ERR(this->logger(), "Bad POST request: {}", request.url());
				SendResponseAsync(response().MakeErrorResponse(400, "Bad request"));
				return;
			}

			LG_ERR(this->logger(), "action.iType: {} action.iID: {} action.iQueryParam: {},{}", action.iType, action.iID, action.iQueryParam.first, action.iQueryParam.second);

			if (action.iType == Action::Device) {
				iCache->addCommand(action.iID, action.iAction);
			} else {
				iCache->set(action.iID, action.iQueryParam);
			}
			SendResponseAsync(response().MakeOKResponse());
	}
	else if (request.method() == "DELETE")
	{
			std::string key(request.url());
			std::string value;

			// Decode the key value
			key = CppCommon::Encoding::URLDecode(key);
			CppCommon::StringUtils::ReplaceFirst(key, "/api/cache", "");
			CppCommon::StringUtils::ReplaceFirst(key, "?key=", "");

	}
	else if (request.method() == "OPTIONS")
			SendResponseAsync(response().MakeOptionsResponse());
	else if (request.method() == "TRACE")
			SendResponseAsync(response().MakeTraceResponse(request.cache()));
	else
			SendResponseAsync(response().MakeErrorResponse("Unsupported HTTP method: " + std::string(request.method())));
}

void HTTPSCacheSession::onReceivedRequestError(const CppServer::HTTP::HTTPRequest &request, const std::string &error)
{
	// cout << "Request error: " << error << std::endl;
}

void HTTPSCacheSession::onError(int error, const std::string& category, const std::string& message)
{
	// cout << "HTTPS session caught an error with code " << error << " and category '" << category << "': " << message << std::endl;
}

} // namespace tc::server::http