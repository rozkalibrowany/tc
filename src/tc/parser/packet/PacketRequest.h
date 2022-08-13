#ifndef E0CCFC07_4DC4_466F_BFF6_501414B18F09
#define E0CCFC07_4DC4_466F_BFF6_501414B18F09

#include <tc/common/Common.h>
#include <tc/parser/packet/Packet.h>
#include <tc/parser/Reader.h>
#include <tc/parser/ReqType.h>
namespace tc::parser {

class PacketRequest : public Packet, public ReqType
{
public:
	static size_t REQ_MIN_SIZE;

	static bool hasRequest(const uchar* buf, size_t size);
	static bool contains(const uchar* buf, size_t size, uchar c);

	result_t parse(uchar* cbuf, size_t size, size_t offset = 0) override;

	const size_t size() override;

	Method method() const override;
	Type type() const override;

protected:
	Method iMethod {NONE};
	Type iType {Unknown};
	size_t iSize {0};
};

} // namespace tc::parser

#endif /* E0CCFC07_4DC4_466F_BFF6_501414B18F09 */
