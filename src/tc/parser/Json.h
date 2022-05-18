#ifndef F97C8391_D724_4CBC_98C6_8C46054B1479
#define F97C8391_D724_4CBC_98C6_8C46054B1479

#include <tc/common/Common.h>
#include <json/json.h>

namespace tc::parser {

class JsonI
{
public:
	JsonI() = default;
	virtual ~JsonI() = default;

	virtual result_t fromJson(const uchar *rhs, size_t size) noexcept final;
	virtual result_t fromJson(const std::string &rhs) noexcept final;
	virtual result_t fromJson(const Json::Value &rhs) noexcept final;
	virtual result_t fromJson(const Json::Value &rhs, bool root) noexcept final;

	virtual result_t toJson(std::string &rhs, bool root) const noexcept final;
	virtual result_t toJson(Json::Value &rhs) const noexcept final;
	virtual result_t toJson(Json::Value &rhs, bool root) const noexcept final;
	Json::Value toJson() const noexcept;

protected:
	virtual result_t fromJsonImpl(const Json::Value &rhs, bool root);
	virtual result_t toJsonImpl(Json::Value &rhs, bool root) const;
};

} // namespace tc::parser


#endif /* F97C8391_D724_4CBC_98C6_8C46054B1479 */
