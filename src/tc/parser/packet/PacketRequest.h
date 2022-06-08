#ifndef E0CCFC07_4DC4_466F_BFF6_501414B18F09
#define E0CCFC07_4DC4_466F_BFF6_501414B18F09

#include <tc/common/Common.h>
#include <tc/parser/packet/Packet.h>
#include <tc/parser/Reader.h>

namespace tc::parser {

class PacketRequest : public Packet
{
public:
	enum Type {
		Unknown = 0,
		Devices,
		Data
	};

	enum Method {
		NONE = 0,
		HEAD,
		GET,
		POST,
		DELETE
	};


	static size_t REQ_MIN_SIZE;

	static Method toMethod(const std::string_view method);
	static std::string fromMethod(const Method method);
	static std::string fromType(const Type type);


	static bool hasRequest(const uchar* buf, size_t size);
	static bool contains(const uchar* buf, size_t size, uchar c);

	static Type str2req(const std::string &req);
	static const std::string req2str(const Type type);

	result_t parse(uchar* cbuf, size_t size, size_t offset = 0) override;

	const size_t size() override;

	Method iMethod {NONE};
	Type iType {Unknown};
	size_t iSize {0};
};

} // namespace tc::parser

#endif /* E0CCFC07_4DC4_466F_BFF6_501414B18F09 */
