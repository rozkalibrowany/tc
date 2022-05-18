#ifndef B7996C57_756C_45C2_A7D7_6B3EAA93CBFE
#define B7996C57_756C_45C2_A7D7_6B3EAA93CBFE

#include <tc/common/Common.h>
#include <tc/parser/Json.h>
#include <map>

namespace tc::server::tcp::data {

class Device : public parser::JsonI
{
	using Configuration = std::string;
	using Type = std::string;
	using ID = std::string;

public:
	Device(const Imei imei);
	Device(const Imei imei, const ID id);

	virtual ~Device() = default;

	void setType(const Type type);
	void setConf(const Configuration conf);

	ID iID;
	Imei iImei;
	Type iType;
	Configuration iConfiguration;

protected:
	result_t toJsonImpl(Json::Value &rhs, bool root) const override;};

} // namespace tc::server::tcp::data


#endif /* B7996C57_756C_45C2_A7D7_6B3EAA93CBFE */
