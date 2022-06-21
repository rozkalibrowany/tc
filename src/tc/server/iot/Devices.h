

#ifndef C4F49DD5_45B3_4AAD_8B44_E0716E65EE57
#define C4F49DD5_45B3_4AAD_8B44_E0716E65EE57

#include <tc/common/Common.h>
#include <tc/server/iot/Device.h>
#include <tc/parser/Json.h>

namespace tc::server::iot {

class Devices : public tc::LogI, public parser::JsonI
{
public:
	using DeviceMap = std::map<Imei, std::shared_ptr<Device>>;

	virtual ~Devices() = default;

	result_t add(const std::shared_ptr<Device> &device);
	size_t size() const;
	Json::Value toJson() const;

protected:
	result_t fromJsonImpl(const Json::Value &rhs, bool root) override;
	result_t toJsonImpl(Json::Value &rhs, bool root) const override;

private:
	DeviceMap iDevices;
};

} // namespace tc::server::iot


#endif /* C4F49DD5_45B3_4AAD_8B44_E0716E65EE57 */