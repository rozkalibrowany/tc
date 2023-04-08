#include <tc/parser/omni/packet/Payload.h>

namespace tc::parser::omni {

bool Payload::isOmni(const uchar* buf, size_t size)
{
	return false;
}

result_t Payload::parse(const uchar* cbuf, size_t size, size_t /* offset */)
{
	return RES_NOIMPL;
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

