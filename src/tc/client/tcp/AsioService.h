#ifndef A650B499_9C59_4E32_AE77_57A0CD8B07EC
#define A650B499_9C59_4E32_AE77_57A0CD8B07EC

#include <server/asio/service.h>
namespace tc::client::tcp {

class AsioService : public CppServer::Asio::Service
{
public:
	using CppServer::Asio::Service::Service;

protected:
	void onError(int error, const std::string &category, const std::string &message) override;
};

} // namespace tc::server::tcp

#endif /* A650B499_9C59_4E32_AE77_57A0CD8B07EC */
