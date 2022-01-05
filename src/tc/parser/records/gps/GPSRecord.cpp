#include <tc/parser/records/gps/GPSRecord.h>
#include <fmt/format.h>
namespace parser::records::gps {

GPSRecord::GPSRecord(const GPSData &data)
 : iData(data)
{
  // nothing to do
}


GPSRecord::GPSRecord()
 : GPSRecord(GPSData{})
{
  // nothing to do
}

GPSData &GPSRecord::data()
{
  return iData;
}

const GPSData &GPSRecord::cdata() const
{
  return iData;
}

result_t GPSRecord::parse(const reader::ReaderSPtr &reader, int codec)
{
  return RES_OK;
}

result_t GPSRecord::parse(const reader::ReaderSPtr &reader)
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
 return fmt::format("************ GPS Record ************\
  \n\tLongitude: {}\n\tLatitude: {}\n\tAltitude: {}\n\tAngle: {}\n\tSatellites: {}\n\n",
  iData.iLongitude, iData.iLatitude, iData.iAltitude, iData.iAngle, iData.iSatellites);
}

} // namespace parser::records::gps
