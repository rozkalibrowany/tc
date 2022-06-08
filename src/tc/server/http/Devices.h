#include <tc/server/iot/Devices.h>

namespace tc::server::http {

class Devices : public iot::Devices
{
public:

	virtual ~Devices() = default;

protected:
	result_t fromJsonImpl(const Json::Value &rhs, bool root) override;
};

result_t Devices::fromJsonImpl(const Json::Value &rhs, bool root)
{
	result_t res = RES_OK;
	auto hasCache = rhs.isMember("cache");

	if (hasCache == false) {
		return RES_NOENT;
	}

	auto &devices = rhs["devices"];
	if (devices.isArray() == true) {
		for (const auto &d : devices) {
			auto device = std::make_shared < iot::Device >();
			result_t r = device->fromJson(d, false);
			if (r == RES_OK) {
				//add(device);
			}
			res |= r;
		}
	} else {
		res |= RES_INVARG;
	}

	return res;
}

} // namespace tc::server::http