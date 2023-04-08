#include <tc/server/tcp/RequestDispatcher.h>

namespace tc::server::tcp {

RequestDispatcher::RequestDispatcher(const std::shared_ptr< parser::InternalRequest > &request)
 : iRequest(std::move(request))
{
  // nothing to do
}



} // namespace tc::server::tcp
