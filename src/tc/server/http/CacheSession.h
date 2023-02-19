#ifndef D1B17E6F_7E12_4F90_8A8B_53760F4A7BE7
#define D1B17E6F_7E12_4F90_8A8B_53760F4A7BE7

#include <server/http/http_session.h>
#include <tc/server/http/CacheHandler.h>
#include <tc/asio/AsioService.h>
#include <tc/common/Logger.h>
namespace tc::server::http {

class HTTPCacheSession : public CppServer::HTTP::HTTPSession, public tc::LogI
{
public:
	using CppServer::HTTP::HTTPSession::HTTPSession;

	HTTPCacheSession(const std::shared_ptr<CppServer::HTTP::HTTPServer> &server, const std::shared_ptr<CacheHandler> &cache);

protected:
	result_t handle(const Action &action);

	void onReceivedRequest(const CppServer::HTTP::HTTPRequest &request) override;
	void onReceivedRequestError(const CppServer::HTTP::HTTPRequest &request, const std::string &error) override;
	void onError(int error, const std::string &category, const std::string &message) override;

private:
	std::shared_ptr< tc::asio::AsioService > iService;
	std::shared_ptr< CacheHandler > iCache {nullptr};
};

} // namespace tc::server::http

#endif /* D1B17E6F_7E12_4F90_8A8B_53760F4A7BE7 */
