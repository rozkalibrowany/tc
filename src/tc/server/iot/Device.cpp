#include <tc/server/iot/Device.h>

namespace tc::server::iot {

Device::Device(const Imei &imei, const std::string id)
 : iImei(imei)
 , iID(id)
 , iType("TST100")
 , iTimestamp(tc::SysTime(true))
 , iPacketsCounter(0LL)
{
  // nothing to do
}

bool Device::operator==(const Device &rhs) const
{
	return iImei == rhs.iImei;
}

Device &Device::operator=(const Device &rhs)
{
	iImei = rhs.iImei;
	iID = rhs.iID;
	iTimestamp = rhs.iTimestamp;
	iPacketsCounter = rhs.iPacketsCounter;
	iType = rhs.iType;
	return *this;
}

bool Device::operator!=(const Device &rhs) const
{
	return iImei.compare(rhs.iImei) != 0;
}

size_t Device::lastRecords() const
{
	return iPayloadPackets.empty() ? 0 : iPayloadPackets.back()->size();
}

result_t Device::add(const uchar* buffer, size_t size)
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

result_t Device::add(const std::shared_ptr< parser::PacketPayload > &packet)
{
  if (has(packet) == true) {
		LG_ERR(this->logger(), "Packet already exists.");
		return RES_INVARG;
  }

	++iPacketsCounter;
	iPayloadPackets.push_back(std::move(packet));
	return RES_OK;
}

bool Device::has(const std::shared_ptr< parser::PacketPayload > &packet)
{
	auto it = std::find(iPayloadPackets.begin(), iPayloadPackets.end(), packet);
	return it == iPayloadPackets.end() ? false : true;
}

result_t Device::fromJsonImpl(const Json::Value &rhs, bool root)
{
	if (rhs.getMemberNames().size() == 0) {
		return RES_INVARG;
	}

	for (auto const& id : rhs.getMemberNames()) {
		if (!id.compare("ID"))
			iID = rhs[id].asString();
		if (!id.compare("Imei"))
			iImei = rhs[id].asString();
		if (!id.compare("Timestamp"))
			iTimestamp = rhs[id].asInt64();
		if (!id.compare("Type"))
			iType = rhs[id].asString();
		if (!id.compare("Packets"))
			iPacketsCounter = rhs[id].asInt64();
	}

	return RES_OK;
}

result_t Device::toJsonImpl(Json::Value &rhs, bool root) const
{
	rhs["ID"] = iID;
	rhs["Imei"] = iImei;
	rhs["Type"] = iType;
	rhs["Timestamp"] = iTimestamp.timestamp();
	rhs["Packets"] = iPacketsCounter;

	return RES_OK;
}

} // namespace tc::server::tcp