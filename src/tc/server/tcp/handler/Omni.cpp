#include <tc/server/tcp/handler/Omni.h>
#include <tc/server/tcp/TelematicsSession.h>
#include <tc/server/tcp/TelematicsServer.h>
#include <tc/parser/omni/Action.h>

namespace tc::server::tcp {

result_t OmniHandler::handle(const uchar* buffer, size_t size)
{
	omni::Action action;
	LG_NFO(this->logger(), "OmniHandler, {}", action.get(buffer, size));

	return RES_OK;
}

} // namespace tc::server::tcp 