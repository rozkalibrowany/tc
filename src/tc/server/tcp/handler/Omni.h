#ifndef F9F31F6C_704F_4F53_9925_5F72ED796D61
#define F9F31F6C_704F_4F53_9925_5F72ED796D61

#include <tc/server/tcp/handler/HandlerI.h>

namespace tc::server::tcp {

class TelematicsSession;

class OmniHandler : public HandlerI {
	friend class TelematicsSession;
public:

	OmniHandler(const std::shared_ptr< TelematicsSession > &session)
	 : HandlerI(session) {}

	result_t handle(const uchar* buffer, size_t size) override;

private:
};

} // namespace tc::server::tcp

#endif /* F9F31F6C_704F_4F53_9925_5F72ED796D61 */
