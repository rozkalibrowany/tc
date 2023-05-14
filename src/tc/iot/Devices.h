

#ifndef C4F49DD5_45B3_4AAD_8B44_E0716E65EE57
#define C4F49DD5_45B3_4AAD_8B44_E0716E65EE57

#include <tc/common/Common.h>
#include <tc/iot/Device.h>
#include <tc/parser/Json.h>
#include <tc/common/MagicEnum.h>

namespace tc::server::iot {

template <class T>
class Devices : public tc::LogI, public parser::JsonI
{
public:
	using DeviceMap = std::map<Imei, std::shared_ptr<T>>;
	using iterator = Devices::DeviceMap::iterator;
	using const_iterator = Devices::DeviceMap::const_iterator;

  iterator begin();
	iterator end();
	iterator find(const Imei &imei);

	const_iterator cbegin() const;
	const_iterator cend() const;

	bool operator==(const Devices<T> &rhs) const;

	bool has(const Imei &imei) const;
	result_t add(const std::shared_ptr<T> &device);
	result_t add(const std::shared_ptr<T> &device, const Imei &imei);

	std::vector<Imei> imeis();
	DeviceMap& devices();
	size_t size() const;

	Json::Value toJson(bool active = true);
	result_t fromJsonPacket(const Json::Value &rhs);

protected:
	result_t fromJsonImpl(const Json::Value &rhs, bool active) override;
	result_t toJsonImpl(Json::Value &rhs, bool active_only) const override;

	DeviceMap iDevices;
};

template <class T>
Devices<T>::iterator Devices<T>::begin()
{
	return iDevices.begin();
}

template <class T>
Devices<T>::iterator Devices<T>::end()
{
	return iDevices.end();
}

template <class T>
Devices<T>::const_iterator Devices<T>::cbegin() const
{
	return iDevices.begin();
}

template <class T>
Devices<T>::const_iterator Devices<T>::cend() const
{
	return iDevices.end();
}

template <class T>
bool Devices<T>::operator==(const Devices<T> &rhs) const
{
	return iDevices == rhs.iDevices;
}

template <class T>
Devices<T>::iterator Devices<T>::find(const Imei &imei)
{
	return iDevices.find(imei);
}

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
std::vector<Imei> Devices<T>::imeis()
{
	std::vector<Imei> imeis;
	transform(iDevices.begin(), iDevices.end(), back_inserter(imeis), [](std::pair<Imei, std::shared_ptr<T>> p) { return p.first;} );
	return imeis;
}

template <class T>
size_t Devices<T>::size() const
{
	return iDevices.size();
}

template <class T>
Json::Value Devices<T>::toJson(bool active)
{
	Json::Value list;
	toJsonImpl(list, active);

	return list;
}

template <class T>
result_t Devices<T>::fromJsonPacket(const Json::Value &rhs)
{
	auto &packets = rhs["packets"];
	if (packets.size() == 0) {
		return RES_INVARG;
	}

	for (const auto &records : packets) {
		const auto& imei = records["Imei"].asString();
		if (has(imei) == false)
				continue;

		// FIXME : catching only first for now
		const auto& record = records["Records"][0];
		if (!record.isMember("Record")) {
			LG_WRN(this->logger(), "Record not present in json for imei: {}", imei);
			continue;
		}

		auto packet = std::make_shared<parser::teltonika::Payload>();
		if (packet->fromJson(record["Record"], true) != RES_OK) {
			LG_WRN(this->logger(), "Unable to parse Record for imei: {}", imei);
			continue;
		}

		auto &vehicle = iDevices.at(imei);
		vehicle->add(std::move(packet));
	}

	return RES_OK;
}

template <class T>
result_t Devices<T>::fromJsonImpl(const Json::Value &rhs, bool active)
{
	auto &devices = rhs["devices"];
	auto size = devices.size();

	if (size == 0) {
		return RES_INVARG;
	}

	// add unique devices
	for (auto &dev : devices) {
		auto imei = dev["Imei"].asString();
		auto type = enum_cast<parser::Protocol::Type>(dev["Type"].asString()).value_or(parser::Protocol::eUnknown);
		if (has(imei) == false) {
			auto device = std::make_shared < T >(imei, type);
			if (device->fromJson(dev, active) != RES_OK) {
				continue;
			}
			add(std::move(device));
		} else {
			auto &device = iDevices.at(dev["Imei"].asString());
			device->fromJson(dev, active);
		}
	}

	return RES_OK;
}

template <class T>
result_t Devices<T>::toJsonImpl(Json::Value &rhs, bool active_only) const
{
	auto &el = rhs["devices"] = Json::arrayValue;
	for (auto &d : iDevices) {
		Json::Value val;
		if (d.second->toJson(val, active_only) == RES_OK)
			el.append(val);
	}
	rhs["length"] = el.size();

	return RES_OK;
}

} // namespace tc::server::iot


#endif /* C4F49DD5_45B3_4AAD_8B44_E0716E65EE57 */