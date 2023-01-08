#include <tc/server/http/Cache.h>
#include <json/json.h>
#include <tc/parser/Util.h>

namespace tc::server::http {

Cache::Cache(Signal<Imei, std::string, timestamp> &signal)
 : CppCommon::Singleton<Cache>()
 , iSignal(signal)
{
	// nothing to do
}

bool  Cache::hasImei(const Imei imei) const
{
	return iDevices.has(imei);
}

result_t Cache::handleAction(const Action &action, CppServer::HTTP::HTTPResponse &response)
{
	switch(action.get()->type()) {
		case Action::eDevices:
			return handleDevicesRequest(response);

		case Action::eDevice:
			if (action.get()->method() == Action::eGet) {
				if (!action.get()->command().compare("packets")) {
					LG_NFO(this->logger(), "getPackcet");

					return getPacket(action.get()->id(), response);
				} else {
					return getDevice(action.get()->id(), response);
				}
			}
			if (!action.get()->command().compare("set")) {
				return set(action.get()->id(), action.get()->query(), response);
			} else {
				return requestCommand(action.get()->id(), action.get()->command(), response);
			}
		default:
			return RES_NOIMPL;
		}

		return RES_NOENT;
}

result_t Cache::getDevice(const Imei &imei, CppServer::HTTP::HTTPResponse &response)
{
	if ((iDevices.devices().find(imei) == iDevices.devices().end())) {
		response.MakeErrorResponse(400, "Bad request");
		return RES_NOENT;
	}
	response.MakeGetResponse(iDevices.devices().at(imei)->toJson().toStyledString(), "application/json; charset=UTF-8");
	return RES_OK;
}

result_t Cache::handleDevicesRequest(CppServer::HTTP::HTTPResponse &response)
{
	response.MakeGetResponse(iDevices.toJson().toStyledString(), "application/json; charset=UTF-8");
	return RES_OK;
}

std::string Cache::getDevices()
{
	return iDevices.toJson().toStyledString();
}

result_t Cache::getPacket(const Imei &imei, CppServer::HTTP::HTTPResponse &response)
{
	if (!iDevices.has(imei)) {
		response.MakeErrorResponse(400, "Bad request");
		LG_NFO(this->logger(), "RES_NOENT");

		return RES_NOENT;
	}

	std::shared_ptr<parser::packet::PacketPayload> packet;
	if (iDevices.devices().at(imei)->last(packet) != RES_OK) {
		response.MakeErrorResponse(400, "Bad request");
		LG_NFO(this->logger(), "RES_NOENT");

		return RES_NOENT;
	}
	response.MakeGetResponse(packet->toJson().toStyledString(), "application/json; charset=UTF-8");
	return RES_OK;
}

void Cache::onReceived(const void *buffer, size_t size)
{
	if (size == 0) {
		LG_ERR(this->logger(), "Buffer is empty");
		return;
	}

	auto str = std::string((const char*) buffer, size);
	auto fromHex = tc::hex2string(str);

	if (decodeJson(fromHex) != RES_OK) {
		LG_ERR(this->logger(), "Unable to decode string");
		return;
	}

	//return updatePackets();
}

result_t Cache::decodeJson(const std::string &data)
{
	Json::Value root;
	Json::Reader reader;
	bool parsing_ok = reader.parse(data, root, false);
	if (!parsing_ok) {
		LG_ERR(this->logger(), "Unable to parse json");
		return RES_INVARG;
	}

	return iDevices.fromJson(root);
}

result_t Cache::requestCommand(const Imei imei, const std::string cmd, CppServer::HTTP::HTTPResponse &response)
{
	iSignal.emit(imei, cmd, 0);
	response.MakeOKResponse();
	return RES_OK;
}

result_t Cache::set(const Imei imei, pair< const string, const string > val, CppServer::HTTP::HTTPResponse &response)
{
	auto &devices = iDevices.devices();
	auto it = devices.find(imei);
	if (it != devices.end()) {
		if (!val.first.compare("ID") || !val.first.compare("id")) {
			(*it).second->iID = val.second;
		} else {
			response.MakeErrorResponse(400, "Bad Request");
			return RES_INVARG;
		}
	}

	response.MakeOKResponse();
	return RES_OK;
}

void Cache::updatePackets()
{
	LockGuard g(iMutex);

	for (const auto &d : iDevices.devices()) {
		if (iPacketsCounter.find(d.second->iImei) == iPacketsCounter.end()) {
			iPacketsCounter.insert(std::make_pair(d.second->iImei, d.second->iPacketsCounter));
			continue;
		}

		if (iPacketsCounter.at(d.second->iImei) < d.second->iPacketsCounter) {
			iPacketsCounter.at(d.second->iImei) = d.second->iPacketsCounter;
			iSignal.emit(d.second->iImei, Types::type2str(Action::eDevice), d.second->iTimestamp);
		}
	}
}

} // namespace tc::server::http