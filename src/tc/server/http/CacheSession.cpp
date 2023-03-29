#include <tc/server/http/CacheSession.h>
#include <string/string_utils.h>
#include <regex>

namespace tc::server::http {

HTTPCacheSession::HTTPCacheSession(const std::shared_ptr<CppServer::HTTP::HTTPServer> &server, const std::shared_ptr<CacheHandler> &cache)
 : HTTPSession(server)
 , iCache(cache)
{
	// nothing to do
}


void HTTPCacheSession::onReceivedRequest(const CppServer::HTTP::HTTPRequest& request)
{
	LG_NFO(this->logger(), "request: {}", request.string());
	if (iCache == nullptr) {
		return;
	}
	// Process HTTP request methods
	if (request.method() == "HEAD") {
		SendResponseAsync(response().MakeHeadResponse());
	}
	if (request.method() != "GET" && request.method() != "POST") {
		SendResponseAsync(response().MakeErrorResponse(400, "Bad request"));
	}

	Action action;
	if (action.parse(request) != RES_OK) {
		LG_ERR(this->logger(), "Bad {} request[{}]", request.method(), request.url());
		SendResponseAsync(response().MakeErrorResponse(400, "Bad request"));
		return;
	}

	CppServer::HTTP::HTTPResponse resp;
	if (result_t res; (res = iCache->handleAction(action, resp)) != RES_OK) {
		LG_ERR(this->logger(), "Unable to handle action[{}][{}]", request.method(), request.url());
		SendResponseAsync(resp);
		return;
	}

	SendResponseAsync(resp);
}

void HTTPCacheSession::onReceivedRequestError(const CppServer::HTTP::HTTPRequest &request, const std::string &error)
{
	LG_ERR(this->logger(), "Request error: {}", error);
}

void HTTPCacheSession::onError(int error, const std::string& category, const std::string& message)
{
	LG_ERR(this->logger(), "HTTPS session caught an error with code: {}, cat: {}, msg: {}", error, category, message);
}

} // namespace tc::server::http