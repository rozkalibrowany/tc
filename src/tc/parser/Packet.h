#ifndef E2EB797D_BF96_49F2_8076_6FCE70519A6C
#define E2EB797D_BF96_49F2_8076_6FCE70519A6C

#include <tc/common/Buf.h>
#include <tc/common/Common.h>
#include <tc/parser/Reader.h>
#include <tc/parser/Json.h>
#include <compare>

#define TYPE_PACKET_COMMAND			0x5
#define IMEI_LENGTH							0xF

namespace tc::parser {

class Packet : public tc::LogI, public JsonI
{
public:

	Packet() = default;
	Packet(const Imei &imei);
	Packet(const Packet &rhs) = default;
	Packet(Packet &&rhs) = default;

	virtual ~Packet() = default;

	auto operator<=>(const Packet& rhs) const = default;

	static result_t parseImei(const uchar *cbuf, size_t size, Imei &imei);
	static const std::string toImei(const uchar *cbuf, int len);
	static bool hasImei(const uchar *cbuf, size_t size);

	virtual result_t parse(const uchar *cbuf, size_t size, size_t offset = 0) = 0;
	virtual const size_t size() = 0;

	virtual Json::Value toJsonValue();

	virtual int codec() const;
	virtual const SysTime &timestamp() const;

protected:
	bool crcOk(const common::Buf &buf, size_t size) const;
	result_t toJsonImpl(Json::Value &rhs, bool root) const override;
	result_t fromJsonImpl(const Json::Value &rhs, bool root) override;

	Imei iImei {"unknown"};
	SysTime iTimestamp;
	int iCodec {0};
};

} // namespace tc::parser


#endif /* E2EB797D_BF96_49F2_8076_6FCE70519A6C */
