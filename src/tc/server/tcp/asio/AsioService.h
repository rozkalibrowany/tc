#ifndef F2080690_61B2_4AD4_8BF5_96CEAFE240CE
#define F2080690_61B2_4AD4_8BF5_96CEAFE240CE

#include <server/asio/service.h>
#include <spdlog/spdlog.h>

namespace tc::server::tcp {

class AsioService : public CppServer::Asio::Service
{
public:
	using CppServer::Asio::Service::Service;

	protected:
    void onError(int error, const std::string& category, const std::string& message) override
    {
        //spdlog::critical("Asio service caught an error with code {} and category {}", error, category);
    }
};


} // namespace tc::server::tcp

#endif /* F2080690_61B2_4AD4_8BF5_96CEAFE240CE */
