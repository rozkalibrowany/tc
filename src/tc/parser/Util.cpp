#include <tc/parser/Util.h>

namespace tc::parser {

bool has(const Json::Value &rhs, const char *key) noexcept
{
	try {
		return rhs.isMember(key);
	} catch (...) {
		return false;
	}
}

template <>
result_t get<  Json::Value >(const Json::Value &lhs, const char *key, Json::Value &rhs)
{
	if (lhs.isMember(key) == false) {
		return RES_NOENT;
	}

	rhs = lhs[key];
	return RES_OK;
}

template <>
result_t get< std::string >(const Json::Value &lhs, const char *key, std::string &rhs)
{
	if (lhs.isMember(key) == false) {
		return RES_NOENT;
	}

	const auto &v = lhs[key];
	if (v.isString() == false) {
		return RES_NOENT;
	}

	rhs = v.asString();
	return RES_OK;
}

template <>
result_t get< std::vector< uint64_t > >(const Json::Value &lhs, const char *key, std::vector< uint64_t > &rhs)
{
	return getArray(lhs, key, rhs);
}

template <>
result_t get< std::vector< int64_t > >(const Json::Value &lhs, const char *key, std::vector< int64_t > &rhs)
{
	return getArray(lhs, key, rhs);
}

template <>
result_t get< std::vector< std::string > >(const Json::Value &lhs, const char *key, std::vector< std::string > &rhs)
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
		rhs.push_back(val.asString());
	}

	return RES_OK;
}

template <>
result_t set< std::vector< uint64_t > >(Json::Value &lhs, const char *key, const std::vector< uint64_t > &rhs)
{
	return setArray(lhs, key, rhs);
}

template <>
result_t set< std::vector< int64_t > >(Json::Value &lhs, const char *key, const std::vector< int64_t > &rhs)
{
	return setArray(lhs, key, rhs);
}

template <>
result_t set< std::vector< std::string > >(Json::Value &lhs, const char *key, const std::vector< std::string > &rhs)
{
	return setArray(lhs, key, rhs);
}

template <>
result_t set< int64_t >(Json::Value &lhs, const char *key, const int64_t &rhs)
{
	return set(lhs, key, (int64_t)(rhs));
}

template <>
result_t set< uint64_t >(Json::Value &lhs, const char *key, const uint64_t &rhs)
{
	return set(lhs, key, (uint64_t)(rhs));
}

template <>
result_t setArray< int64_t >(Json::Value &lhs, const char *key, const std::vector< int64_t > &rhs)
{
	auto &V = lhs[key] = Json::arrayValue;
	for (const auto &v : rhs) {
		V.append(Json::Value(( int64_t ) v));
	}

	return RES_OK;
}

template <>
result_t setArray< uint64_t >(Json::Value &lhs, const char *key, const std::vector< uint64_t > &rhs)
{
	auto &V = lhs[key] = Json::arrayValue;
	for (const auto &v : rhs) {
		V.append(Json::Value(( uint64_t ) v));
	}

	return RES_OK;
}

} // namespace tc::parser