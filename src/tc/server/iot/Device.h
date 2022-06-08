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

  Device(const Imei &imei, const std::string id = "");
	virtual ~Device() = default;

	bool operator==(const Device &rhs) const;
  Device &operator=(const Device &rhs);

	std::string id() const;
	Imei imei() const;
	std::string type() const;
	int64_t timestamp() const;

	result_t add(const std::shared_ptr< parser::PacketPayload > &packet);

protected:
	result_t toJsonImpl(Json::Value &rhs, bool root) const override;

private:
	std::string iID;
	Imei iImei;
	SysTime iTimestamp;
  PayloadPackets iPayloadPackets;
};

} // namespace tc::server::iot


#endif /* C57632A5_1AB3_4509_93A8_7BA96D45B603 */