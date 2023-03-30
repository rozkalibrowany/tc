#include <tc/server/http/CacheHandler.h>
#include <json/json.h>
#include <tc/parser/Util.h>
namespace tc::server::http {

CacheHandler::CacheHandler(std::shared_ptr<client::tcp::Client> client)
 : CppCommon::Singleton<CacheHandler>()
 , iClient(client)
{
	iClient->signal().connect([&](const void * buf, size_t size) {
		this->onReceived(buf, size);
	});
}

bool CacheHandler::hasImei(const Imei imei) const
{
	return iVehicles.has(imei);
}

result_t CacheHandler::findImei(Request &request)
{
	if (request.hasImei())
		return RES_OK;

	for (const auto& vehicle : iVehicles) {
		if (vehicle.second->id() == request.id()) {
			request.setID(vehicle.second->imei());
			return RES_OK;
		}
	}

	return RES_NOENT;
}

/**
 * It handles the action
 * @param action The action that was requested.
 * @param response The response object that will be sent back to the client.
 * @return The result of the action.
 */
result_t CacheHandler::handleAction(Request &request, CppServer::HTTP::HTTPResponse &response)
{
	switch(request.type()) {
		case Request::eDevices:
			if (request.hasQuery()) {
				std::string key;
				if (request.query(key) == RES_OK) {
					return getDevices(response, key.compare("all") == 0 ? false : true);
				}
			}
			return getDevices(response);
		
		case Request::eDevice:
			if (!request.hasImei() && findImei(request) != RES_OK) {
				response.MakeErrorResponse(400, fmt::format("Unable to map ID[{}] into Imei", request.id()));
				LG_NFO(this->logger(), "Unable to map ID[{}] into Imei", request.id());
				return RES_NOENT;
			}

			if (request.method() == Request::eGet) {
				return getDevice(request, response);
			}
			if (!request.key().compare("set")) {
				return set(request, response);
			} else {
				return addCommand(request, response);
			}
		
		case Request::ePackets: 
			return getPackets(response);
		
		case Request::eUnknown:
			return RES_NOENT;
	}

	return RES_NOENT;
}

iot::Devices<iot::Vehicle> &CacheHandler::vehicles()
{
	return iVehicles;
}

Signal<Imei> &CacheHandler::signal()
{
	return iSignal;
}

/**
 * It checks if the device exists in the cachehandler, and if it does, it returns it in the response
 * @param imei The IMEI of the device to get.
 * @param response The response object that will be sent back to the client.
 * @return A device object.
 */
result_t CacheHandler::getDevice(const Request &request, CppServer::HTTP::HTTPResponse &response)
{
	if ((iVehicles.devices().find(request.id()) == iVehicles.devices().end())) {
		response.MakeErrorResponse(400, fmt::format("Device {} not found", request.id()));
		return RES_NOENT;
	}
	if (!request.key().compare("packet")) {
		const auto& vehicle = iVehicles.devices().at(request.id());
		if (vehicle == nullptr || vehicle->packets().empty()) {
			response.MakeErrorResponse(400, fmt::format("Packet data empty for {}", request.id()));
			return RES_NOENT;
		}
		Json::Value packet;
		if(getPacket(vehicle, packet) != RES_OK) {
			response.MakeErrorResponse(400, fmt::format("Error getting packet for {}", request.id()));
			return RES_NOENT;
		}
		response.MakeGetResponse(packet.toStyledString(), "application/json; charset=UTF-8");
	} else {
		response.MakeGetResponse(iVehicles.devices().at(request.id())->toJson().toStyledString(), "application/json; charset=UTF-8");
	}
	return RES_OK;
}

/**
 * It gets devices as JSON, and returns an HTTP response
 * @param response The HTTP response object.
 * @return A JSON string containing the list of devices.
 */
result_t CacheHandler::getDevices(CppServer::HTTP::HTTPResponse &response, bool active_only)
{
	if (iVehicles.devices().empty()) {
		response.MakeErrorResponse(500, "Internal Server Error");
		return RES_NOENT;
	}
	response.MakeGetResponse(iVehicles.toJson(active_only).toStyledString(), "application/json; charset=UTF-8");
	return RES_OK;
}

result_t CacheHandler::getDevices(std::string &devices, bool active_only)
{
	devices = iVehicles.toJson(active_only).toStyledString();
	return RES_OK;
}

result_t CacheHandler::getPackets(CppServer::HTTP::HTTPResponse &response)
{
	if (iVehicles.size() == 0) {
		response.MakeErrorResponse(500, "Vehicle list empty");
		return RES_NOENT;
	}

	Json::Value packets = Json::arrayValue;
	for (auto &[imei, vehicle] : iVehicles.devices()) {
		if (vehicle == nullptr || vehicle->packets().empty())
			continue;

		Json::Value val;
		if (getPacket(vehicle, val) != RES_OK) {
			response.MakeErrorResponse(500, "Internal Server Error");
			return RES_ERROR;
		}
		packets.append(val);
	}

	response.MakeGetResponse(packets.toStyledString(), "application/json; charset=UTF-8");
	return RES_OK;
}

result_t CacheHandler::getPacket(const std::shared_ptr< iot::Vehicle > vehicle, Json::Value &rhs)
{

	std::lock_guard lock(iMutex);
	auto payload_packet = vehicle->packets().back();

	try {
		auto json = payload_packet->toJsonValue();
		for (const auto& records : json["Records"]) {
			const auto &rec = records["Record"][0];
			if (rec.isMember("Header")) {
				rhs["Priority"] = rec["Header"]["Priority"];
				rhs["Timestamp"] = rec["Header"]["Timestamp"];
			}
			if (rec.isMember("GPS")) {
				Json::Value gps;
				gps["Longitude"] = rec["GPS"]["Longitude"];
				gps["Latitude"] = rec["GPS"]["Latitude"];
				gps["Altitude"] = rec["GPS"]["Altitude"];
				gps["Angle"] = rec["GPS"]["Angle"];
				gps["Satellites"] = rec["GPS"]["Satellites"];
				gps["Speed"] = rec["GPS"]["Speed"];
				rhs["GPS"] = gps;
				lastLocation = gps;
			} else if (!lastLocation.empty()) {
				Json::Value gps;
				gps["Longitude"] = lastLocation["GPS"]["Longitude"];
				gps["Latitude"] = lastLocation["GPS"]["Latitude"];
				gps["Altitude"] = lastLocation["GPS"]["Altitude"];
				gps["Angle"] = lastLocation["GPS"]["Angle"];
				gps["Satellites"] = lastLocation["GPS"]["Satellites"];
				gps["Speed"] = lastLocation["GPS"]["Speed"];
			}

			if (rec.isMember("IoRecords")) {
				for (const auto& io : rec["IoRecords"]) {
					if (io["ID"].asInt() == 113) 
						rhs["Battery_capacity"] = io["value"].asInt();
					else if (io["ID"].asInt() == 16)
						 rhs["Odometer"] = io["value"].asInt();
					else if (io["ID"].asInt() == 341)
						 rhs["Error"] = io["value"].asInt();
					else if (io["ID"].asInt() == 344)
						 rhs["Lock"] = io["value"].asInt();
					else if (io["ID"].asInt() == 349)
						 rhs["Speed_mode"] = io["value"].asInt();
					else if (io["ID"].asInt() == 352)
						 rhs["Battery_percentage"] = io["value"].asInt();
					else if (io["ID"].asInt() == 353)
						 rhs["Actual_remaining_mileage"] = io["value"].asInt();
					else if (io["ID"].asInt() == 354)
						 rhs["Predicted_remaining_mileage"] = io["value"].asInt();
					else if (io["ID"].asInt() == 355)
						 rhs["Speed"] = io["value"].asInt();
					else if (io["ID"].asInt() == 356)
						 rhs["Total_mileage"] = io["value"].asInt();
					else if (io["ID"].asInt() == 352)
						 rhs["Scooter_battery"] = io["value"].asInt();
				}
			}
		}
	} catch (...) {
		LG_NFO(this->logger(), "Unable to serialize packet JSON for vehicle[{}]", vehicle->imei());
		return RES_NOENT;
	}

	rhs["Imei"] = vehicle->imei();
	rhs["ID"] = vehicle->id();

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

	if (decodeJson(tc::hex2string(std::string{(const char*) buffer, size})) != RES_OK) {
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
	if (!reader.parse(data, root, false)) {
		LG_ERR(this->logger(), "Exception caught while decding JSON");
		return RES_INVARG;
	}

	if (root.isMember("devices"))
		return iVehicles.fromJson(root, true);
	else if (root.isMember("packets"))
		return iVehicles.fromJsonPacket(root);

	return RES_NOENT;
}

result_t CacheHandler::addCommand(const Request &request, CppServer::HTTP::HTTPResponse &response)
{
	auto vehicle = iVehicles.find(request.id());
	if (vehicle == iVehicles.end()) {
		LG_ERR(this->logger(), "Vehicle not found");
		response.MakeErrorResponse(400, fmt::format("Device with ID: {} not found", request.id()));
		return RES_NOENT;
	}

	if (!vehicle->second->online()) {
		LG_ERR(this->logger(), "Vehicle not online");
		response.MakeErrorResponse(400, fmt::format("Device with ID: {} is not online", request.id()));
		return RES_NOENT;
	}

	if (iClient->send(request.id(), request.command()) != RES_OK) {
		LG_ERR(this->logger(), "Unable to send command.");
		response.MakeErrorResponse(400, "Internal Server Error");
		return RES_NOENT;
	}
	response.MakeOKResponse();
	return RES_OK;
}

/**
 * It sets the value of a device's ID or fleet
 * @param request The request object that contains the request information.
 * @param response The response object that will be sent back to the client.
 * @return A result code.
 */
result_t CacheHandler::set(Request &request, CppServer::HTTP::HTTPResponse &response)
{
	auto &vehicles = iVehicles.devices();
	auto it = vehicles.find(request.id());
	if (it != vehicles.end()) {
		std::string key, val;
		if (request.query(key, val) == RES_OK) {
			if (!key.compare("id"))
				it->second->setID(val);
			if (!key.compare("fleet"))
				it->second->setFleet(val);
			iSignal.emit(it->first);
		}
	} else {
		response.MakeErrorResponse(400, "Bad Request");
		return RES_INVARG;
	}

	response.MakeOKResponse();
	return RES_OK;
}

} // namespace tc::server::http