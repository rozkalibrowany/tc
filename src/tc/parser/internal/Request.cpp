#include <tc/parser/internal/Request.h>

namespace tc::parser::internal {

bool Request::hasRequest(const uchar* buf, size_t size)
{
	bool isRequest = false;
	if (size < Request::REQ_MIN_SIZE) {
		return isRequest;
	}

	isRequest |= buf[1] == static_cast<uchar>(Request::c_id) ? true : false;

	return isRequest;
}

result_t Request::parse(const uchar* cbuf, size_t size)
{
	if (hasRequest(cbuf, size) == false) {
		return RES_NOENT;
	}

	iMethod = static_cast<Method>((cbuf + 3)[0]);

	size_t len = (size - 5) / 2;
	auto out = new char[len];
	auto str = tc::uchar2string((const uchar*)cbuf + 5, size - 5);
	tc::hex2bin((char*) str.data(), out);

	iType = string2type(out);
	iSize = size - 5;
	delete out;

	LG_DBG(this->logger(), "Request parse, method: {} type: {}", (int) iMethod, (int) iType);

	return RES_OK;
}

size_t Request::size() const
{
	return iSize;
}

} // namespace tc::parser::internal