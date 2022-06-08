#include <tc/server/http/RequestFactory.h>

namespace tc::server::http {


result_t RequestFactory::create(const Action &action, parser::Buf &buf, bool cr)
{
	result_t res = RES_OK;

	// 1 zero-byte
	auto val = byte2string(0);
	buf.insert(val.data(), val.length());

	// packet type
	val = byte2string((int) TYPE_PACKET_REQUEST);
	buf.insert(val.data(), val.length());

	// 1 zero-byte
	val = byte2string(0);
	buf.insert(val.data(), val.length());

	// method type
	val = byte2string(action.iMethod);
	buf.insert(val.data(), val.length());

	// 1 zero-byte
	val = byte2string(0);
	buf.insert(val.data(), val.length());

  val = tc::tohex(action.iReq, cr);
  buf.insert(val.data(), val.length());

	return res;
}

} // namespace tc::server::http
