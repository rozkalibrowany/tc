#include <tc/parser/Request.h>

namespace tc::parser {

Request::Request(const std::string_view method, const std::string type)
  : iMethod(toMethod(method))
  , iType(toType(type))
{
  // nothing to do
}

Request::Method Request::toMethod(const std::string_view method)
{
	if (method.compare("GET") == 0) {
		return Method::GET;
	} else if (method.compare("POST") == 0) {
		return Method::POST;
	} else if (method.compare("DELETE") == 0) {
		return Method::DELETE;
	} else if (method.compare("HEAD") == 0) {
		return Method::HEAD;
	}

	return Method::NONE;
}

Request::Type Request::toType(const std::string type)
{
  if (type.compare("devices") == 0) {
		return Type::Devices;
	} else if (type.compare("device") == 0) {
		return Type::Device;
	} else if (type.compare("packets") == 0) {
		return Type::Packets;
	}

	return Type::Unknown;
}

std::string_view Request::fromMethod(Method method)
{
  switch (method) {
    case GET:
    return {"GET"};
    case POST:
    return {"POST"};
    case DELETE:
    return {"DELETE"};
    case HEAD:
    return {"HEAD"};
    default:
    return {"NONE"};
  }
}
std::string Request::fromType(Type type)
{
  switch (type) {
    case Device:
    return "device";
    case Devices:
    return "devices";
    default:
    return "unknown";
  }
}

Request::Method Request::method() const
{
	return iMethod;
}

Request::Type Request::type() const
{
	return iType;
}

void Request::setMethod(const std::string_view method)
{
  iMethod = toMethod(method);
}

void Request::setType(const std::string type)
{
  iType = toType(type);
}

Request::Type Request::str2req(const std::string &req)
{
	if (req.compare("devices") == 0) {
		return Type::Devices;
	} else if (req.compare("data") == 0) {
		return Type::Device;
	} else {
		return Type::Unknown;
	}
}


} // namespace tc::parser