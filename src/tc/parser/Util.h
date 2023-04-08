#ifndef D2287EC0_5CFD_4186_8A47_42D2A9981F96
#define D2287EC0_5CFD_4186_8A47_42D2A9981F96

#include <tc/common/Common.h>
#include <tc/common/Buf.h>
#include <tc/parser/Json.h>
#include <tc/parser/Convert.h>

namespace tc::parser {

bool has(const Json::Value &rhs, const char *key) noexcept;

template < typename T >
result_t getArray(const Json::Value &lhs, const char *key, std::vector< T > &rhs)
{
	if (lhs.isMember(key) == false) {
		return RES_NOENT;
	}

	auto &v = lhs[key];
	if (v.isArray() == false) {
		return RES_NOENT;
	}

	rhs.clear();
	for (const auto &val : v) {
		T v;
		result_t res = convert(val, v);
		if (res == RES_OK) {
			rhs.push_back(v);
		}
	}

	return RES_OK;
}

template < typename T >
result_t setArray(Json::Value &lhs, const char *key, const std::vector< T > &rhs)
{
	auto &V = lhs[key] = Json::arrayValue;
	for (const auto &v : rhs) {
		V.append(Json::Value(v));
	}

	return RES_OK;
}

template < typename T >
result_t get_atomic(const Json::Value &lhs, const char *key, T &rhs)
{
	if (lhs.isMember(key) == false) {
		return RES_NOENT;
	}

	typename T::value_type val;
	result_t res = convert(lhs[key], val);
	if (res == RES_OK) {
		rhs = val;
	}

	return res;
}

template < typename T >
result_t get(const Json::Value &lhs, const char *key, T &rhs)
{
	if (lhs.isMember(key) == false) {
		return RES_NOENT;
	}

	return convert(lhs[key], rhs);
}

template < typename T >
result_t get(const Json::Value &lhs, const std::string &key, T &rhs)
{
	return get(lhs, key.c_str(), rhs);
}

template < typename T >
result_t get(const Json::Value &lhs, const char *key, T &rhs, const T &def)
{
	result_t res = get(lhs, key, rhs);
	if (res != RES_OK) {
		rhs = def;
	}
	return RES_OK;
}

template < typename T >
result_t get(const Json::Value &lhs, const std::string &key, T &rhs, const T &def)
{
	return get(lhs, key.c_str(), rhs, def);
}

template < typename T >
result_t set(Json::Value &lhs, const char *key, const T &rhs)
{
	lhs[key] = Value(rhs);
	return RES_OK;
}

template < typename T >
result_t set(Json::Value &lhs, const std::string &key, const T &rhs)
{
	return set(lhs, key.c_str(), rhs);
}

template <>
result_t get< Json::Value >(const Json::Value &lhs, const char *key, Json::Value &rhs);

template <>
result_t get< std::string >(const Json::Value &lhs, const char *key, std::string &rhs);


template <>
result_t get< std::vector< uint64_t > >(const Json::Value &lhs, const char *key, std::vector< uint64_t > &rhs);

template <>
result_t get< std::vector< int64_t > >(const Json::Value &lhs, const char *key, std::vector< int64_t > &rhs);

template <>
result_t get< std::vector< std::string > >(const Json::Value &lhs, const char *key, std::vector< std::string > &rhs);

template <>
result_t set< std::vector< uint64_t > >(Json::Value &lhs, const char *key, const std::vector< uint64_t > &rhs);

template <>
result_t set< std::vector< int64_t > >(Json::Value &lhs, const char *key, const std::vector< int64_t > &rhs);

template <>
result_t set< std::vector< std::string > >(Json::Value &lhs, const char *key, const std::vector< std::string > &rhs);

/**
 * jsoncpp defines
 * Int64 = int64_t;
 * Uint64 = uint64_t;
 * /usr/include/x86_64-linux-gnu/bits/types.h
 * int64_t is not interchangable with int64 -> long long int
 */
template <>
result_t set< int64_t >(Json::Value &lhs, const char *key, const int64_t &rhs);

template <>
result_t set< uint64_t >(Json::Value &lhs, const char *key, const uint64_t &rhs);

template <>
result_t setArray< int64_t >(Json::Value &lhs, const char *key, const std::vector< int64_t > &rhs);

template <>
result_t setArray< uint64_t >(Json::Value &lhs, const char *key, const std::vector< uint64_t > &rhs);

} // namespace rgabi::core::js


#endif /* D2287EC0_5CFD_4186_8A47_42D2A9981F96 */
