

#ifndef C4F49DD5_45B3_4AAD_8B44_E0716E65EE57
#define C4F49DD5_45B3_4AAD_8B44_E0716E65EE57

#include <tc/common/Common.h>
#include <tc/server/iot/Device.h>
#include <tc/parser/Json.h>

namespace tc::server::iot {

class Devices : public tc::LogI, public parser::JsonI
{
public:
	using DevicesMap = std::map< Imei, std::shared_ptr < Device > >;

  Devices(uint32_t max_devices = 150);
	virtual ~Devices() = default;

	void printJson();

	virtual bool has(const Imei &imei);
	virtual result_t add(const Imei &imei);
	virtual result_t add(const std::shared_ptr<Device> &device);
	virtual result_t add(std::shared_ptr<Device> device, const Imei &imei);
	virtual result_t add(const Imei &imei, const std::shared_ptr<parser::PacketPayload> &packet);
	virtual result_t get(const Imei &imei, std::shared_ptr< Device > &device);

	Json::Value toJson() const;

protected:
	result_t fromJsonImpl(const Json::Value &rhs, bool root) override;
	result_t toJsonImpl(Json::Value &rhs, bool root) const override;

	uint32_t iMaxDevices;
	DevicesMap iDevices;
};

} // namespace tc::server::iot


#endif /* C4F49DD5_45B3_4AAD_8B44_E0716E65EE57 */