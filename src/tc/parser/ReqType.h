#ifndef B94047BD_14E6_47AB_816B_617BFEAE19BF
#define B94047BD_14E6_47AB_816B_617BFEAE19BF

#include <tc/common/Common.h>

namespace tc::parser {

using namespace parser;

class ReqType {
public:

	enum Type {
		Unknown = 0,
		Devices,
		Device,
    Set
  };

	enum Method {
		NONE = 0,
		HEAD,
		GET,
		POST,
		DELETE
	};

  ReqType() = default;
  ReqType(const std::string_view method, const std::string type);

  virtual ~ReqType() = default;

  void setMethod(const std::string_view method);
  void setType(const std::string type);

	const std::string req2str(const Type type);
	Type str2req(const std::string &req);

	virtual Method method() const;
	virtual Type type() const;

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
