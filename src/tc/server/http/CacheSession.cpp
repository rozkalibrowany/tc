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
			const std::regex action("\\/(.*?)\\/");
			const std::regex id("\\/([0-9]+)\\/");
			const std::regex cmd("([^\/]+$)");

			std::string url(request.url());

			auto str_action = tc::regex(action, url);
			auto str_id = tc::regex(id, url);
			auto str_cmd = tc::regex(cmd, url);

			if (str_action.empty() || str_id.empty() || str_cmd.empty()) {
				LG_ERR(this->logger(), "Bad POST request: {}", url);
				return;
			}

			if (str_cmd == "set") {

			} /*else {
				iCache->addCommand(str_id, str_cmd);
			}*/

			// Response with the cache value
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