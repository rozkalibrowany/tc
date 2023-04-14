#include <tc/server/tcp/handler/Omni.h>
#include <tc/server/tcp/TelematicsSession.h>
#include <tc/server/tcp/TelematicsServer.h>
#include <tc/parser/omni/packet/Payload.h>
#include <tc/parser/omni/Action.h>
#include <magic_enum.hpp>

namespace tc::server::tcp {

using namespace omni;
using namespace magic_enum;

result_t OmniHandler::handle(const uchar* buffer, size_t size)
{
	Action action;
	auto type = action.get(buffer, size);
	const std::string_view buf(reinterpret_cast<char const *>(buffer));

	// if device is not initialized yet
	if (result_t res; !iDevice && (res = initDevice(buf)) != RES_OK) {
		LG_ERR(this->logger(), "[{}] Unable to handle device, payload[{}]", enum_name(iType), buf);
		return res;
	}

	switch (type) {
		case Action::eCheckIn:
			[[fallthrough]];
		case Action::eHeartBeat:
			return handleBasicInfo(buf);
	}

	return RES_OK;
}

result_t OmniHandler::initDevice(const std::string_view& buf)
{
	Imei imei;
	if (auto res = Payload::parseImei(buf, imei); res != RES_OK) {
		LG_ERR(this->logger(), "[{}] Error parsing imei", enum_name(iType));
		return res;
	}
	LG_NFO(this->logger(), "[{}] Imei parsed OK: {}", enum_name(iType), imei);

	iDevice = std::make_unique<iot::Device>(imei); // TODO: now omitting cache size
	iImei = std::move(imei);

	return RES_OK;
}

result_t OmniHandler::handleBasicInfo(const std::string_view& buf)
{
	return RES_OK;
}

} // namespace tc::server::tcp