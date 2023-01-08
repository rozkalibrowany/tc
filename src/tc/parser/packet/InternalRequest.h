#ifndef E0CCFC07_4DC4_466F_BFF6_501414B18F09
#define E0CCFC07_4DC4_466F_BFF6_501414B18F09

#include <tc/parser/packet/Packet.h>
#include <tc/parser/Reader.h>

namespace tc::parser::packet {

class InternalRequest : public Packet
{
public:
	static size_t REQ_MIN_SIZE;
	static size_t REQ_MAX_SIZE;

	using timestamp = int64_t;

	InternalRequest() = default;
	InternalRequest(const std::string imei);

	static bool hasInternalRequest(const uchar *buf, size_t size);
	static bool contains(const uchar *buf, size_t size, uchar c);

	result_t handleDevices(uchar *cbuf, size_t size);
	result_t handleDeviceData(uchar *cbuf, size_t size);

	result_t parse(uchar *cbuf, size_t size, size_t offset = 0) override;
	result_t create(Method method, Type type, const Imei &imei, timestamp t, bool cr = false);
	result_t create(Method method, Type type, bool cr = false);

	size_t size() const override;

	Type type() const;
	Method method() const;
	Imei imei() const;

	uchar *data();
	const uchar *cdata() const;

protected:
	Buf iBuf;
	Method iMethod{eNone};
	Type iType{eUnknown};
	size_t iSize{0};
};

} // namespace tc::parser::packet

#endif /* E0CCFC07_4DC4_466F_BFF6_501414B18F09 */
