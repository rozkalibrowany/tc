#include <tc/server/http/Request.h>
#include <tc/common/Regex.h>

namespace tc::server::http {

Request::Request(const CppServer::HTTP::HTTPRequest& request)
  : iRequest(std::move(request))
{
  // nothing to do
}

Request::Method Request::str2method(const std::string_view method)
{
	if (method.compare("GET") == 0) {
		return Method::eGet;
	} else if (method.compare("POST") == 0) {
		return Method::ePost;
	} else if (method.compare("DELETE") == 0) {
		return Method::eDelete;
	} else if (method.compare("HEAD") == 0) {
		return Method::eHead;
	}

	return Method::eNone;
}

Request::Type Request::str2type(const std::string type)
{
  if (type.compare("devices") == 0) {
		return Type::eDevices;
	} else if (type.compare("device") == 0) {
		return Type::eDevice;
	} else if (type.compare("packets") == 0) {
		return Type::ePackets;
	}

	return Type::eUnknown;
}

const std::string_view Request::method2str(Method method)
{
  switch (method) {
    case eGet:
    return {"GET"};
    case ePost:
    return {"POST"};
    case eDelete:
    return {"DELETE"};
    case eHead:
    return {"HEAD"};
    default:
    return {"NONE"};
  }
}
const std::string Request::type2str(Type type)
{
  switch (type) {
    case eDevice:
    return "device";
    case eDevices:
    return "devices";
		case ePackets:
    return "packets";
    default:
    return "unknown";
  }
}

Request::Method Request::method() const
{
	return str2method(iRequest.method());
}

Request::Type Request::type() const
{
	return str2type(regex(std::regex{"\\/(.*?)\\/"}, iRequest.url()));
}

const std::string Request::id() const
{
	return regex(std::regex{"\\/([0-9]+)\\/"}, iRequest.url());
}

const std::string Request::command() const
{
	return tc::regex(std::regex{"([^\\/]+$)"}, iRequest.url());
}

Request::Type Request::str2req(const std::string &req)
{
	if (req.compare("devices") == 0) {
		return Type::eDevices;
	} else if (req.compare("data") == 0) {
		return Type::eDevice;
	} else {
		return Type::eUnknown;
	}
}


} // namespace tc::server::http