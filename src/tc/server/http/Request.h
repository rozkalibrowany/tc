#ifndef B94047BD_14E6_47AB_816B_617BFEAE19BF
#define B94047BD_14E6_47AB_816B_617BFEAE19BF

#include <server/http/http_request.h>
#include <tc/common/Common.h>
#include <tc/parser/Buf.h>

namespace tc::server::http {

class HTTPRequest;

class Request : public tc::LogI {
public:
	enum Type {
		eUnknown = 0,
		ePacket,
		ePackets,
		eDevices,
		eDevice
  };

	enum Method {
		eNone = 0,
		eHead,
		eGet,
		ePost,
		eDelete
	};

  Request();
  Request(Method method, Type type);
	Request(std::string_view method, std::string_view type);
	Request(const CppServer::HTTP::HTTPRequest &request);

	Method method() const;
	Type type() const;

	bool hasQuery() const;
	bool hasImei() const;

	void setID(std::string_view id);

	const std::string id() const;
	const std::string command() const;
	const std::string key() const;

	CppServer::HTTP::HTTPRequest request() const;

	result_t query(std::string &key);
	result_t query(std::string &key, std::string &val);

	result_t toInternal(parser::Buf &buf, bool cr = false);

	static Type str2req(const std::string &req);
  static const std::string method2str(Method method);
  static const std::string type2str(Type type);
  static Method str2method(const std::string_view method);
  static Type str2type(const std::string type);

private:
	const size_t depth() const;

	mutable std::optional<std::string> iID;
	mutable CppServer::HTTP::HTTPRequest iRequest;
};

} // namespace tc::server::http


#endif /* B94047BD_14E6_47AB_816B_617BFEAE19BF */
