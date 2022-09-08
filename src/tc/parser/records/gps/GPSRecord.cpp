#include <tc/parser/records/gps/GPSRecord.h>
//#include <fmt/format.h>
namespace tc::parser::records::gps {

GPSRecord::GPSRecord(const Data &data)
 : iData(data)
{
  // nothing to do
}


GPSRecord::GPSRecord()
 : GPSRecord(Data{})
{
  // nothing to do
}

void GPSRecord::clear()
{
	iData.iAltitude = 0;
	iData.iLatitude = 0;
	iData.iLongitude = 0;
	iData.iAngle = 0;
	iData.iSatellites = 0;
	iData.iSpeed = 0;
}


bool GPSRecord::empty() const
{
	return iData.empty();
}

GPSRecord::Data &GPSRecord::data()
{
  return iData;
}

const GPSRecord::Data &GPSRecord::cdata() const
{
  return iData;
}

result_t GPSRecord::parse(const std::shared_ptr< Reader > &reader, int codec)
{
  return RES_NOIMPL;
}

result_t GPSRecord::parse(const std::shared_ptr< Reader > &reader)
{
  if (reader == nullptr) {
    return RES_INVARG;
  }

  iData.iLongitude = reader->read(4);
  iData.iLatitude = reader->read(4);
  iData.iAltitude = reader->read(2);
  iData.iAngle = reader->read(2);
  iData.iSatellites = reader->read(1);
  iData.iSpeed = reader->read(2);

  return RES_OK;
}

std::string GPSRecord::toString()
{
	/*return !empty() ? fmt::format("**************** GPS Record *****************\
  \n\tLongitude: {}\n\tLatitude: {}\n\tAltitude: {}\n\tAngle: {}\n\tSatellites: {}\n\n",
	iData.iLongitude, iData.iLatitude, iData.iAltitude, iData.iAngle, iData.iSatellites) : fmt::format("************* GPS Record EMPTY *************\n");*/
  return fmt::format("**************** GPS Record *****************\
  \n\tLongitude: {}\n\tLatitude: {}\n\tAltitude: {}\n\tAngle: {}\n\tSatellites: {}\n\n",
	iData.iLongitude, iData.iLatitude, iData.iAltitude, iData.iAngle, iData.iSatellites);
}

result_t GPSRecord::toJsonImpl(Json::Value &rhs, bool root) const
{
	rhs["Longitude"] = iData.iLongitude;
	rhs["Latitude"] = iData.iLatitude;
  rhs["Altitude"] = iData.iAltitude;
  rhs["Angle"] = iData.iAngle;
  rhs["Satellites"] = iData.iSatellites;
  rhs["Speed"] = iData.iSpeed;

	return RES_OK;
}

} // namespace tc::parser::records::gps
