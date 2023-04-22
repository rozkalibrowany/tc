#include <tc/parser/RequestI.h>

namespace tc::parser {

RequestI::Type RequestI::string2type(const std::string &req)
{
  if (!req.compare("devices")) {
		return Type::eDevices;
	} else if (!req.compare("device")) {
		return Type::eDevice;
	} else if (!req.compare("packets")) {
		return Type::ePackets;
	}
	return eUnknown;
}

std::string_view RequestI::type2string(Type type)
{
	switch (type) {
		case ePackets: return "Packets";
		case eDevice: return "Device";
		case eDevices: return "Devices";
		default: return "Unknown";
	}
}

RequestI::Method RequestI::method() const
{
	return iMethod;
}

RequestI::Type RequestI::type() const
{
	return iType;
}

} // namespace tc::parser::internal