#include <tc/server/http/Action.h>

namespace tc::server::http {

result_t Action::parse(const CppServer::HTTP::HTTPRequest& request)
{
	if (request.empty()) {
		return RES_NOENT;
	}

	auto method = PacketRequest::toMethod(request.method());
	if (method == Method::NONE) {
		return RES_INVARG;
	}

	auto reqs = request.string();
	reqs.erase(std::remove(reqs.begin(), reqs.end(), '/'), reqs.end());

	iReq = reqs;
	iMethod = method;

	return RES_OK;
}

result_t Action::parse(const Type type, const Method method)
{
	if (type == Type::Unknown || method == Method::NONE) {
		return RES_NOENT;
	}

	auto reqs = PacketRequest::fromType(type);
	reqs.erase(std::remove(reqs.begin(), reqs.end(), '/'), reqs.end());

	iReq = reqs;
	iMethod = method;

	return RES_OK;
}

} // namespace tc::server::http
