#ifndef C654F989_E9FE_40CC_A66B_6803FFA9FE3F
#define C654F989_E9FE_40CC_A66B_6803FFA9FE3F

#include <tc/server/tcp/handler/HandlerI.h>

namespace tc::server::tcp {

class TelematicsSession;

class InternalHandler : public HandlerI {
	friend class TelematicsSession;
public:

	InternalHandler(const std::shared_ptr< TelematicsSession > &session)
	 : HandlerI(session, eInternal) {}

	result_t handle(const uchar* buffer, size_t size) override;

private:
	result_t handleCommand(const uchar *buffer, size_t size);
	result_t handleRequest(const uchar *buffer, size_t size);

	result_t handleTeltonikaCommand(const uchar *buffer, size_t size);
	result_t handleOmniCommand(const uchar *buffer, size_t size);
};

} // namespace tc::server::tcp

#endif /* C654F989_E9FE_40CC_A66B_6803FFA9FE3F */
