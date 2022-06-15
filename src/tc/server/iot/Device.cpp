#include <tc/server/iot/Device.h>

namespace tc::server::iot {

Device::Device(const Imei &imei, const std::string id)
 : iImei(imei)
 , iID(id)
 , iTimestamp(tc::SysTime(true))
{
  // nothing to do
}

bool Device::operator==(const Device &rhs) const
{
	return iImei.compare(rhs.imei()) == 0 ? true : false;
}

Device &Device::operator=(const Device &rhs)
{
	iImei = rhs.imei();
	return *this;
}

std::string Device::id() const
{
	return iID;
}

Imei Device::imei() const
{
  return iImei;
}

std::string Device::type() const
{
	return "TST100";
}

int64_t Device::timestamp() const
{
	return iTimestamp.timestamp();
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
	auto packet = std::make_shared<parser::PacketPayload>();

	if ((res = packet->parse((uchar*) buffer, size)) != RES_OK) {
		LG_DBG(this->logger(), "Parse payload packet");
		return res;
	}

	return add(std::move(packet));
}

result_t Device::add(const std::shared_ptr< parser::PacketPayload > &packet)
{
  if (has(packet) == true) {
		LG_DBG(this->logger(), "Packet already exists.");
		return RES_INVARG;
  }

  iPayloadPackets.push_back(std::move(packet));
  return RES_OK;
}

bool Device::has(const std::shared_ptr< parser::PacketPayload > &packet)
{
	if (std::find(iPayloadPackets.begin(), iPayloadPackets.end(), packet) != iPayloadPackets.end()) {
    return false;
  }
	return true;
}

result_t Device::toJsonImpl(Json::Value &rhs, bool root) const
{
	rhs["ID"] = id();
	rhs["Imei"] = imei();
	rhs["Type"] = type();
	rhs["Timestamp"] = timestamp();
	rhs["Packets"] = iPayloadPackets.size();

	return RES_OK;
}

} // namespace tc::server::tcp