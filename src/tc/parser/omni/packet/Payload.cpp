#include <tc/parser/omni/packet/Payload.h>

namespace tc::parser::omni {

bool Payload::valid(const uchar* buf, size_t size)
{
	if (size < 1) return false;
	return (uchar)buf[0] == (uchar)OMNI_HEADER;
}

result_t Payload::parseImei(const common::Buf &buf, Imei &imei)
{
	if (buf.empty())
		return RES_NOENT;

	// getting index where imei should start
	auto index = buf.find_nth(',', 2);
	if (!index && index + IMEI_SIZE > buf.size())
		return RES_NOMEM;

	imei.reserve(IMEI_SIZE);
	imei = std::string((const char*) (buf.cdata() + index), IMEI_SIZE);

	return RES_OK;
}

result_t Payload::parseManufacturer(const common::Buf &buf, std::string &manufacturer)
{
	if (buf.empty())
		return RES_NOENT;
	
	// getting index where manufacturer should start
	auto index = buf.find_nth(',', 1);
	if (!index && index + MANUFACTURER_SIZE > buf.size())
		return RES_NOMEM;

	manufacturer.reserve(2);
	manufacturer = std::string((const char*) (buf.cdata() + index), MANUFACTURER_SIZE);

	return RES_OK;
}

result_t Payload::parseDatetime(const common::Buf &buf, Datetime &datetime)
{
	if (buf.empty())
		return RES_NOENT;
	
	// getting index where datetime should start
	auto index = buf.find_nth(',', 3);
	if (!index && index + DATETIME_SIZE > buf.size())
		return RES_NOMEM;

	if(datetime.parse(std::string((const char*) (buf.cdata() + index), DATETIME_SIZE)) != RES_OK)
		return RES_ERROR;

	return RES_OK;
}

result_t Payload::parse(const std::string_view &buffer)
{
	return RES_OK;
}

result_t Payload::parse(const uchar* cbuf, size_t size, size_t /* offset */)
{
	const std::string_view str_buf(reinterpret_cast<char const *>(cbuf));
	return parse(str_buf);
}

const size_t Payload::size()
{
	return 0;
}

result_t Payload::toJsonImpl(Json::Value &rhs, bool root) const
{
	return RES_NOIMPL;
}

result_t Payload::fromJsonImpl(const Json::Value &rhs, bool root)
{
	return RES_NOIMPL;
}

} // namespace tc::parser::omni

