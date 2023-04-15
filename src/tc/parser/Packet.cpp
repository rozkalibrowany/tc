#include <tc/parser/Packet.h>
#include <tc/common/CRC16.h>

namespace tc::parser {

Packet::Packet(const std::string imei)
	: tc::LogI("console")
	, iCodec(0)
{
	// nothing to do
}

Packet::Type Packet::str2req(const std::string &req)
{
	if (req.compare("devices") == 0) {
		return Type::eDevices;
	} else if (req.compare("packets") == 0) {
		return Type::ePackets;
	} else {
		return Type::eUnknown;
	}
}

const char* Packet::type2string(Type type)
{
	switch (type) {
		case ePackets: return "Packets";
		case eDevice: return "Device";
		case eDevices: return "Devices";
		default: return "Unknown";
	}
}

const char* Packet::method2string(Method method)
{
	switch (method) {
		case eGet: return "GET";
		case eHead: return "HEAD";
		case ePost: return "POST";
		case eDelete: return "DELETE";
		default: return "None";
	}
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

bool Packet::crcOk(const std::shared_ptr< common::Buf > buf, size_t size)
{
	if (buf == nullptr) {
		return false;
	}

	common::CRC16 crc;
	auto reader = std::make_shared< parser::Reader >(buf, size);
	auto reader_crc = reader->read(4, size - 4);

	common::Buf subBuf(common::Buf::ByteArray{buf->begin() + 8, buf->begin() + size - 4});
	auto calc = crc.calc((char*) subBuf.iBuf.data(), subBuf.size());

	return calc == (int)reader_crc ? true : false;
}

int Packet::codec() const
{
	return iCodec;
}

const Packet::Timestamp &Packet::timestamp() const
{
	return iTimestamp;
}

} // namespace tc::parser