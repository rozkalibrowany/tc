#include <tc/server/http/Action.h>
#include <tc/parser/teltonika/Command.h>
#include <tc/parser/omni/Command.h>

namespace tc::server::http {

using namespace parser;

result_t Action::parse(const Request &request)
{
	result_t res = RES_OK;
	if (request.request().empty()) {
		return RES_NOENT;
	}

	switch (request.method()) {
		case Request::eGet:
			res = handleGet(request);
			break;
		case Request::ePost:
			res = handlePost(request);
			break;
		default:
			res = RES_NOIMPL;
	}

	return res;
}

result_t Action::handleGet(const Request &request)
{
	if (request.type() == Request::eDevice) {
		return parseDevice(request);
	}

	if (request.type() == Request::eDevices) {
		return parseDevices(request);
	}

	return RES_INVARG;
}


result_t Action::handlePost(const Request &request)
{
	if (request.type() == Request::eDevices) {
		return RES_NOENT;
	}

	if (request.type() == Request::eDevice) {
		return parseDevice(request);
	}

	return RES_NOENT;
}

result_t Action::parseDevice(const Request &request)
{
	if (request.method() == Request::ePost) {
		auto teltonika = teltonika::Command::sMapping.find(request.command());
		auto omni = omni::Command::sMapping.find(request.command());
		if (teltonika == teltonika::Command::sMapping.end() && omni == omni::Command::sMapping.end()) {
			return RES_NOENT;
		}

		if (request.command().find("?") != std::string::npos) {
			return parseQuery(request);
		}

		return RES_OK;
	}

	if (request.method() == Request::eGet) {
		if(parseDeviceId(request) == RES_OK && request.command().empty()) {
			return RES_OK;
		}

		if (!request.key().empty() && !request.key().compare("packets")) {
			return RES_OK;
		}
	}

	return RES_NOENT;
}

result_t Action::parseDevices(const Request &request)
{
	if (request.method() != Request::eGet) {
		return RES_NOENT;
	}

	return RES_OK;
}

result_t Action::parseCommand(const Request &request)
{
	return RES_NOIMPL;
}

result_t Action::parseDeviceId(const Request &request)
{
	if (request.id().length() != 6 && request.id().length() != 15) { // IMEI_LENGTH
		return RES_INVARG;
	}

	return RES_OK;
}

result_t Action::parseQuery(const Request &request)
{
	auto left = request.command().substr(0, request.command().find({"?"}));
	if (left.empty() || (left.compare("set") && left.compare("fleet"))) {
		return RES_NOENT;
	}

	auto right = request.command().substr(request.command().find("?") + 1);
	if (right.empty() || right.find("=") == std::string::npos) {
		return RES_NOENT;
	}

	auto id = right.substr(0, right.find("="));
	auto cmp_id = id.compare("id");
	auto id_val = right.substr(right.find("=") + 1);

	auto fleet = right.substr(0, right.find("="));
	auto cmp_fleet = fleet.compare("fleet");
	auto fleet_val = right.substr(right.find("=") + 1);

	if (cmp_id && cmp_fleet) {
		return RES_NOENT;
	}

	if (!cmp_id && id_val.length() != 6) {
		return RES_NOENT;
	}

	if (!cmp_fleet && fleet_val.length() > 50) {
		return RES_NOENT;
	}

	return RES_OK;
}

} // namespace tc::server::http
