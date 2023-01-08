#include <tc/server/http/Request.h>
#include <tc/common/Common.h>
#include <tc/parser/packet/InternalRequest.h>
#include <tc/common/Regex.h>

namespace tc::server::http {

Request::Request()
 : Request(CppServer::HTTP::HTTPRequest("GET", "/devices", "HTTP/1.1"))
{
 // nothing to do
}

Request::Request(const CppServer::HTTP::HTTPRequest& request)
  : tc::LogI("console")
	, iRequest(std::move(request))
{
  // nothing to do
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
	std::string type = url.substr(0, it);

	return str2type(type);
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

const std::pair<std::string, std::string> Request::query() const
{
	auto right = command().substr(command().find("?") + 1);
	auto key = right.substr(0, right.find("="));
	auto val = right.substr(right.find("=") + 1);

	return std::make_pair(key, val);
}

const size_t Request::depth() const
{
	return count(iRequest.url().begin(), iRequest.url().end(), '/');
}

} // namespace tc::server::http