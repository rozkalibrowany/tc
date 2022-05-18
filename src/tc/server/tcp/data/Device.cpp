#include <tc/server/tcp/data/Device.h>

namespace tc::server::tcp::data {

Device::Device(const Imei imei)
 : Device(imei, "")
 {
	 // nothing to do
 }

Device::Device(const Imei imei, const ID id)
 : iImei(imei)
 , iID(id)
 , iType("TST100")
 , iConfiguration("")
{
	// nothing to do
}

void Device::setType(const Type type) {
	iType = type;
}

void Device::setConf(const Configuration conf)
{
	iConfiguration = conf;
}

result_t Device::toJsonImpl(Json::Value &rhs, bool root) const
{
	rhs["imei"] = iImei;
	rhs["id"] = iID;
	rhs["type"] = iType;
	rhs["configuration"] = iConfiguration;

	return RES_OK;
}

}