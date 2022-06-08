#include <tc/server/http/CacheSession.h>
#include <tc/server/http/Cache.h>
#include <string/string_utils.h>
#include <tc/server/http/RequestFactory.h>

namespace tc::server::http {

void HTTPSCacheSession::onReceivedRequest(const CppServer::HTTP::HTTPRequest& request)
{
	// Show HTTP request content
	LG_NFO(this->logger(), "request: {}", request.string());

	// Process HTTP request methods
	if (request.method() == "HEAD") {
		SendResponseAsync(response().MakeHeadResponse());
		LG_NFO(this->logger(), "request.method() == HEAD");
	}
	else if (request.method() == "GET")
	{
		auto action = request.string();
		action.erase(std::remove(action.begin(), action.end(), '/'), action.end());
		if (action.empty()) {
				// Response with all cache values
				SendResponseAsync(response().MakeGetResponse(Cache::GetInstance().GetAllCache(), "application/json; charset=UTF-8"));
		}
		/*parser::Buf buf;
		client::tcp::RequestFactory requestFactory(action);
		if (requestFactory.create(action,buf) != RES_OK) {
			LG_ERR(this->logger(), "Unable to handle request: {}", request.string());
			return;
		}

		if (connectClient() != RES_OK) {
			LG_ERR(this->logger(), "Unable to connect client: {}", request.string());
			return;
		}*/
	}
	else if ((request.method() == "POST") || (request.method() == "PUT"))
	{
			std::string key(request.url());
			std::string value(request.body());

			// Decode the key value
			key = CppCommon::Encoding::URLDecode(key);
			CppCommon::StringUtils::ReplaceFirst(key, "/api/cache", "");
			CppCommon::StringUtils::ReplaceFirst(key, "?key=", "");

			// Put the cache value
			Cache::GetInstance().PutCacheValue(key, value);

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

			// Delete the cache value
			if (Cache::GetInstance().DeleteCacheValue(key, value))
			{
					// Response with the cache value
					SendResponseAsync(response().MakeGetResponse(value));
			}
			else
					SendResponseAsync(response().MakeErrorResponse(404, "Deleted cache value was not found for the key: " + key));
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