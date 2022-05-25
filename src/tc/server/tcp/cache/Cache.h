#ifndef C4F49DD5_45B3_4AAD_8B44_E0716E65EE57
#define C4F49DD5_45B3_4AAD_8B44_E0716E65EE57

#include <tc/common/Common.h>
#include <tc/server/tcp/cache/Device.h>
#include <tc/parser/Json.h>

namespace tc::server::tcp {

class Cache : public tc::LogI, public parser::JsonI
{
public:
	using Devices = std::map< Imei, std::shared_ptr < Device > >;

  Cache(uint32_t max_devices = 150);
	virtual ~Cache() = default;

	void printJson();

	virtual bool has(const Imei &imei);
	virtual result_t add(const Imei &imei);
	virtual result_t add(std::shared_ptr<Device> device, const Imei &imei);
	virtual result_t add(const Imei &imei, const std::shared_ptr<parser::PacketPayload> &packet);
	virtual result_t get(const Imei &imei, std::shared_ptr< Device > &device);

protected:
	result_t toJsonImpl(Json::Value &rhs, bool root) const override;

	uint32_t iMaxDevices;
	Devices iDevices;
};

} // namespace tc::server::tcp


#endif /* C4F49DD5_45B3_4AAD_8B44_E0716E65EE57 */
