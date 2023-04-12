#include <tc/server/tcp/handler/Omni.h>
#include <tc/server/tcp/TelematicsSession.h>
#include <tc/server/tcp/TelematicsServer.h>

namespace tc::server::tcp {

result_t OmniHandler::handle(const uchar* buffer, size_t size)
{
	LG_NFO(this->logger(), "OmniHandler::handle[{}]", size);
	return RES_NOENT;
}

} // namespace tc::server::tcp 