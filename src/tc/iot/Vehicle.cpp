#include <tc/iot/Vehicle.h>
#include <tc/common/MagicEnum.h>

namespace tc::server::iot {

Vehicle::Vehicle(const Imei &imei, parser::Protocol::Type type, size_t cache, Source source)
 : Device(imei, type, cache)
 , iSource(source)
{
	// nothing to do
}

Vehicle::Vehicle(const Imei &imei, parser::Protocol::Type type, size_t cache)
 : Vehicle(imei, type, cache, eTelematics)
{
	// nothing to do
}

Vehicle::Vehicle(const Imei &imei, parser::Protocol::Type type)
 : Vehicle(imei, type, 10)
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

	if (rhs.fleet().compare("unknown")) {
		iFleet = rhs.fleet();
	}

	if (rhs.id().compare("unknown")) {
		iID = rhs.id();
	}

	if (rhs.iModified > iModified) {
		iModified = rhs.iModified;
	}

	iSource = rhs.iSource;

	return *this;
}

bool Vehicle::has(const std::shared_ptr< parser::Packet > packet)
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
	auto packet = std::make_shared< parser::teltonika::Payload >();

 	if ((res = packet->parse((uchar*) buffer, size)) != RES_OK) {
		LG_ERR(this->logger(), "Parse payload packet");
		return res;
	}
	return add(std::move(packet));
}

result_t Vehicle::add(const std::shared_ptr< parser::Packet > packet)
{
  if (has(packet) == true) {
		/* Logging an error message. */
		LG_ERR(this->logger(), "Packet already exists.");
		return RES_INVARG;
  }

	if (iPayloadPackets.size() >= iCacheSize) {
		iPayloadPackets.pop_front();
	}
	iPayloadPackets.push_back(std::move(packet));

	return RES_OK;
}

result_t Vehicle::set(const std::string &json_doc)
{
	if (json_doc.empty()) {
		return RES_NOENT;
	}

	Json::Value root;
	if (result_t res; (res = fromJsonString(json_doc, root)) != RES_OK) {
		return res;
	}

	auto modified = root["Modified"].asInt64();

	if(iModified > modified) {
		return RES_OK;
	}

	return fromJsonImpl(root, true);
}

const std::string Vehicle::id() const
{
	return iID;
}

const std::string Vehicle::fleet() const
{
	return iFleet;
}

bool Vehicle::online() const
{
	return iSource == eTelematics ? true : false;
}

void Vehicle::setID(const std::string &id)
{
	iID = id;
	return updateModified(SysTime(true).timestamp());
}

void Vehicle::setFleet(const std::string &fleet)
{
	iFleet = fleet;
	return updateModified(SysTime(true).timestamp());
}

result_t Vehicle::fromJsonString(const std::string &json_doc, Json::Value &rhs)
{
	Json::Reader reader;
	bool ok = reader.parse(json_doc.c_str(), rhs);
	return ok ? RES_OK : RES_ERROR;
}

result_t Vehicle::fromJsonImpl(const Json::Value &rhs, bool active)
{
	if (rhs.getMemberNames().size() == 0) {
		return RES_INVARG;
	}

	for (auto const& id : rhs.getMemberNames()) {
		if (!id.compare("Imei"))
			iImei = rhs[id].asString();
		else if (!id.compare("ID") && rhs[id].asString().compare("unknown"))
			iID = rhs[id].asString();
		else if (!id.compare("Fleet") && rhs[id].asString().compare("unknown"))
			iFleet = rhs[id].asString();
		else if (!id.compare("Type"))
			iType = enum_cast<parser::Protocol::Type>(rhs[id].asString()).value_or(parser::Protocol::eUnknown);
		else if (!id.compare("Timestamp"))
			iTimestamp = rhs[id].asInt64();
		else if (!id.compare("Modified") && rhs[id].asInt64() > iModified)
			iModified = rhs[id].asInt64();
		else if (!id.compare("Packets"))
			iTotal = rhs[id].asInt64();
	}

	iSource = active ? eTelematics : eDatabase;

	return RES_OK;
}

result_t Vehicle::toJsonImpl(Json::Value &rhs, bool active_only) const
{
	if (active_only && iSource == eDatabase) {
		return RES_NOENT;
	}

	auto systime = SysTime(iTimestamp);

	rhs["Imei"] = iImei;
	rhs["ID"] = iID;
	rhs["Fleet"] = iFleet;
	rhs["Type"] = std::string(enum_name(iType));
	rhs["Timestamp"] = iTimestamp;
	rhs["Modified"] = iModified;
	rhs["Datetime"] = systime.getDateTime();
	rhs["Online"] = online() ? "True" : "False";
	const auto time = SysTime().timestamp(uptime());
	rhs["Uptime"] = fmt::format("{:d}d:{:d}h:{:d}m", (time.getDay() - 1), time.getHour(), time.getMin());
	rhs["Packets"] = iTotal;

	return RES_OK;
}

void Vehicle::updateModified(int64_t timestamp)
{
	iModified = timestamp;
}

} // namespace tc::server::iot