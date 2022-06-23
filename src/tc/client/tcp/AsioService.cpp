#include <tc/client/tcp/AsioService.h>

namespace tc::client::tcp {

void AsioService::onError(int error, const std::string& category, const std::string& message)
{
      std::cout << "Asio service caught an error with code " << error << " and category '" << category << "': " << message << std::endl;
}

} // namespace tc::client::tcp