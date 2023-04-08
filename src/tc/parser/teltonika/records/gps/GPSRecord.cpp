#include <tc/parser/teltonika/records/gps/GPSRecord.h>

namespace tc::parser::teltonika::records {

GPSRecord::GPSRecord()
{
  // nothing to do
}

result_t GPSRecord::parse(Reader &reader, int codec)
{
  return RES_NOIMPL;
}

result_t GPSRecord::parse(Reader &reader)
{
	auto longitude = reader.read(4);
	if (longitude != 0)
		iLongitude = longitude;

	auto latitude = reader.read(4);
	if (latitude != 0)
		iLatitude = latitude;

	iAltitude = reader.read(2);
	iAngle = reader.read(2);
	iSatellites = reader.read(1);
	iSpeed = reader.read(2);

  return RES_OK;
}

result_t GPSRecord::toJsonImpl(Json::Value &rhs, bool root) const
{
	Json::Value gps;	

	if (iLongitude.has_value())
		gps["Longitude"] = iLongitude.value();
	
	if (iLatitude.has_value())
		gps["Latitude"] = iLatitude.value();

	if (iAltitude.has_value())
		gps["Altitude"] = iAltitude.value();;
	
	if (iAngle.has_value())
		gps["Angle"] = iAngle.value();
	
	if (iSatellites.has_value())
		gps["Satellites"] = iSatellites.value();
	
	if (iSpeed.has_value())
		gps["Speed"] = iSpeed.value();

	rhs["GPS"] = gps;

	return RES_OK;
}

result_t GPSRecord::fromJsonImpl(const Json::Value &rhs, bool root)
{
	const auto& gps = rhs["GPS"];
	if (gps.size() == 0) {
		LG_WRN(this->logger(), "GPS record object empty");
		return RES_INVARG;
	}

	if (gps.isMember("Longitude"))
		iLongitude = gps["Longitude"].asInt();

	if (gps.isMember("Latitude"))
		iLatitude = gps["Latitude"].asInt();
	
	if (gps.isMember("Altitude"))
		iAltitude = gps["Altitude"].asInt();
	
	if (gps.isMember("Angle"))
		iAngle = gps["Angle"].asInt();
	
	if (gps.isMember("Satellites"))
		iSatellites = gps["Satellites"].asInt();
	
	if (gps.isMember("Speed"))
		iSpeed = gps["Speed"].asInt();

	return RES_OK;
}

bool GPSRecord::empty() const
{
	return !iLongitude.has_value() && !iLatitude.has_value();
}

} // namespace tc::parser::teltonika::records
