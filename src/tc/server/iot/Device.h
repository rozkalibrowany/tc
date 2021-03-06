#ifndef C57632A5_1AB3_4509_93A8_7BA96D45B603
#define C57632A5_1AB3_4509_93A8_7BA96D45B603

#include <tc/common/Common.h>
#include <tc/parser/packet/PacketPayload.h>
#include <tc/common/SysTime.h>

namespace tc::server::iot {

class Device : public tc::LogI, public parser::JsonI
{
public:
  using PayloadPackets = std::vector< std::shared_ptr< parser::PacketPayload > >;

	Device() = default;
  Device(const Imei &imei, const std::string id = "");
	virtual ~Device() = default;

	bool operator==(const Device &rhs) const;
	bool operator!=(const Device &rhs) const;
  Device &operator=(const Device &rhs);

	result_t add(const uchar* buffer, size_t size);
	result_t add(const std::shared_ptr< parser::PacketPayload > &packet);
	bool has(const std::shared_ptr< parser::PacketPayload > &packet);
	size_t lastRecords() const;


	Imei iImei;
	std::string iID;
	std::string iType;
	SysTime iTimestamp;
	int64_t iPacketsCounter;

protected:
	result_t fromJsonImpl(const Json::Value &rhs, bool root) override;
	result_t toJsonImpl(Json::Value &rhs, bool root) const override;

	PayloadPackets iPayloadPackets;

};

} // namespace tc::server::iot


#endif /* C57632A5_1AB3_4509_93A8_7BA96D45B603 */