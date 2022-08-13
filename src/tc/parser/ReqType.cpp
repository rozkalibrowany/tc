#include <tc/parser/ReqType.h>

namespace tc::parser {

ReqType::ReqType(const std::string_view method, const std::string type)
  : iMethod(toMethod(method))
  , iType(toType(type))
{
  // nothing to do
}

ReqType::Method ReqType::toMethod(const std::string_view method)
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

ReqType::Type ReqType::toType(const std::string type)
{
  if (type.compare("devices") == 0) {
		return Type::Devices;
	} else if (type.compare("device") == 0) {
		return Type::Device;
	}

	return Type::Unknown;
}

std::string_view ReqType::fromMethod(Method method)
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
std::string ReqType::fromType(Type type)
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

ReqType::Method ReqType::method() const
{
  return iMethod;
}

ReqType::Type ReqType::type() const
{
  return iType;
}

void ReqType::setMethod(const std::string_view method)
{
  iMethod = toMethod(method);
}

void ReqType::setType(const std::string type)
{
  iType = toType(type);
}

const std::string ReqType::req2str(const Type type)
{
	switch(type) {
		case Type::Device:
			return "data";
		case Type::Devices:
			return "devices";
		default: return "unknown";
	}
}

ReqType::Type ReqType::str2req(const std::string &req)
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