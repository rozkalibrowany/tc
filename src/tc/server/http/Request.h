#ifndef B94047BD_14E6_47AB_816B_617BFEAE19BF
#define B94047BD_14E6_47AB_816B_617BFEAE19BF

#include <server/http/http_request.h>

namespace tc::server::http {

class HTTPRequest;

class Request {
public:

	enum Type {
		eUnknown = 0,
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

  Request() = default;
  Request(const CppServer::HTTP::HTTPRequest& request);

  ~Request() = default;

	Method method() const;
	Type type() const;
	const std::string id() const;
	const std::string command() const;

	static Type str2req(const std::string &req);
  static const std::string_view method2str(Method method);
  static const std::string type2str(Type type);
  static Method str2method(const std::string_view method);
  static Type str2type(const std::string type);

	mutable std::pair<std::string, std::string> iQueryParam;
protected:
	mutable CppServer::HTTP::HTTPRequest iRequest;
};

} // namespace tc::server::http


#endif /* B94047BD_14E6_47AB_816B_617BFEAE19BF */
