#include <tc/server/iot/Vehicle.h>

namespace tc::server::iot {

Vehicle::Vehicle(const Imei &imei)
 : Device (imei, 1000)
{
		// nothing to do
}

Vehicle::Vehicle(const Imei &imei, size_t cache)
 : Device(imei, cache)
{
		// nothing to do
}

bool Vehicle::operator==(const Device &rhs) const
{
	return iImei == rhs.imei();
}

bool Vehicle::operator!=(const Device &rhs) const
{
	return iImei.compare(rhs.imei()) != 0;
}

Vehicle &Vehicle::operator=(const Vehicle &rhs)
{
	iImei = rhs.imei();
	iTimestamp = rhs.timestamp();
	iTotal = rhs.total();
	iType = rhs.type();
	iCacheSize = rhs.cached();
	iID = rhs.id();
	iFleet = rhs.fleet();

	return *this;
}

bool Vehicle::has(const std::shared_ptr< parser::PacketPayload > packet)
{
	auto it = std::find(iPayloadPackets.begin(), iPayloadPackets.end(), packet);
	return it == iPayloadPackets.end() ? false : true;
}

result_t Vehicle::add(const uchar* buffer, size_t size)
{
	if (buffer == nullptr) {
		return RES_INVARG;
	}

	result_t res = RES_OK;
	auto packet = std::make_shared< parser::PacketPayload >();

 	if ((res = packet->parse((uchar*) buffer, size)) != RES_OK) {
		LG_ERR(this->logger(), "Parse payload packet");
		return res;
	}
	return add(std::move(packet));
}

result_t Vehicle::add(const std::shared_ptr< parser::PacketPayload > packet)
{
  if (has(packet) == true) {
		LG_ERR(this->logger(), "Packet already exists.");
		return RES_INVARG;
  }

	if (iPayloadPackets.size() >= iCacheSize) {
		iPayloadPackets.pop_front();
	}
	iPayloadPackets.push_back(std::move(packet));

	return RES_OK;
}

const std::string Vehicle::id() const
{
	return iID;
}

const std::string Vehicle::fleet() const
{
	return iFleet;
}

void Vehicle::setID(const std::string &id)
{
	iID = id;
}

void Vehicle::setFleet(const std::string &fleet)
{
	iFleet = fleet;
}

result_t Vehicle::fromJsonImpl(const Json::Value &rhs, bool root)
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
		else if (!id.compare("ID"))
			iID = rhs[id].asString();
		else if (!id.compare("Fleet"))
			iFleet = rhs[id].asString();
	}

	return RES_OK;
}

result_t Vehicle::toJsonImpl(Json::Value &rhs, bool root) const
{
	auto systime = SysTime(iTimestamp);

	rhs["Imei"] = iImei;
	rhs["ID"] = iID;
	rhs["Fleet"] = iFleet;
	rhs["Type"] = iType;
	rhs["Timestamp"] = iTimestamp;
	rhs["Datetime"] = systime.getDateTime();
	const auto time = SysTime().timestamp(uptime());
	rhs["Uptime"] = fmt::format("{:d}d:{:d}h:{:d}m", (time.getDay() - 1), time.getHour(), time.getMin());
	rhs["Packets"] = iTotal;

	return RES_OK;
}

} // namespace tc::server::iot