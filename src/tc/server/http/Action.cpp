#include <tc/server/http/Action.h>
#include <tc/common/LockGuard.h>
#include <tc/common/Regex.h>

namespace tc::server::http {

using namespace parser;

std::shared_ptr< Request > Action::get() const
{
	return std::make_shared<Request>(iRequest);
}

bool Action::hasQuery(const Request &request)
{
	return request.command().find({"?"}) != request.command().npos;
}

result_t Action::parse(const CppServer::HTTP::HTTPRequest& request)
{
	result_t res = RES_OK;
	if (request.empty()) {
		return RES_NOENT;
	}

	Request req(request);

	switch (req.method()) {
		case Request::eGet:
			res = handleGet(req);
			break;
		case Request::ePost:
			res = handlePost(req);
			break;
		default:
			res = RES_NOIMPL;
	}

	iRequest = std::move(req);

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
	if (parseDeviceId(request) != RES_OK) {
		return RES_NOENT;
	}

	return parseCommand(request);
}

result_t Action::parseDevices(const Request &request)
{
	if (request.method() != Request::eGet) {
		return RES_NOENT;
	}

	return RES_OK;
}

result_t Action::parseQuery(const Request &request)
{
	auto left = request.command().substr(0, request.command().find({"?"}));

	if (left.compare("packets") && left.compare("set")) {
		return RES_NOENT;
	}

	auto right = request.command().substr(request.command().find("?") + 1);
	auto key = right.substr(0, right.find("="));
	auto val = right.substr(right.find("=") + 1);

	if (key.compare("id") && key.compare("begin") && key.compare("end")) {
		LG_ERR(this->logger(), "Invalid key.");
		return RES_INVARG;
	}

	// ID shoud have 6 digits
	if (!key.compare("id") && val.size() != 6) {
		LG_ERR(this->logger(), "Invalid ID format. Required: '123456'");
		return RES_INVARG;
	}
	return RES_OK;
}

result_t Action::parseCommand(const Request &request)
{
	if (request.method() == Request::eGet && !request.command().compare("set")) {
		return RES_NOENT;
	}

	if (request.method() == Request::ePost && !request.command().compare("packets")) {
		return RES_NOENT;
	}

	if (hasQuery(request) == true) {
		return parseQuery(request);
	}

	return RES_OK;
}

result_t Action::parseDeviceId(const Request &request)
{
	if (request.id().length() != 6 && request.id().length() != 15) { // IMEI_LENGTH
		return RES_INVARG;
	}

	return RES_OK;
}

} // namespace tc::server::http
