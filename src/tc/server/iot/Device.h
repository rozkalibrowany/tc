#ifndef EAD57CC8_F87B_4E24_B54A_C8E0F992FBE7
#define EAD57CC8_F87B_4E24_B54A_C8E0F992FBE7

#include <tc/parser/teltonika/packet/PacketPayload.h>
#include <tc/common/SysTime.h>

namespace tc::server::iot {

class Device : public tc::LogI, public parser::JsonI
{
public:
  using PayloadPackets = std::deque< std::shared_ptr< parser::teltonika::PacketPayload > >;

	explicit Device(const Imei &imei);
	explicit Device(const Imei &imei, size_t cache);

	virtual ~Device() = default;

	virtual bool operator==(const Device &rhs) const;
	virtual bool operator!=(const Device &rhs) const;
	virtual Device &operator=(const Device &rhs);

	virtual bool has(const std::shared_ptr<parser::teltonika::PacketPayload> packet);

	virtual result_t add(const uchar *buffer, size_t size);
	virtual result_t add(const std::shared_ptr<parser::teltonika::PacketPayload> packet);

	virtual Json::Value toJsonValue(bool active = true);

	virtual uint64_t uptime() const;

	virtual size_t cached() const;
	virtual size_t total() const;
	virtual int64_t timestamp() const;
	virtual Imei imei() const;
	virtual std::string type() const;

	virtual PayloadPackets &packets();

protected:
	result_t fromJsonImpl(const Json::Value &rhs, bool root) override;
	result_t toJsonImpl(Json::Value &rhs, bool root) const override;

	Imei iImei{"unknown"};
	size_t iCacheSize;

	std::string iType{"TST100"};

	SysTime iUptime {static_cast<int64_t>(0LL)};
	int64_t iTimestamp{SysTime(true).timestamp()};
	int64_t iTotal{0LL};

	PayloadPackets iPayloadPackets;
};

} // namespace tc::server::iot

#endif /* EAD57CC8_F87B_4E24_B54A_C8E0F992FBE7 */
