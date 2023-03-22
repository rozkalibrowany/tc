#include <tc/server/http/CacheHandler.h>
#include <json/json.h>
#include <tc/parser/Util.h>
namespace tc::server::http {

CacheHandler::CacheHandler(Signal<Imei, std::string> &signal, Signal<Imei> &signal_modified)
 : CppCommon::Singleton<CacheHandler>()
 , iSignal(signal)
 , iSignalModified(signal_modified)
{
	// nothing to do
}

bool CacheHandler::hasImei(const Imei imei) const
{
	return iDevices.has(imei);
}

/**
 * It handles the action
 * @param action The action that was requested.
 * @param response The response object that will be sent back to the client.
 * @return The result of the action.
 */
result_t CacheHandler::handleAction(const Action &action, CppServer::HTTP::HTTPResponse &response)
{
	switch(action.get()->type()) {
		case Request::eDevices: {
			if (action.get()->hasQuery()) {
				std::string key;
				if (action.get()->query(key) == RES_OK) {
					return getDevices(response, key.compare("all") == 0 ? false : true);
				}
			}
			return getDevices(response);
		}
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
			LG_NFO(this->logger(), "ePackets");
			return RES_NOIMPL;
		}

		return RES_NOENT;
}

iot::Devices<iot::Vehicle> &CacheHandler::devices()
{
	return iDevices;
}

/**
 * It checks if the device exists in the cachehandler, and if it does, it returns it in the response
 * @param imei The IMEI of the device to get.
 * @param response The response object that will be sent back to the client.
 * @return A device object.
 */
result_t CacheHandler::getDevice(const Imei &imei, CppServer::HTTP::HTTPResponse &response)
{
	if ((iDevices.devices().find(imei) == iDevices.devices().end())) {
		response.MakeErrorResponse(400, "Bad request");
		return RES_NOENT;
	}
	response.MakeGetResponse(iDevices.devices().at(imei)->toJson().toStyledString(), "application/json; charset=UTF-8");
	return RES_OK;
}

/**
 * It gets devices as JSON, and returns an HTTP response
 * @param response The HTTP response object.
 * @return A JSON string containing the list of devices.
 */
result_t CacheHandler::getDevices(CppServer::HTTP::HTTPResponse &response, bool active_only)
{

	if (iDevices.devices().empty()) {
		response.MakeErrorResponse(500, "Internal Server Error");
		return RES_NOENT;
	}
	response.MakeGetResponse(iDevices.toJson(active_only).toStyledString(), "application/json; charset=UTF-8");
	return RES_OK;
}

result_t CacheHandler::getDevices(std::string &devices, bool active_only)
{
	devices = iDevices.toJson(active_only).toStyledString();
	return RES_OK;
}

/**
 * It receives a buffer, converts it to a string, decodes the string from hex, and then decodes the JSON
 * @param buffer The buffer containing the data received from the server.
 * @param size The size of the buffer.
 * @return The return value is the result of the decodeJson function.
 */
void CacheHandler::onReceived(const void *buffer, size_t size)
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

/**
 * It takes a string of JSON data, parses it, and then calls the fromJson() function
 * @param data The JSON string to parse
 * @return A result_t
 */
result_t CacheHandler::decodeJson(const std::string &data)
{
	Json::Value root;
	Json::Reader reader;
	bool parsing_ok = reader.parse(data, root, false);
	if (!parsing_ok) {
		LG_ERR(this->logger(), "Unable to parse json");
		return RES_INVARG;
	}

	return iDevices.fromJson(root, true);
}

result_t CacheHandler::addCommand(const Imei imei, const std::string cmd, CppServer::HTTP::HTTPResponse &response)
{
	if (imei.length() < IMEI_LENGTH) {
		for (const auto& device : iDevices) {
			if (device.second->id() == imei) {
				iSignal.emit(device.second->imei(), cmd);
				response.MakeOKResponse();
				return RES_OK;
			}
		}
	}

	if (imei.length() < IMEI_LENGTH) {
		response.MakeErrorResponse(400, fmt::format("Device with ID: {} not found", imei));
		return RES_NOENT;
	}

	iSignal.emit(imei, cmd);
	response.MakeOKResponse();
	return RES_OK;
}

/**
 * It sets the value of a device's ID or fleet
 * @param request The request object that contains the request information.
 * @param response The response object that will be sent back to the client.
 * @return A result code.
 */
result_t CacheHandler::set(std::shared_ptr< Request > request, CppServer::HTTP::HTTPResponse &response)
{
	auto &devices = iDevices.devices();
	auto it = devices.find(request->id());
	if (it != devices.end()) {
		std::string key, val;
		if (request->query(key, val) == RES_OK) {
			if (!key.compare("id"))
				it->second->setID(val);
			if (!key.compare("fleet"))
				it->second->setFleet(val);
			iSignalModified.emit(it->first);
		}
	} else {
		response.MakeErrorResponse(400, "Bad Request");
		return RES_INVARG;
	}

	response.MakeOKResponse();
	return RES_OK;
}

} // namespace tc::server::http