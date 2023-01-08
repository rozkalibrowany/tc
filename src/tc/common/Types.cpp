#include <tc/common/Types.h>

namespace tc {

Types::Method Types::str2method(const std::string_view method)
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

Types::Type Types::str2type(const std::string type)
{
  if (!type.compare("devices")) {
		return Type::eDevices;
	} else if (!type.compare("device")) {
		return Type::eDevice;
	}
	return Type::eUnknown;
}

const std::string_view Types::method2str(Method method)
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

const std::string Types::type2str(Type type)
{
  switch (type) {
    case eDevice:
    return "device";
    case eDevices:
    return "devices";
    default:
    return "unknown";
  }
}

Types::Type Types::str2req(const std::string &req)
{
	if (req.compare("devices") == 0) {
		return Type::eDevices;
	} else if (req.compare("data") == 0) {
		return Type::eDevice;
	} else {
		return Type::eUnknown;
	}
}

} // namespace tc