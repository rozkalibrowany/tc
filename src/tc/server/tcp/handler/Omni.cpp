#include <tc/server/tcp/handler/Omni.h>
#include <tc/server/tcp/TelematicsSession.h>
#include <tc/server/tcp/TelematicsServer.h>
#include <tc/parser/omni/packet/Payload.h>
#include <tc/parser/omni/Action.h>
#include <tc/parser/omni/records/Unlock.h>
#include <tc/parser/omni/records/CheckIn.h>
#include <tc/parser/omni/records/Heartbeat.h>
#include <magic_enum.hpp>

namespace tc::server::tcp {

using namespace omni;
using namespace magic_enum;

result_t OmniHandler::handle(const uchar* buffer, size_t size)
{
	common::Buf buf(buffer, size);
	auto type = action::Locker::get(buf);

	// if device is not initialized yet
	if (result_t res; !iDevice && (res = initDevice(buf)) != RES_OK) {
		LG_ERR(this->logger(), "[{}] Unable to handle device, payload[{}]", enum_name(iType), tc::uchar2string(buf.data(), buf.size()));
		return res;
	}

	switch (type) {
		case action::Locker::eCheckIn:
			return handleCheckIn(buf);
		case action::Locker::eHeartBeat:
			return handleHeartbeat(buf);
		case action::Locker::eLockReport:
			return handleLockReport(buf);
		case action::Locker::eUnlock:
			return handleUnlock(buf);
		case action::Locker::eAlarm:
		case action::Locker::eUpgradeData:
		case action::Locker::eUpgradeDetection:
		case action::Locker::eUpgradeResults:
		case action::Locker::eUnknown:
			return RES_NOIMPL;
		}

	return RES_OK;
}

result_t OmniHandler::initDevice(const common::Buf& buf)
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

// get voltage or/and lock status, according to Omni protcol
result_t OmniHandler::handleCheckIn(const common::Buf& buf)
{
	records::CheckIn checkin(buf);
	if (checkin.parse() != RES_OK)
		return RES_NOENT;

	return RES_OK;
}

result_t OmniHandler::handleHeartbeat(const common::Buf& buf)
{
	records::Heartbeat heartbeat(buf);
	if (heartbeat.parse() != RES_OK)
		return RES_NOENT;

	return RES_OK;
}

result_t OmniHandler::handleLockReport(const common::Buf& buf)
{

	return RES_NOIMPL;
}

result_t OmniHandler::handleUnlock(const common::Buf& buf)
{
	records::Unlock unlock(buf);
	if (unlock.parse() != RES_OK)
		return RES_NOENT;

	common::Buf response;
	unlock.response(response);

	iSession->send(response.data(), response.size(), true);

	return RES_OK;
}

} // namespace tc::server::tcp