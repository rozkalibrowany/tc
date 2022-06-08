
#include <tc/server/iot/Devices.h>

namespace tc::server::iot {

Devices::Devices(uint32_t max_devices)
 : iMaxDevices(max_devices)
{
	// nothing to do
}

result_t Devices::add(const Imei &imei)
{
	if (has(imei) == true) {
		LG_ERR(this->logger(), "Imei already exists: {}", imei);
		return RES_NOENT;
	}

	auto device = std::make_shared< Device >(imei);
	return add(std::move(device), imei);
}

result_t Devices::add(std::shared_ptr < Device > device, const Imei &imei)
{
	if (device == nullptr) {
		return RES_NOENT;
	}

	if (iDevices.size() >= iMaxDevices) {
		LG_ERR(this->logger(), "Max devices exceeded, max: {}", iMaxDevices);
		return RES_NOENT;
	}

	iDevices.insert(std::make_pair(imei, std::move(device)));
	return RES_OK;
}

result_t Devices::add(const Imei &imei, const std::shared_ptr<parser::PacketPayload> &packet)
{
	if (has(imei) == false) {
		return RES_INVARG;
	}

	auto it = iDevices.find(imei);
	if (it == iDevices.end()) {
		return RES_NOENT;
	}

	it->second->add(std::move(packet));
	return RES_OK;
}


void Devices::printJson()
{
	Json::Value val;
	toJsonImpl(val, true);
	LG_NFO(this->logger(), "CACHE: {}", val.toStyledString());
}

bool Devices::has(const Imei &imei)
{
	return iDevices.find(imei) != iDevices.end();
}

result_t Devices::get(const Imei &imei, std::shared_ptr< Device > &device)
{
	if (has(imei) != true) {
		return RES_NOENT;
	}

	auto it = iDevices.find(imei);
	device = it->second;

	return RES_OK;
}

Json::Value Devices::toJson() const
{
 Json::Value list;
 toJsonImpl(list, true);

 return list;
}

result_t Devices::toJsonImpl(Json::Value &rhs, bool root) const
{
	auto &el = rhs["Devices"] = Json::arrayValue;
	for (auto &d : iDevices) {
		Json::Value val;
		d.second->toJson(val);
		el.append(val);
	}
	return RES_OK;
}

} // namespace tc::server::iot