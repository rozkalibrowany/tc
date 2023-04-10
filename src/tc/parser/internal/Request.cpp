#include <tc/parser/internal/Request.h>

namespace tc::parser::internal {

size_t Request::REQ_MIN_SIZE = 11;

bool Request::hasRequest(const uchar* buf, size_t size)
{
	bool isRequest = false;
	if (size < Request::REQ_MIN_SIZE) {
		return isRequest;
	}

	isRequest |= contains(buf, size, (uchar) TYPE_PACKET_REQUEST);

	return isRequest;
}

bool Request::contains(const uchar* buf, size_t size, uchar c)
{
	auto end = buf + (unsigned long) size;
	auto pos = std::find(buf, end, c);

	return pos != end;
}

result_t Request::parse(const uchar* cbuf, size_t size, size_t  offset)
{
	if (Request::hasRequest(cbuf, size) == false) {
		return RES_NOENT;
	}

	iMethod = static_cast<Method>((cbuf + 3)[0]);

	size_t len = (size - 5) / 2;
	auto out = new char[len];
	auto str = tc::uchar2string((const uchar*)cbuf + 5, size - 5);
	tc::hex2bin((char*) str.data(), out);

	iType = Packet::str2req(out);
	iSize = size - 5;
	delete out;

	LG_DBG(this->logger(), "Request parse, method: {} type: {}", (int) iMethod, (int) iType);

	return RES_OK;
}

const size_t Request::size()
{
	return iSize;
}

} // namespace tc::parser::internal