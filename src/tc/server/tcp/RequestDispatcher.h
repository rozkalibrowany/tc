#ifndef A0DB7237_906D_4DD5_BEC2_CF49FAE2F746
#define A0DB7237_906D_4DD5_BEC2_CF49FAE2F746

#include <tc/common/Common.h>
#include <tc/parser/packet/InternalRequest.h>

namespace tc::server::tcp {

	using namespace parser;
	class RequestDispatcher : public tc::LogI
	{
	public:
		RequestDispatcher(const std::shared_ptr<packet::InternalRequest> &request);
		virtual ~RequestDispatcher() = default;

	protected:
		std::shared_ptr<packet::InternalRequest> iRequest;
	};

} // namespace tc::server::tcp

#endif /* A0DB7237_906D_4DD5_BEC2_CF49FAE2F746 */
