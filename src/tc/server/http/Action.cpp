#include <tc/server/http/Action.h>
#include <tc/common/LockGuard.h>
#include <tc/common/Regex.h>
#include <tc/parser/Command.h>

namespace tc::server::http {

using namespace parser;

result_t Action::parse(const CppServer::HTTP::HTTPRequest& request)
{
	if (request.empty()) {
		return RES_NOENT;
	}

	auto method = toMethod(request.method());
	if (method == Method::NONE) {
		return RES_INVARG;
	}

	auto type = toType(regex(std::regex{"\\/(.*?)\\/"}, request.url()));
	auto id = regex(std::regex{"\\/([0-9]+)\\/"}, request.url());
	auto action = tc::regex(std::regex{"([^\\/]+$)"}, request.url());

	if (type == Unknown || id.empty() || action.empty()) {
		return RES_INVARG;
	}

	{
		iMethod = method;
		iID = id;

		LockGuard guard(iMutex);
		if (Command::sMapping.find(action) == Command::sMapping.end()) {
			return parse_internal_command(action);
		} else {
			iAction = action;
			iType = type;
		}
	}

	return RES_OK;
}

result_t Action::parse(const Type type, const Method method)
{
	if (type == Type::Unknown || method == Method::NONE) {
		return RES_NOENT;
	}

	auto reqs = fromType(type);
	reqs.erase(std::remove(reqs.begin(), reqs.end(), '/'), reqs.end());

	iReq = reqs;
	iMethod = method;

	return RES_OK;
}

result_t Action::parse_internal_command(const std::string &action)
{
	auto left = action.substr(0, action.find({"?"}));

	if (left.compare("set")) {
			LG_ERR(this->logger(), "Invalid action");
		return RES_NOENT;
	}

	auto right = action.substr(action.find("?") + 1);
	auto key = right.substr(0, right.find("="));
	auto val = right.substr(right.find("=") + 1);

	if (key.compare("id") && key.compare("imei")) {
		LG_ERR(this->logger(), "Invalid key.");
		return RES_INVARG;
	}

	// ID shoud have 6 digits
	if (!key.compare("id") && val.size() != 6) {
		LG_ERR(this->logger(), "Invalid ID format.");
		return RES_INVARG;
	}

	iQueryParam.first = key;
	iQueryParam.second = val;
	iAction = left;
	iType = Set;

	return RES_OK;
}

} // namespace tc::server::http
