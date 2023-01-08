#ifndef D4F57DC0_9751_43F6_8654_3A2E77B3D463
#define D4F57DC0_9751_43F6_8654_3A2E77B3D463

#include <string>

namespace tc {

class Types {
public:

enum Type {
	eUnknown = 0,
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

static Method str2method(const std::string_view method);
static Type str2type(const std::string type);
static const std::string_view method2str(Method method);
static const std::string type2str(Type type);
static Type str2req(const std::string &req);
};

} // namespace tc

#endif /* D4F57DC0_9751_43F6_8654_3A2E77B3D463 */
