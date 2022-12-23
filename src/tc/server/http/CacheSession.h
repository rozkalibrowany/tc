#ifndef D1B17E6F_7E12_4F90_8A8B_53760F4A7BE7
#define D1B17E6F_7E12_4F90_8A8B_53760F4A7BE7

#include <server/http/http_session.h>
#include <tc/server/http/Cache.h>
#include <tc/asio/AsioService.h>
namespace tc::server::http {

class HTTPCacheSession : public CppServer::HTTP::HTTPSession
{
public:
	using CppServer::HTTP::HTTPSession::HTTPSession;

	virtual void setCache(const std::shared_ptr<Cache> &cache);

protected:
	result_t handle(const Action &action);

	void onReceivedRequest(const CppServer::HTTP::HTTPRequest &request) override;
	void onReceivedRequestError(const CppServer::HTTP::HTTPRequest &request, const std::string &error) override;
	void onError(int error, const std::string &category, const std::string &message) override;

private:
	std::shared_ptr< tc::asio::AsioService > iService;
	std::shared_ptr< Cache > iCache {nullptr};
	int iPort {8443};
	std::string iAddress {"127.0.0.1"};

};

} // namespace tc::server::http

#endif /* D1B17E6F_7E12_4F90_8A8B_53760F4A7BE7 */
