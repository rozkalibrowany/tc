#ifndef A650B499_9C59_4E32_AE77_57A0CD8B07EC
#define A650B499_9C59_4E32_AE77_57A0CD8B07EC

#include <server/asio/service.h>
#include <spdlog/spdlog.h>

namespace tc::client::tcp {

class AsioService : public CppServer::Asio::Service
{
public:
	using CppServer::Asio::Service::Service;

	virtual ~AsioService() = default;
};


} // namespace tc::server::tcp

#endif /* A650B499_9C59_4E32_AE77_57A0CD8B07EC */
