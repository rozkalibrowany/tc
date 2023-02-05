

#ifndef C4F49DD5_45B3_4AAD_8B44_E0716E65EE57
#define C4F49DD5_45B3_4AAD_8B44_E0716E65EE57

#include <tc/common/Common.h>
#include <tc/server/iot/Device.h>
#include <tc/parser/Json.h>

namespace tc::server::iot {

template <class T>
class Devices : public tc::LogI, public parser::JsonI
{
public:
	using DeviceMap = std::map<Imei, std::shared_ptr<T>>;

	bool has(const Imei &imei) const;
	result_t add(const std::shared_ptr<T> &device);
	result_t add(const std::shared_ptr<T> &device, const Imei &imei);

	DeviceMap& devices();

	size_t size() const;
	Json::Value toJson();

protected:
	result_t fromJsonImpl(const Json::Value &rhs, bool root) override;
	result_t toJsonImpl(Json::Value &rhs, bool root) const override;

private:
	DeviceMap iDevices;
};


template <class T>
result_t Devices<T>::add(const std::shared_ptr<T> &device)
{
	return add(device, device->imei());
}

template <class T>
result_t Devices<T>::add(const std::shared_ptr<T> &device, const Imei &imei)
{
	if (iDevices.find(imei) != iDevices.end()) {
		iDevices.at(imei) = device;
		return RES_OK;
	}

	iDevices.insert(std::make_pair(imei, device));
	return RES_OK;
}

template <class T>
Devices<T>::DeviceMap& Devices<T>::devices()
{
	return iDevices;
}

template <class T>
bool Devices<T>::has(const Imei &imei) const
{
	return iDevices.find(imei) != iDevices.end();
}

template <class T>
size_t Devices<T>::size() const
{
	return iDevices.size();
}

template <class T>
Json::Value Devices<T>::toJson()
{
	Json::Value list;
	toJsonImpl(list, true);

	return list;
}

template <class T>
result_t Devices<T>::fromJsonImpl(const Json::Value &rhs, bool root)
{
	auto &devices = rhs["devices"];
	auto size = devices.size();

	if (size == 0) {
		return RES_INVARG;
	}

	// add unique devices
	for (auto &dev : devices) {
		auto imei = dev["Imei"].asString();
		if (has(imei) == false) {
			auto device = std::make_shared < T >(imei);
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

template <class T>
result_t Devices<T>::toJsonImpl(Json::Value &rhs, bool root) const
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


#endif /* C4F49DD5_45B3_4AAD_8B44_E0716E65EE57 */