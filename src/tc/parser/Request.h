#ifndef B94047BD_14E6_47AB_816B_617BFEAE19BF
#define B94047BD_14E6_47AB_816B_617BFEAE19BF

#include <tc/common/Common.h>

namespace tc::parser {

using namespace parser;

class Request {
public:

	enum Type {
		Unknown = 0,
		Packets,
		Devices,
		Device
  };

	enum Method {
		NONE = 0,
		HEAD,
		GET,
		POST,
		DELETE
	};

  Request() = default;
  Request(const std::string_view method, const std::string type);

  ~Request() = default;

	Method method() const;
	Type type() const;

	void setMethod(const std::string_view method);
  void setType(const std::string type);

	Type str2req(const std::string &req);

protected:
  Method toMethod(const std::string_view method);
  Type toType(const std::string type);
  std::string_view fromMethod(Method method);
  std::string fromType(Type type);

	Method iMethod {NONE};
	Type iType {Unknown};
};

} // namespace tc::parser


#endif /* B94047BD_14E6_47AB_816B_617BFEAE19BF */
