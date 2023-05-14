#include <tc/asio/AsioService.h>
#include <fmt/format.h>

namespace tc::asio {

void AsioService::onError(int error, const std::string& category, const std::string& message)
{
	throw std::runtime_error(fmt::format("Asio caught an error[{}][{}][{}]", error, category, message));
}

} // namespace tc::server::tcp