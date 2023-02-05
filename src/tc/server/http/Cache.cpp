#include <tc/server/http/Cache.h>
#include <json/json.h>
#include <tc/parser/Util.h>
namespace tc::server::http {

Cache::Cache(Signal<Imei, std::string> &signal)
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
		case Request::eDevices:
			return getDevices(response);

		case Request::eDevice:
			if (action.get()->method() == Request::eGet) {
				return getDevice(action.get()->id(), response);
			}
			if (!action.get()->key().compare("set")) {
				return set(action.get(), response);
			} else {
				return addCommand(action.get()->id(), action.get()->command(), response);
			}

		case Request::ePackets:
		default:
			return RES_NOIMPL;
		}

		return RES_NOENT;
}

iot::Devices<iot::Vehicle> &Cache::devices()
{
	return iDevices;
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

result_t Cache::getDevices(CppServer::HTTP::HTTPResponse &response)
{
	response.MakeGetResponse(iDevices.toJson().toStyledString(), "application/json; charset=UTF-8");
	return RES_OK;
}

std::string Cache::getDevices()
{
	return iDevices.toJson().toStyledString();
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

result_t Cache::addCommand(const Imei imei, const std::string cmd, CppServer::HTTP::HTTPResponse &response)
{
	iSignal.emit(imei, cmd);
	response.MakeOKResponse();
	return RES_OK;
}

result_t Cache::set(std::shared_ptr< Request > request, CppServer::HTTP::HTTPResponse &response)
{
	auto &devices = iDevices.devices();
	auto it = devices.find(request->id());
	if (it != devices.end()) {
		std::string val;
		if (request->query(val) == RES_OK) {
			(*it).second->setID(val);
		} else {
			response.MakeErrorResponse(400, "Bad Request");
			return RES_INVARG;
		}
	}

	response.MakeOKResponse();
	return RES_OK;
}

} // namespace tc::server::http