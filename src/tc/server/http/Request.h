#ifndef B94047BD_14E6_47AB_816B_617BFEAE19BF
#define B94047BD_14E6_47AB_816B_617BFEAE19BF

#include <server/http/http_request.h>
#include <tc/common/Common.h>
#include <tc/common/Buf.h>
#include <tc/parser/RequestI.h>

namespace tc::server::http {

class HTTPRequest;

class Request : public parser::RequestI {
public:

  Request();
  Request(Request::Method method, Type type);
	Request(std::string_view method, std::string_view type);
	Request(const CppServer::HTTP::HTTPRequest &request);

	result_t parse(const uchar *cbuf, size_t size) override;
	size_t size() const override;

	Method method() const override;
	Type type() const override;

	bool hasQuery() const;
	bool hasImei() const;

	void setID(const std::string_view &id);

	const std::string id() const;
	const std::string command() const;
	const std::string key() const;

	CppServer::HTTP::HTTPRequest request() const;

	result_t query(std::string &key);
	result_t query(std::string &key, std::string &val);

	result_t toInternal(common::Buf &buf, bool cr = false);

  static const std::string method2str(Method method);
  static const std::string type2str(Type type);

private:
	const size_t depth() const;

	mutable std::optional<std::string> iID;
	mutable CppServer::HTTP::HTTPRequest iRequest;
};

} // namespace tc::server::http


#endif /* B94047BD_14E6_47AB_816B_617BFEAE19BF */
