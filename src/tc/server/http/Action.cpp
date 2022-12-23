#include <tc/server/http/Action.h>
#include <tc/common/LockGuard.h>
#include <tc/common/Regex.h>
#include <tc/parser/Command.h>

namespace tc::server::http {

using namespace parser;

std::shared_ptr< Request > Action::get() const
{
	return std::make_shared<Request>(iRequest);
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

result_t Action::parseDevice(const Request &request)
{
	auto it = Command::sMapping.find(request.command());

	if (request.method() == Request::ePost) {
		if (!request.command().compare("set")) {
			return parseQuery(request);
		}
		if (it != Command::sMapping.end()) {
			return RES_OK;
		}
	}

	if (request.method() == Request::eGet) {
		return parseDeviceId(request);
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

result_t Action::handlePost(const Request &request)
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

	if (left.compare(Request::type2str(Request::ePackets))) {
		return RES_NOENT;
	}

	auto right = request.command().substr(request.command().find("?") + 1);
	auto key = right.substr(0, right.find("="));
	auto val = right.substr(right.find("=") + 1);

	if (key.compare("id")) {
		LG_ERR(this->logger(), "Invalid key.");
		return RES_INVARG;
	}

	// ID shoud have 6 digits
	if (!key.compare("id") && val.size() != 6) {
		LG_ERR(this->logger(), "Invalid ID format. Required: '123456'");
		return RES_INVARG;
	}

	request.iQueryParam.first = key;
	request.iQueryParam.second = val;

	return RES_OK;
}

} // namespace tc::server::http
