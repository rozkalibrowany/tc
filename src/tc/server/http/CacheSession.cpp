#include <tc/server/http/CacheSession.h>
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
		//LG_ERR(this->logger(), "Unable to create request factory.");
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
	//LG_NFO(this->logger(), "request: {}", request.string());
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
	result_t res;
	if ((res = action.parse(request)) != RES_OK) {
		//LG_ERR(this->logger(), "Bad {} request[{}]", request.method(), request.url());
		SendResponseAsync(response().MakeErrorResponse(400, "Bad request"));
		return;
	}

	CppServer::HTTP::HTTPResponse resp;
	if ((res = iCache->handleAction(action, resp)) != RES_OK) {
		//LG_ERR(this->logger(), "Unable to handle action[{}][{}]", request.method(), request.url());
		resp = res == RES_NOIMPL ? response().MakeErrorResponse(500, "Internal Server Error") : response().MakeErrorResponse(400, "Bad Request");
		SendResponseAsync(resp);
		return;
	}

	SendResponseAsync(response().MakeOKResponse());
}

void HTTPCacheSession::onReceivedRequestError(const CppServer::HTTP::HTTPRequest &request, const std::string &error)
{
	//LG_ERR(this->logger(), "Request error: {}", error);
}

void HTTPCacheSession::onError(int error, const std::string& category, const std::string& message)
{
	//LG_ERR(this->logger(), "HTTPS session caught an error with code: {}, cat: {}, msg: {}", error, category, message);
}

} // namespace tc::server::http