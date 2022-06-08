#include <tc/parser/packet/PacketRequest.h>
#include <algorithm>

namespace tc::parser {

size_t PacketRequest::REQ_MIN_SIZE = 11;

PacketRequest::Method PacketRequest::toMethod(const std::string_view method)
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

std::string PacketRequest::fromMethod(const Method method)
{
	switch(method) {
		case Method::GET:
			return "GET";
		case Method::POST:
			return "POST";
		case Method::DELETE:
			return "DELETE";
		case Method::HEAD:
			return "HEAD";
		case Method::NONE:
		default:
			return "None";
	}
}

std::string PacketRequest::fromType(const Type type)
{
	switch (type) {
		case Type::Devices:
			return "/devices";
		case Type::Data:
			return "/data";
		case Type::Unknown:
		default:
			return "unknown";
	}
}

bool PacketRequest::hasRequest(const uchar* buf, size_t size)
{
	bool isRequest = false;
	if (size < PacketRequest::REQ_MIN_SIZE) {
		return isRequest;
	}

	isRequest |= contains(buf, size, (uchar) TYPE_PACKET_REQUEST);

	return isRequest;
}

const std::string PacketRequest::req2str(const Type type)
{
	switch(type) {
		case Type::Data:
			return "data";
		case Type::Devices:
			return "devices";
		default: return "unknown";
	}
}

PacketRequest::Type PacketRequest::str2req(const std::string &req)
{
	if (req.compare("devices") == 0) {
		return Type::Devices;
	} else if (req.compare("data") == 0) {
		return Type::Data;
	} else {
		return Type::Unknown;
	}
}

bool PacketRequest::contains(const uchar* buf, size_t size, uchar c)
{
	auto end = buf + (unsigned long) size;
	auto pos = std::find(buf, end, c);

	return pos != end;
}

result_t PacketRequest::parse(uchar* cbuf, size_t size, size_t  offset)
{
	if (PacketRequest::hasRequest(cbuf, size) == false) {
		return RES_NOENT;
	}

	Method met = static_cast<Method>((cbuf + 3)[0]);

	iMethod = met;

	size_t len = (size - 5) / 2;
	auto out = new char[len];
	auto str = tc::uchar2string((const uchar*)cbuf + 5, size - 5);
	tc::hex2bin((char*) str.data(), out);

	iType = str2req(out);
	iSize = size - 5;
	delete out;

	return RES_OK;
}

const size_t PacketRequest::size()
{
	return iSize;
}

} // namespace tc::parser