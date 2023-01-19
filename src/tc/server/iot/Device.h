#ifndef C57632A5_1AB3_4509_93A8_7BA96D45B603
#define C57632A5_1AB3_4509_93A8_7BA96D45B603

#include <tc/common/Common.h>
#include <tc/parser/packet/PacketPayload.h>
#include <tc/common/SysTime.h>

namespace tc::server::iot {

class Device : public tc::LogI, public parser::JsonI
{
public:
  using PayloadPackets = std::deque< std::shared_ptr< parser::PacketPayload > >;

	Device() = default;
	Device(size_t cache, const Imei &imei, const std::string id = "");
	virtual ~Device() = default;

	bool operator==(const Device &rhs) const;
	bool operator!=(const Device &rhs) const;
  Device &operator=(const Device &rhs);

	bool has(const std::shared_ptr< parser::PacketPayload > &packet);

	PayloadPackets &packets();

	result_t add(const uchar* buffer, size_t size);
	result_t add(const std::shared_ptr< parser::PacketPayload > packet);

	size_t lastRecords() const;

	Imei iImei{"unknown"};
	std::string iID;
	std::string iType;
	SysTime iUptime;
	int64_t iTimestamp;
	int64_t iPacketsCounter;

protected:
	result_t fromJsonImpl(const Json::Value &rhs, bool root) override;
	result_t toJsonImpl(Json::Value &rhs, bool root) const override;

	PayloadPackets iPayloadPackets;
	size_t iCacheSize;

private:
	uint64_t getUptime() const;
};

} // namespace tc::server::iot


#endif /* C57632A5_1AB3_4509_93A8_7BA96D45B603 */