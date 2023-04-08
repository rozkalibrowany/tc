#ifndef D0D21C63_ABC2_402A_BA25_0EDB333AB7CD
#define D0D21C63_ABC2_402A_BA25_0EDB333AB7CD

#include <tc/common/Common.h>
#include <tc/common/Buf.h>
#include <tc/parser/Json.h>

namespace tc::parser {

result_t convert(const Json::Value &rhs, bool &val) noexcept;
result_t convert(const Json::Value &rhs, int8_t &val) noexcept;
result_t convert(const Json::Value &rhs, int16_t &val) noexcept;
result_t convert(const Json::Value &rhs, int32_t &val) noexcept;
result_t convert(const Json::Value &rhs, int64_t &val) noexcept;


result_t convert(const Json::Value &rhs, uint8_t &val) noexcept;
result_t convert(const Json::Value &rhs, uint16_t &val) noexcept;
result_t convert(const Json::Value &rhs, uint32_t &val) noexcept;
result_t convert(const Json::Value &rhs, uint64_t &val) noexcept;

result_t convert(const Json::Value &rhs, float &val) noexcept;
result_t convert(const Json::Value &rhs, double &val) noexcept;

template < typename T >
result_t convertStr(const Json::Value &rhs, T &val) noexcept
{
	if (rhs.isString() == true) {
		result_t res = convert(rhs.asString(), val);
		if (res == RES_OK) {
			return res;
		}
	}

	return RES_NOENT;
}

} // namespace tc::parser


#endif /* D0D21C63_ABC2_402A_BA25_0EDB333AB7CD */
