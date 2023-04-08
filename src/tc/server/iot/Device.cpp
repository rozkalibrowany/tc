#include <tc/server/iot/Device.h>

namespace tc::server::iot {

Device::Device(const Imei &imei)
 : Device (imei, 100)
{
		// nothing to do
}

Device::Device(const Imei &imei, size_t cache)
 : iImei(imei)
 , iCacheSize(cache)
{
		// nothing to do
}

bool Device::operator==(const Device &rhs) const
{
	return iImei == rhs.imei();
}

bool Device::operator!=(const Device &rhs) const
{
	return iImei.compare(rhs.imei()) != 0;
}

Device &Device::operator=(const Device &rhs)
{
	iImei = rhs.imei();
	iTimestamp = rhs.timestamp();
	iTotal = rhs.total();
	iType = rhs.type();
	iCacheSize = rhs.cached();

	return *this;
}

bool Device::has(const std::shared_ptr< parser::teltonika::PacketPayload > packet)
{
	auto it = std::find(iPayloadPackets.begin(), iPayloadPackets.end(), packet);
	return it == iPayloadPackets.end() ? false : true;
}

result_t Device::add(const uchar* buffer, size_t size)
{
	if (buffer == nullptr) {
		return RES_INVARG;
	}

	result_t res = RES_OK;
	auto packet = std::make_shared< parser::teltonika::PacketPayload >();

 	if ((res = packet->parse((uchar*) buffer, size)) != RES_OK) {
		LG_ERR(this->logger(), "Parse payload packet");
		return res;
	}
	return add(std::move(packet));
}

result_t Device::add(const std::shared_ptr< parser::teltonika::PacketPayload > packet)
{
  if (has(packet) == true) {
		LG_ERR(this->logger(), "Packet already exists.");
		return RES_INVARG;
  }

	++iTotal;
	if (iPayloadPackets.size() >= iCacheSize) {
		iPayloadPackets.pop_front();
	}
	iPayloadPackets.push_back(std::move(packet));

	return RES_OK;
}

Json::Value Device::toJsonValue(bool active)
{
	Json::Value val;
	toJsonImpl(val, active);

	return val;
}

Device::PayloadPackets &Device::packets()
{
	return iPayloadPackets;
}

Imei Device::imei() const
{
	return iImei;
}

size_t Device::total() const
{
	return iTotal;
}

std::string Device::type() const
{
	return iType;
}

int64_t Device::timestamp() const
{
	return iTimestamp;
}

size_t Device::cached() const
{
	return iPayloadPackets.size();
}

uint64_t Device::uptime() const
{
	return SysTime(true).timestamp() - iTimestamp;
}

result_t Device::fromJsonImpl(const Json::Value &rhs, bool root)
{
	if (rhs.getMemberNames().size() == 0) {
		return RES_INVARG;
	}

	for (auto const& id : rhs.getMemberNames()) {
		if (!id.compare("Imei"))
			iImei = rhs[id].asString();
		else if (!id.compare("Timestamp"))
			iTimestamp = rhs[id].asInt64();
		else if (!id.compare("Type"))
			iType = rhs[id].asString();
		else if (!id.compare("Packets"))
			iTotal = rhs[id].asInt64();
	}

	return RES_OK;
}

result_t Device::toJsonImpl(Json::Value &rhs, bool root) const
{
	auto systime = SysTime(iTimestamp);

	rhs["Imei"] = iImei;
	rhs["Type"] = iType;
	rhs["Timestamp"] = iTimestamp;
	rhs["Datetime"] = systime.getDateTime();
	const auto time = SysTime().timestamp(uptime());
	rhs["Uptime"] = fmt::format("{:d}d:{:d}h:{:d}m", (time.getDay() - 1), time.getHour(), time.getMin());
	rhs["Packets"] = iTotal;

	return RES_OK;
}


} // namespace tc::server::iot