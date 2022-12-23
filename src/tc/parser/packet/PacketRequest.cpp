#include <tc/parser/packet/PacketRequest.h>
#include <algorithm>

namespace tc::parser {

size_t PacketRequest::REQ_MIN_SIZE = 11;

bool PacketRequest::hasRequest(const uchar* buf, size_t size)
{
	bool isRequest = false;
	if (size < PacketRequest::REQ_MIN_SIZE) {
		return isRequest;
	}

	isRequest |= contains(buf, size, (uchar) TYPE_PACKET_REQUEST);

	return isRequest;
}

bool PacketRequest::contains(const uchar* buf, size_t size, uchar c)
{
	auto end = buf + (unsigned long) size;
	auto pos = std::find(buf, end, c);

	return pos != end;
}

result_t PacketRequest::parse(uchar* cbuf, size_t size, size_t  offset)
{
	if (PacketRequest::hasRequest(cbuf, size) == false) {
		return RES_NOENT;
	}

	int met = static_cast<int>((cbuf + 3)[0]);
	iMethod = met;

	size_t len = (size - 5) / 2;
	auto out = new char[len];
	auto str = tc::uchar2string((const uchar*)cbuf + 5, size - 5);
	tc::hex2bin((char*) str.data(), out);

	//iType = tc::server::http::Request::str2req(out);
	iSize = size - 5;
	delete out;

	return RES_OK;
}

const size_t PacketRequest::size()
{
	return iSize;
}

} // namespace tc::parser