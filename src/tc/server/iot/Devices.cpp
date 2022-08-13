
#include <tc/server/iot/Devices.h>
#include <tc/parser/Util.h>

namespace tc::server::iot {

result_t Devices::add(const std::shared_ptr<Device> &device)
{
	if (device->iImei.empty()) {
		return RES_INVARG;
	}

	if (iDevices.find(device->iImei) != iDevices.end()) {
		iDevices.at(device->iImei) = device;
		return RES_OK;
	}

	iDevices.insert(std::make_pair(device->iImei, device));
	return RES_OK;
}

Devices::DeviceMap& Devices::devices()
{
	return iDevices;
}

size_t Devices::size() const
{
	return iDevices.size();
}

bool Devices::has(const Imei &imei) const
{
	return iDevices.find(imei) != iDevices.end();
}

Json::Value Devices::toJson() const
{
	Json::Value list;
	toJsonImpl(list, true);

	return list;
}

result_t Devices::fromJsonImpl(const Json::Value &rhs, bool root)
{
	auto &devices = rhs["devices"];
	auto size = devices.size();

	if (size == 0) {
		return RES_INVARG;
	}

	// add unique devices
	for (auto &dev : devices) {
		if (has(dev["Imei"].asString()) == false) {
			auto device = std::make_shared < iot::Device >();
			if (device->fromJson(dev) != RES_OK) {
				continue;
			}
			add(std::move(device));
		} else {
			auto &device = iDevices.at(dev["Imei"].asString());
			device->fromJson(dev);
		}
	}

	return RES_OK;
}

result_t Devices::toJsonImpl(Json::Value &rhs, bool root) const
{
	auto &el = rhs["devices"] = Json::arrayValue;
	for (auto &d : iDevices) {
		Json::Value val;
		d.second->toJson(val);
		el.append(val);
	}
	return RES_OK;
}

} // namespace tc::server::iot