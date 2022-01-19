#ifndef F2080690_61B2_4AD4_8BF5_96CEAFE240CE
#define F2080690_61B2_4AD4_8BF5_96CEAFE240CE

#include <server/asio/service.h>
#include <spdlog/spdlog.h>

namespace tc::server::tcp {

class AsioService : public CppServer::Asio::Service
{
public:
	using CppServer::Asio::Service::Service;

	virtual ~AsioService() = default;
};


} // namespace tc::server::tcp

#endif /* F2080690_61B2_4AD4_8BF5_96CEAFE240CE */
