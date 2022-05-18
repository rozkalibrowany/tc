#ifndef D1B17E6F_7E12_4F90_8A8B_53760F4A7BE7
#define D1B17E6F_7E12_4F90_8A8B_53760F4A7BE7

#include <server/http/https_server.h>
#include <tc/common/Common.h>

namespace tc::server::http {

class HTTPSCacheSession : public CppServer::HTTP::HTTPSSession, public tc::LogI
{
public:
    using CppServer::HTTP::HTTPSSession::HTTPSSession;

protected:
	void onReceivedRequest(const CppServer::HTTP::HTTPRequest &request) override;
	void onReceivedRequestError(const CppServer::HTTP::HTTPRequest &request, const std::string &error) override;
	void onError(int error, const std::string &category, const std::string &message) override;
};

} // namespace tc::server::http

#endif /* D1B17E6F_7E12_4F90_8A8B_53760F4A7BE7 */
