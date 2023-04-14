#include <tc/parser/omni/packet/Payload.h>

namespace tc::parser::omni {

bool Payload::valid(const uchar* buf, size_t size)
{
	if (size < 1) return false;
	return (uchar)buf[0] == (uchar)OMNI_HEADER;
}

result_t Payload::parseImei(const std::string_view &buf, Imei &imei)
{
	if (buf.empty())
		return RES_NOENT;

	// getting index where imei should start
	auto index = index_of_nth(buf, ',', 2);
	if (index + IMEI_SIZE > buf.size())
		return RES_NOMEM;

	imei.reserve(IMEI_SIZE);
	imei = std::string(buf.data() + index, IMEI_SIZE);

	return RES_OK;
}

size_t Payload::index_of_nth(const std::string_view &str, char delim, unsigned n)
{
	if (n == 0)
		return std::string_view::npos;

	size_t pos, from = 0;
	unsigned i=0;
	while (i < n) {
		pos = str.find(delim, from);
		if (pos == std::string_view::npos)
			break;
		from = pos + 1;
		++i;
	}
	return ++pos; // return with +1 offset
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

