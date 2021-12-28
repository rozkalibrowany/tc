#include <records/gps/GPSRecord.h>

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

GPSRecord& GPSRecord::operator=(const GPSRecord &rhs)
{
	iData = rhs.iData;
	return *this;
}

result_t GPSRecord::parse(const reader::ReaderSPtr &reader)
{
  std::cout << "GPSParser::parse";
  if (reader == nullptr) {
    return RES_INVARG;
  }

  iData.iLongitude = reader->read(4);
  std::cout << "GPSParser::iLongitude " << iData.iLongitude<< std::endl;
  iData.iLatitude = reader->read(4);
  std::cout << "GPSParser::iLatitude " << iData.iLatitude<< std::endl;
  iData.iAltitude = reader->read(2);
  std::cout << "GPSParser::iAltitude " << iData.iAltitude<< std::endl;
  iData.iAngle = reader->read(2);
  std::cout << "GPSParser::iAngle " << iData.iAngle<< std::endl;
  iData.iSatellites = reader->read(1);
  std::cout << "GPSParser::iSatellites " << iData.iSatellites<< std::endl;
  iData.iSpeed = reader->read(2);

  return RES_OK;
}
} // namespace parser::records::gps
