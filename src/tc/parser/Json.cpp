#include <tc/parser/Json.h>

namespace tc::parser {

result_t JsonI::fromJson(const uchar *rhs, size_t size) noexcept
{
	try {
		if (size <= 0) {
			return RES_INVARG;
		}

		Json::Value root;
		std::string err;
		Json::CharReaderBuilder builder;
		const std::unique_ptr< Json::CharReader > r(builder.newCharReader());

		bool res = r->parse(( const char * ) rhs, ( const char * ) (rhs + size), &root, &err);
		if (true == res) {
			return fromJson(root);
		}

		return RES_INVARG;

	} catch (...) {
	}
	return RES_ERROR;
}

result_t JsonI::fromJson(const std::string &rhs) noexcept
{
	return fromJson(( const uchar * ) rhs.c_str(), ( size_t ) rhs.size());
}

result_t JsonI::fromJson(const Json::Value &rhs) noexcept
{
	return fromJson(rhs, false);
}

result_t JsonI::fromJson(const Json::Value &rhs, bool root) noexcept
{
	try {
		return fromJsonImpl(rhs, root);
	} catch (...) {
	}
	return RES_ERROR;
}

result_t JsonI::toJson(std::string &rhs, bool root) const noexcept
{
	try {
		Json::Value v;
		result_t res = toJson(v, root);
		if (res != RES_OK) {
			return res;
		}

		rhs = v.toStyledString();
		return RES_OK;
	} catch (...) {
	}
	return RES_ERROR;
}

result_t JsonI::toJson(Json::Value &rhs) const noexcept
{
	try {
		return toJsonImpl(rhs, false);
	} catch (...) {
	}
	return RES_ERROR;
}

result_t JsonI::toJson(Json::Value &rhs, bool root) const noexcept
{
	try {
		return toJsonImpl(rhs, root);
	} catch (...) {
	}
	return RES_ERROR;
}

Json::Value JsonI::toJson() const noexcept
{
	try {
		Json::Value v;
		toJson(v);
		return v;
	} catch (...) {
	}
	return RES_ERROR;
}

result_t JsonI::fromJsonImpl(const Json::Value &rhs, bool root)
{
	return RES_NOIMPL;
}

result_t JsonI::toJsonImpl(Json::Value &rhs, bool root) const
{
	return RES_NOIMPL;
}

} // namespace tc::parser