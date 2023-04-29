#include <tc/server/tcp/handler/Omni.h>
#include <tc/server/tcp/TelematicsSession.h>
#include <tc/server/tcp/TelematicsServer.h>
#include <tc/parser/omni/Action.h>
#include <tc/parser/omni/Crc.h>
#include <tc/common/Buf.h>
#include <magic_enum.hpp>

namespace tc::server::tcp {

using namespace omni;
using namespace magic_enum;
using namespace parser::omni;

result_t OmniHandler::handle(const uchar* buffer, size_t size)
{
	common::Buf buf(buffer, size);
	auto payload = std::make_shared<Payload>();
	// if device is not initialized yet
	if (result_t res; !iDevice && (res = initDevice(payload, buf)) != RES_OK) {
		LG_ERR(this->logger(), "[{}] Unable to handle device, payload[{}]", enum_name(iType), tc::uchar2string(buf.data(), buf.size()));
		return res;
	}

	auto event = action::Locker::get(buf);
	if (event == action::Locker::eUnknown)
		return RES_NOENT;

	if (auto res = payload->parse(buf, event); res != RES_OK) {
		LG_ERR(this->logger(), "[{}] Unable to parse [{}]", enum_name(iType), tc::uchar2string(buf.data(), buf.size()));
		return res;
	}

	/*auto save = iSession->server()->dbClient()->enabled();
	if (result_t res; save && (res = iSession->savePacket(payload)) != RES_OK) {
		LG_ERR(this->logger(), "[{}] Unable to save [{}]", enum_name(iType), tc::uchar2string(buf.data(), buf.size()));
		return res;
	}*/
	auto& last = payload->records().back();
	if (last->mandatory_ack() && sendResponse(payload) != RES_OK) {
		LG_ERR(this->logger(), "[{}] Unable to send response [{}]", enum_name(iType), tc::uchar2string(buf.data(), buf.size()));
		return RES_ERROR;
	}

	iDevice->packets().push_back(payload);

	return RES_OK;
}

result_t OmniHandler::initDevice(std::shared_ptr<Payload> &payload, const common::Buf& buf)
{
	std::string answer = "*CMDS,OM,862205055040770,000000000000,D1,60#";
	unsigned char first[2];
	first[0] = 0xFF;
	first[1] = 0xFF;
	unsigned char second[answer.length()];
	memcpy(second, answer.data(), answer.length());

	unsigned char out[answer.length() + 2];
	memcpy(out, first, 2);
	memcpy(out + 2, second, answer.length());

	iSession->send(out, answer.length() + 2, true);

	auto res = payload->parse(buf);
	if (res != RES_OK) {
		LG_ERR(this->logger(), "[{}] Error parsing header", enum_name(iType));
		return res;
	}

	auto imei = payload->imei();
	LG_NFO(this->logger(), "[{}] Imei parsed OK: {}", enum_name(iType), imei);

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

	std::string hex = tc::uchar2string(buf.cdata(), buf.size());
	std::string hexAsText;
	for(int i=0; i < hex.length(); i+=2)
	{
			std::string byte = hex.substr(i,2);
			char chr = (char) (int)strtol(byte.c_str(), NULL, 16);
			hexAsText.push_back(chr);
	}

	LG_NFO(this->logger(), "[{}] Sending Response: {}", enum_name(iType), hexAsText);

	return iSession->send(buf.cdata(), buf.size());
}

/*


*CMDS,OM,123456789123456,200318123020,D1,60#

// get voltage or/and lock status, according to Omni protcol
result_t OmniHandler::handleCheckIn(const common::Buf& buf)
{
	records::CheckIn checkin;
	if (checkin.parse(buf) != RES_OK)
		return RES_NOENT;

	return RES_OK;
}

result_t OmniHandler::handleHeartbeat(const common::Buf& buf)
{
	records::Heartbeat heartbeat;
	if (heartbeat.parse(buf) != RES_OK)
		return RES_NOENT;

	return RES_OK;
}

result_t OmniHandler::handleLockReport(const common::Buf& buf)
{

	return RES_NOIMPL;
}

result_t OmniHandler::handleUnlock(const common::Buf& buf)
{
	records::Unlock unlock;
	if (unlock.parse(buf) != RES_OK)
		return RES_NOENT;

	common::Buf response;
	unlock.response(response);

	iSession->send(response.data(), response.size(), true);

	return RES_OK;
}

result_t OmniHandler::handlePositioning(const common::Buf& buf)
{
	records::Positioning positioning;
	if (positioning.parse(buf) != RES_OK)
		return RES_NOENT;

	common::Buf response;
	positioning.response(response);

	iSession->send(response.data(), response.size(), true);

	std::string hex = tc::uchar2string((const uchar*) response.data(), response.size());
	std::string hexAsText;
	for(int i=0; i < hex.length(); i+=2)
	{
			std::string byte = hex.substr(i,2);
			char chr = (char) (int)strtol(byte.c_str(), NULL, 16);
			hexAsText.push_back(chr);
	}

	LG_NFO(this->logger(), "Sent on positioning: {}", hexAsText);

	return RES_OK;
}

result_t OmniHandler::handleUpgradeDetection(const common::Buf& buf)
{

	std::string answer = "*CMDS,OM,862205055040770,000000000000,U0,220,128,{},A1,C7qn#\n";
	unsigned char first[2];
	first[0] = 0xFF;
	first[1] = 0xFF;
	unsigned char second[answer.length()];
	memcpy(second, answer.data(), answer.length());

	unsigned char out[answer.length() + 2];
	memcpy(out, first, 2);
	memcpy(out + 2, second, answer.length());


	std::string hex = tc::uchar2string(out, answer.length() + 2);
	std::string hexAsText;
	for(int i=0; i < hex.length(); i+=2)
	{
			std::string byte = hex.substr(i,2);
			char chr = (char) (int)strtol(byte.c_str(), NULL, 16);
			hexAsText.push_back(chr);
	}

	LG_NFO(this->logger(), "[{}] Sending handleUpgradeDetection: {}", enum_name(iType), hexAsText);

	iSession->send(out, answer.length() + 2, true);

	return RES_OK;
}*/

} // namespace tc::server::tcp