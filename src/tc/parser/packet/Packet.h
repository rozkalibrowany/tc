#ifndef E2EB797D_BF96_49F2_8076_6FCE70519A6C
#define E2EB797D_BF96_49F2_8076_6FCE70519A6C

#include <tc/parser/Buf.h>
#include <tc/common/Common.h>
#include <tc/parser/Reader.h>

#define TYPE_PACKET_COMMAND			0x5
#define TYPE_PACKET_RESPONSE		0x6
#define TYPE_PACKET_REQUEST			0x7
#define IMEI_LENGTH							0xF

namespace tc::parser {

class Packet : public tc::LogI
{
public:

	struct ID {
		ID &operator=(const ID &rhs) = default;
		auto operator<=>(const ID &rhs) const = default;

		ID(bool now) : timestamp(SysTime(now)) {}
		ID() : ID(true) {}

		SysTime timestamp;
	};

	Packet(const std::string imei = "");
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
	virtual const size_t size() = 0;

	virtual int codec() const;
	virtual const ID &id() const;

protected:
	bool crcOk(const std::shared_ptr< Buf > buf, size_t size);

	ID iID;
	int iCodec;
};

} // namespace tc::parser


#endif /* E2EB797D_BF96_49F2_8076_6FCE70519A6C */
