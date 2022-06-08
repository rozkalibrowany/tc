#include <tc/parser/Convert.h>

namespace tc::parser {

result_t convert(const Json::Value &rhs, bool &val) noexcept
{
	try {
		if (rhs.asBool() == true) {
			val = rhs.asBool();
			return RES_OK;
		}
	} catch (...) {
	}

	try {
		if (rhs.asInt() == true) {
			val = rhs.asInt();
		}
	} catch (...) {
	}

	return convertStr(rhs, val);
}

result_t convert(const Json::Value &rhs, int8_t &val) noexcept
{
	try {
		if (rhs.asInt64() == true) {
			val = ( int8_t ) rhs.asInt64();
			return RES_OK;
		}
	} catch (...) {
	}

	return convertStr(rhs, val);
}

result_t convert(const Json::Value &rhs, int16_t &val) noexcept
{
	try {
		if (rhs.asInt64() == true) {
			val = ( int16_t ) rhs.asInt64();
			return RES_OK;
		}
	} catch (...) {
	}

	return convertStr(rhs, val);
}

result_t convert(const Json::Value &rhs, int32_t &val) noexcept
{
	try {
		if (rhs.asInt64() == true) {
			val = ( int32_t ) rhs.asInt64();
			return RES_OK;
		}
	} catch (...) {
	}

	return convertStr(rhs, val);
}

result_t convert(const Json::Value &rhs, int64_t &val) noexcept
{
	try {
		if (rhs.asInt64() == true) {
			val = rhs.asInt64();
			return RES_OK;
		}
	} catch (...) {
	}

	return convertStr(rhs, val);
}

result_t convert(const Json::Value &rhs, uint64_t &val) noexcept
{
	try {
		if (rhs.asInt64() == true) {
			val = rhs.asInt64();
			return RES_OK;
		}
	} catch (...) {
	}

	return convertStr(rhs, val);
}

result_t convert(const Json::Value &rhs, uint8_t &val) noexcept
{
	try {
		if (rhs.asInt64() == true) {
			val = ( uint8_t ) rhs.asInt64();
			return RES_OK;
		}
	} catch (...) {
	}

	return convertStr(rhs, val);
}

result_t convert(const Json::Value &rhs, uint16_t &val) noexcept
{
	try {
		if (rhs.asInt64() == true) {
			val = ( uint16_t ) rhs.asInt64();
			return RES_OK;
		}
	} catch (...) {
	}

	return convertStr(rhs, val);
}

result_t convert(const Json::Value &rhs, uint32_t &val) noexcept
{
	try {
		if (rhs.asInt64() == true) {
			val = ( uint32_t ) rhs.asInt64();
			return RES_OK;
		}
	} catch (...) {
	}

	return convertStr(rhs, val);
}

result_t convert(const Json::Value &rhs, float &val) noexcept
{
	try {
		if (rhs.asDouble() == true) {
			val = ( float ) rhs.asDouble();
			return RES_OK;
		}
	} catch (...) {
	}

	return convertStr(rhs, val);
}

result_t convert(const Json::Value &rhs, double &val) noexcept
{
	try {
		if (rhs.asDouble() == true) {
			val = rhs.asDouble();
			return RES_OK;
		}
	} catch (...) {
	}

	return convertStr(rhs, val);
}

} // namespace rgabi::core::js