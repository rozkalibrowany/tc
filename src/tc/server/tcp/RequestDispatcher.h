#ifndef A0DB7237_906D_4DD5_BEC2_CF49FAE2F746
#define A0DB7237_906D_4DD5_BEC2_CF49FAE2F746

#include <tc/common/Common.h>
#include <tc/parser/internal/Request.h>

namespace tc::server::tcp {

class RequestDispatcher : public tc::LogI {
public:
  RequestDispatcher(const std::shared_ptr< parser::internal::Request > &request);
  virtual ~RequestDispatcher() = default;


protected:
  std::shared_ptr< parser::internal::Request > iRequest;
};

} // namespace tc::server::tcp

#endif /* A0DB7237_906D_4DD5_BEC2_CF49FAE2F746 */
