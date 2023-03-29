#include <tc/server/http/Request.h>
#include <tc/parser/packet/PacketRequest.h>
#include <tc/common/Regex.h>

namespace tc::server::http {

Request::Request()
 : Request(eGet, eDevices)
{
 // nothing to do
}

Request::Request(Method method, Type type)
 : Request(method2str(method), type2str(type))
{
 // nothing to do
}

Request::Request(std::string_view method, std::string_view type)
  : tc::LogI("console")
	, iRequest(CppServer::HTTP::HTTPRequest(method, type))
{
  // nothing to do
}

Request::Request(const CppServer::HTTP::HTTPRequest &request)
	: tc::LogI("console")
	, iRequest(request)
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
  if (!type.compare("devices")) {
		return Type::eDevices;
	} else if (!type.compare("device")) {
		return Type::eDevice;
	} else if (!type.compare("packets")) {
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
    return "/device";
    case eDevices:
    return "/devices";
		case ePackets:
    return "/packets";
    default:
    return "unknown";
  }
}

Request::Method Request::method() const
{
	return str2method(iRequest.method());
}

/* /type/../...  */
Request::Type Request::type() const
{
	auto url = std::string(iRequest.url());
	if (url[0] == '/')
		url.erase(0, 1);
	auto it = url.find('/') != std::string::npos ? url.find('/') : url.length();
	auto url_with_qp = url.substr(0, it);
	std::string type = url_with_qp.substr(0, url_with_qp.find("?"));

	return str2type(type);
}

bool Request::hasQuery() const
{
	return tc::regex(std::regex{"([^\\/]+$)"}, iRequest.url()).find("?") != std::string::npos;
}

/* /.../id/...  */
const std::string Request::id() const
{
	return tc::regex(std::regex{".*[-\\/](\\d+)"}, iRequest.url());
}

/* /.../.../command */
const std::string Request::command() const
{
	return depth() != 3 ? std::string() : tc::regex(std::regex{"([^\\/]+$)"}, iRequest.url());
}

const std::string Request::key() const
{
	return command().substr(0, command().find({"?"}));
}

result_t Request::query(std::string &key)
{
	std::string _key;
	auto after_slash = tc::regex(std::regex{"([^\\/]+$)"}, iRequest.url());
	auto query = after_slash.substr(after_slash.find("?") + 1);

	if (query.find("=") != std::string::npos)
		_key = query.substr(0, query.find("="));
	else
		_key = query.substr(query.find("?") + 1);

	key = _key;
	return RES_OK;
}

result_t Request::query(std::string &key, std::string &val)
{
	auto query = command().substr(command().find("?") + 1);
	auto _val = query.substr(query.find("=") + 1);
	auto _key = query.substr(0, query.find("="));


	key = _key;
	val = _val;
	return RES_OK;
}

result_t Request::toInternal(parser::Buf &buf, bool cr)
{
	result_t res = RES_OK;

	// 1 zero-byte
	auto val = byte2string(0);
	buf.insert(val.data(), val.length());

	// packet type
	val = byte2string((int) TYPE_PACKET_REQUEST);
	buf.insert(val.data(), val.length());

	// 1 zero-byte
	val = byte2string(0);
	buf.insert(val.data(), val.length());

	// method type
	val = byte2string(method());
	buf.insert(val.data(), val.length());

	// 1 zero-byte
	val = byte2string(0);
	buf.insert(val.data(), val.length());

	auto s_type = Request::type2str(type());
	s_type.erase(remove(s_type.begin(), s_type.end(), '/'), s_type.end());
	val = tc::tohex(s_type, cr);
  buf.insert(val.data(), val.length());

	return res;
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

const size_t Request::depth() const
{
	return count(iRequest.url().begin(), iRequest.url().end(), '/');
}

} // namespace tc::server::http