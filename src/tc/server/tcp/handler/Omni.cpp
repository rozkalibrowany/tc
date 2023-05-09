#include <tc/server/tcp/handler/Omni.h>
#include <tc/server/tcp/TelematicsSession.h>
#include <tc/server/tcp/TelematicsServer.h>
#include <tc/parser/omni/Action.h>
#include <tc/parser/omni/Crc.h>
#include <tc/common/Buf.h>
#include <tc/common/MagicEnum.h>

namespace tc::server::tcp {

using namespace omni;
using namespace magic_enum;
using namespace parser::omni;

result_t OmniHandler::handle(const uchar* buffer, size_t size)
{
	common::Buf buf(buffer, size);
	auto payload = std::make_shared<Payload>();

	// if device is not initialized yet, initialize
	if (result_t res; !iDevice && (res = initDevice(payload, buf)) != RES_OK) {
		LG_ERR(this->logger(), "[{}] Unable to handle device, payload[{}]", enum_name(iType), buf.asStringWithoutSuffix(1));
		return res;
	}

	// get event type (heartheat, checkin, lock status, ...)
	auto event = action::Locker::get(buf);
	if (event == action::Locker::eUnknown)
		return RES_NOENT;

	// parse received buffer with corresponding event
	if (auto res = payload->parse(buf, event); res != RES_OK) {
		LG_ERR(this->logger(), "[{}] Unable to parse [{}]", enum_name(iType), buf.asHex());
		return res;
	}

	LG_NFO(this->logger(), "[{}][{}] Parse OK[{}]", enum_name(iType), enum_name(event), buf.asStringWithoutSuffix(1));

	LG_NFO(this->logger(), "Device imei: {} cached: {}", (std::string) iDevice->imei(), iDevice->cached());


	/*auto save = iSession->server()->dbClient()->enabled();*/
	if (result_t res; (res = iSession->savePacket(payload)) != RES_OK) { // save && 
		LG_ERR(this->logger(), "[{}] Unable to save [{}]", enum_name(iType), buf.asStringWithoutSuffix(1));
		return res;
	}


	auto& last = payload->records().back();
	if (last->mandatory_ack() && sendResponse(payload) != RES_OK) {
		LG_ERR(this->logger(), "[{}] Unable to send response", enum_name(iType));
		return RES_ERROR;
	}

	iDevice->packets().push_back(payload);

	return RES_OK;
}

result_t OmniHandler::initDevice(std::shared_ptr<Payload> &payload, const common::Buf& buf)
{
	auto res = payload->parse(buf);
	if (res != RES_OK) {
		LG_ERR(this->logger(), "[{}] Error parsing header, payload[{}]", enum_name(iType), buf.asStringWithoutSuffix(1));
		return res;
	}

	auto imei = payload->imei();
	LG_NFO(this->logger(), "[{}] Imei parsed OK[{}]", enum_name(iType), imei);

	iDevice = std::make_unique<iot::Device>(imei); // TODO: now omitting cache size
	iImei = imei;

	return RES_OK;
}

result_t OmniHandler::sendResponse(std::shared_ptr<Payload> &payload)
{
	common::Buf buf;
	if (result_t res; (res = payload->getResponse(buf)) != RES_OK) {
		LG_ERR(this->logger(), "[{}] Unable to get response [{}]", enum_name(iType));
		return res;
	}

	LG_NFO(this->logger(), "[{}] Sending Response[{}]", enum_name(iType), buf.asString());
	return iSession->send(buf.cdata(), buf.size());
}

} // namespace tc::server::tcp