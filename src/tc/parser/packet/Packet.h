#ifndef E2EB797D_BF96_49F2_8076_6FCE70519A6C
#define E2EB797D_BF96_49F2_8076_6FCE70519A6C

#include <tc/parser/Buf.h>
#include <tc/common/Common.h>
#include <tc/parser/Reader.h>

#define TYPE_UNKNOWN									0xA
#define TYPE_COMMAND									0xB
#define TYPE_RESPONSE									0xC
#define TYPE_REQ_INTERNAL_DEVICES			0xD
#define TYPE_REQ_INTERNAL_DEVICE_DATA	0xE
#define IMEI_LENGTH										0xF

namespace tc::parser::packet {

class Packet : public Types, public tc::LogI
{
public:
	struct Timestamp {
		Timestamp &operator=(const Timestamp &rhs) = default;
		auto operator<=>(const Timestamp &rhs) const = default;

		Timestamp(bool now) : timestamp(SysTime(now)) {}
		Timestamp() : Timestamp(true) {}

		SysTime timestamp;
	};

	Packet() = default;
	Packet(const Imei &imei);
	virtual ~Packet() = default;

	static result_t parseImei(const uchar *cbuf, size_t size, Imei &imei);
	static const std::string toImei(const uchar *cbuf, int len);

	static bool hasImei(const uchar *cbuf, size_t size);

	virtual bool operator<(const Packet &rhs) const;
	virtual bool operator<=(const Packet &rhs) const;
	virtual bool operator>(const Packet &rhs) const;
	virtual bool operator>=(const Packet &rhs) const;
	virtual bool operator==(const Packet &rhs) const;

	virtual result_t parse(uchar *cbuf, size_t size, size_t offset = 0) = 0;
	virtual size_t size() const = 0;

	virtual int codec() const;
	virtual const Timestamp &timestamp() const;

protected:
	bool crcOk(const std::shared_ptr< Buf > buf, size_t size);

	Timestamp iTimestamp;
	Imei iImei;
	int iCodec;
};

} // namespace tc::parser::packet


#endif /* E2EB797D_BF96_49F2_8076_6FCE70519A6C */
