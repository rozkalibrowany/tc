
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

size_t Devices::size() const
{
	return iDevices.size();
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

	for (unsigned int i = 0; i < devices.size(); ++i) {
		auto device = std::make_shared < iot::Device >();
		auto &elem = devices[i];
		if (device->fromJson(elem) != RES_OK) {
			continue;
		}
		add(std::move(device));
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