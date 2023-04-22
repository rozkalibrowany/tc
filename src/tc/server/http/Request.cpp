#include <tc/server/http/Request.h>
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
	: iRequest(CppServer::HTTP::HTTPRequest(method, type))
{
  // nothing to do
}

Request::Request(const CppServer::HTTP::HTTPRequest &request)
	: iRequest(request)
{
  // nothing to do
}

result_t Request::parse(const uchar *cbuf, size_t size)
{
 return RES_NOIMPL;
}

size_t Request::size() const
{
 return iRequest.string().size();
}

const std::string Request::method2str(Method method)
{
  switch (method) {
    case eGet:
    return {"GET"};
    case ePost:
    return {"POST"};
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

void Request::setID(const std::string_view &id)
{
	iID = id;
}

CppServer::HTTP::HTTPRequest Request::request() const
{
	return iRequest;
}

Request::Method Request::method() const
{
	if (!iRequest.method().compare("GET"))
		return eGet;
	else if (!iRequest.method().compare("POST"))
		return ePost;
	else if (!iRequest.method().compare("HEAD"))
		return eHead;
	return eNone;
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

	return string2type(type);
}

bool Request::hasQuery() const
{
	return tc::regex(std::regex{"([^\\/]+$)"}, iRequest.url()).find("?") != std::string::npos;
}

bool Request::hasImei() const
{
	if (id().length() == IMEI_LENGTH)
		return true;
	return false;
}

/* /.../id/...  */
const std::string Request::id() const
{
	if (iID.has_value())
		return iID.value();
	else
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

result_t Request::toInternal(common::Buf &buf, bool cr)
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

const size_t Request::depth() const
{
	return count(iRequest.url().begin(), iRequest.url().end(), '/');
}

} // namespace tc::server::http