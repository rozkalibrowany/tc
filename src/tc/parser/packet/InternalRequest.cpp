#include <tc/parser/packet/InternalRequest.h>
#include <algorithm>
#include <cstring>

namespace tc::parser::packet {

size_t InternalRequest::REQ_MIN_SIZE = 11;
size_t InternalRequest::REQ_MAX_SIZE = 10 + IMEI_LENGTH + sizeof(timestamp) + Types::type2str(Types::eDevices).size() + 5;

InternalRequest::InternalRequest(const std::string imei)
	: Packet(imei)
{
 // nothing to do
}

bool InternalRequest::hasInternalRequest(const uchar* buf, size_t size)
{
	if (size < REQ_MIN_SIZE || size > REQ_MAX_SIZE) {
		return false;
	}

	auto method = static_cast<Method>((buf + 3)[0]);
	auto type = static_cast<int>((buf + 1)[0]);

	return (method == Types::eGet && (type == TYPE_REQ_INTERNAL_DEVICE_DATA || type == TYPE_REQ_INTERNAL_DEVICES));
}

bool InternalRequest::contains(const uchar* buf, size_t size, uchar c)
{
	auto end = buf + (unsigned long) size;
	auto pos = std::find(buf, end, c);

	return pos != end;
}

result_t InternalRequest::parse(uchar* cbuf, size_t size, size_t  offset)
{
	if (hasInternalRequest(cbuf, size) == false) {
		return RES_NOENT;
	}

	auto type = static_cast<int>((cbuf + 1)[0]);
	if (type != TYPE_REQ_INTERNAL_DEVICES && type != TYPE_REQ_INTERNAL_DEVICE_DATA) {
		return RES_NOENT;
	}

	return type == TYPE_REQ_INTERNAL_DEVICES ? handleDevices(cbuf, size) : handleDeviceData(cbuf, size);
}

result_t InternalRequest::handleDevices(uchar *cbuf, size_t size)
{
	auto method = static_cast<Method>((cbuf + 3)[0]);

	size_t len = (size - 5) / 2;
	auto out = new char[len];
	auto str = tc::uchar2string((const uchar*)cbuf + 5, size - 5);
	tc::hex2bin((char*) str.data(), out);

	iType = Packet::str2req(out);
	iSize = size - 5;
	delete out;

	iMethod = method;
	return RES_OK;
}

result_t InternalRequest::handleDeviceData(uchar *cbuf, size_t size)
{
	auto method = static_cast<Method>((cbuf + 3)[0]);

	auto t_len = static_cast<int>((cbuf + 5)[0]) / 2;
	auto timestamp_s = new char[t_len];
	tc::hex2bin((char*) tc::uchar2string((const uchar *)cbuf + 7, t_len).data(), timestamp_s);
	auto timestamp = std::string(timestamp_s, t_len);
	auto offset = 6 + t_len;

	auto i_len = static_cast<int>((cbuf + 6 + t_len + 2)[0]) / 2;
	auto imei_s = new char[i_len];
	tc::hex2bin((char*) tc::uchar2string((const uchar *)cbuf + 6 + t_len + 4, i_len).data(), imei_s);
	Imei imei = std::string(imei_s, i_len);
	offset += 4 + i_len + 1;

	auto r_len = size - offset;
	auto req_s = new char[r_len];
	tc::hex2bin((char *)tc::uchar2string((const uchar *)cbuf + offset, r_len).data(), req_s);
	auto request = std::string(req_s, r_len);

	iTimestamp = std::stoi(timestamp);
	iImei = imei;
	iMethod = method;
	iType = Types::str2type(request);
	return RES_OK;
}

result_t InternalRequest::create(Method method, Type type, const Imei &imei, timestamp t, bool cr)
{
	result_t res = RES_OK;

	if (!iBuf.empty()) iBuf.clear();

	// 1 zero-byte
	auto val = byte2string(0);
	iBuf.insert(val.data(), val.length());

	// packet type
	val = byte2string((int) TYPE_REQ_INTERNAL_DEVICE_DATA);
	iBuf.insert(val.data(), val.length());

	// 1 zero-byte
	val = byte2string(0);
	iBuf.insert(val.data(), val.length());

	// method type
	val = byte2string(method);
	iBuf.insert(val.data(), val.length());

	// 1 zero-byte
	val = byte2string(0);
	iBuf.insert(val.data(), val.length());

	// insert timestamp length
	val = tohex(std::to_string(t));
	auto len = byte2string(val.length());
	iBuf.insert(len.data(), len.length());

	// 1 zero-byte
	auto v = byte2string(0);
	iBuf.insert(v.data(), v.length());

	// insert timestamp
	iBuf.insert(val.data(), val.length());

	// 1 zero-byte
	val = byte2string(0);
	iBuf.insert(val.data(), val.length());

	// imei
	val = tc::tohex(imei, cr);
  len = byte2string(val.length());
	iBuf.insert(len.data(), len.length());

	// 1 zero-byte
	v = byte2string(0);
	iBuf.insert(v.data(), v.length());

	iBuf.insert(val.data(), val.length());

	// 1 zero-byte
	val = byte2string(0);
	iBuf.insert(val.data(), val.length());

	// request
	auto s_type = type2str(type);
	s_type.erase(remove(s_type.begin(), s_type.end(), '/'), s_type.end());
	val = tc::tohex(s_type, cr);
  iBuf.insert(val.data(), val.length());


	return res;
}

result_t InternalRequest::create(Method method, Type type, bool cr)
{
	result_t res = RES_OK;

	if (!iBuf.empty()) iBuf.clear();

	// 1 zero-byte
	auto val = byte2string(0);
	iBuf.insert(val.data(), val.length());

	// packet type
	val = byte2string((int) TYPE_REQ_INTERNAL_DEVICES);
	iBuf.insert(val.data(), val.length());

	// 1 zero-byte
	val = byte2string(0);
	iBuf.insert(val.data(), val.length());

	// method type
	val = byte2string(method);
	iBuf.insert(val.data(), val.length());

	// 1 zero-byte
	val = byte2string(0);
	iBuf.insert(val.data(), val.length());


	auto s_type = type2str(type);
	s_type.erase(remove(s_type.begin(), s_type.end(), '/'), s_type.end());
	val = tc::tohex(s_type, cr);
  iBuf.insert(val.data(), val.length());

	return res;
}

InternalRequest::Type InternalRequest::type() const
{
	return iType;
}

InternalRequest::Method InternalRequest::method() const
{
	return iMethod;
}

Imei InternalRequest::imei() const
{
	return iImei;
}

uchar* InternalRequest::data()
{
	return iBuf.data();
}

const uchar* InternalRequest::cdata() const
{
	return iBuf.cdata();
}

size_t InternalRequest::size() const
{
	return iBuf.size();
}

} // namespace tc::parser::packet