#ifndef B94047BD_14E6_47AB_816B_617BFEAE19BF
#define B94047BD_14E6_47AB_816B_617BFEAE19BF

#include <server/http/http_request.h>
#include <tc/common/Common.h>
#include <tc/parser/Buf.h>

namespace tc::server::http {

class HTTPRequest;

class Request : public Types, public tc::LogI {
public:

  Request();
  Request(const CppServer::HTTP::HTTPRequest& request);

  virtual ~Request() = default;

	Method method() const;
	Type type() const;
	const std::string id() const;
	const std::string command() const;
	const std::pair<std::string, std::string> query() const;

private:
	const size_t depth() const;

	mutable CppServer::HTTP::HTTPRequest iRequest;
};

} // namespace tc::server::http


#endif /* B94047BD_14E6_47AB_816B_617BFEAE19BF */
