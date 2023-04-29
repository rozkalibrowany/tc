#include <tc/parser/Packet.h>
#include <tc/common/CRC16.h>

namespace tc::parser {

Packet::Packet(const Imei &imei)
 : iImei(imei)
 , tc::LogI("console")
{
	// nothing to do
}

bool Packet::hasImei(const uchar *cbuf, size_t size)
{
	if (size < 15) {
		return false;
	}

	return ((cbuf[0] << 8) | ((cbuf[1]) << 0 == 15)) ? true : false;
}

result_t Packet::parseImei(const uchar* cbuf, size_t size, Imei &imei)
{
	auto len = ((cbuf[0]) << 8) | ((cbuf[1]) << 0);
	if (len <= 2 || len > 17) {
		return RES_INVARG;
	}

	auto buf = cbuf + 2;
	auto from_buf = toImei(buf, len);

	imei = from_buf;
	return RES_OK;
}

const Imei Packet::toImei(const uchar* cbuf, int len)
{
	auto hex_str = tc::uchar2string(cbuf, len);

	Imei imei;
	for (Imei::size_type i = 1; i < hex_str.size(); i += 2) {
		char c = hex_str[i];
		imei.push_back(c);
	}

	return imei;
}

bool Packet::crcOk(const common::Buf &buf, size_t size) const
{
	common::CRC16 crc;
	parser::Reader reader(buf, size);
	auto reader_crc = reader.read(4, size - 4);

	common::Buf subBuf(common::Buf::ByteArray{buf.cbegin() + 8, buf.cbegin() + size - 4});
	auto calc = crc.calc((char*) subBuf.iBuf.data(), subBuf.size());

	return calc == (int)reader_crc ? true : false;
}

int Packet::codec() const
{
	return iCodec;
}

const SysTime &Packet::timestamp() const
{
	return iTimestamp;
}

Json::Value Packet::toJsonValue()
{
	return Json::Value();
}

result_t Packet::toJsonImpl(Json::Value &rhs, bool root) const
{
	return RES_NOIMPL;
}

result_t Packet::fromJsonImpl(const Json::Value &rhs, bool root)
{
	return RES_NOIMPL;
}

} // namespace tc::parser