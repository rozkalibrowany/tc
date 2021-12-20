#ifndef BABBB09F_CF25_45B7_ADC7_3741FD210829
#define BABBB09F_CF25_45B7_ADC7_3741FD210829

#include <records/gps/GPSRecord.h>
#include <records/ParserI.h>
#include <reader/Reader.h>
#include <type_traits>

namespace parser::records::gps {

class GPSParser : public ParserI< GPSParser > {
public:

  GPSParser(reader::ReaderSPtr &reader) : ParserI< GPSParser >(reader) {}
  virtual ~GPSParser() {}

  template<typename T>
  result_t parse(std::shared_ptr<T> &record);
};

template<typename T>
result_t GPSParser::parse(std::shared_ptr<T> &record)
{
  std::cout << "GPSParser::parse";
  if constexpr (!std::is_same_v<T, GPSRecord>) {
    return RES_NOENT;
  }

  record->iData.iLongitude = iReader->read(4);
  std::cout << "GPSParser::iLongitude " << record->iData.iLongitude<< std::endl;
  record->iData.iLatitude = iReader->read(4);
  std::cout << "GPSParser::iLatitude " << record->iData.iLatitude<< std::endl;
  record->iData.iAltitude = iReader->read(2);
  std::cout << "GPSParser::iAltitude " << record->iData.iAltitude<< std::endl;
  record->iData.iAngle = iReader->read(2);
  std::cout << "GPSParser::iAngle " << record->iData.iAngle<< std::endl;
  record->iData.iSatellites = iReader->read(1);
  std::cout << "GPSParser::iSatellites " << record->iData.iSatellites<< std::endl;
  record->iData.iSpeed = iReader->read(2);

  return RES_OK;
}

} // namespace parser::records::gps

#endif /* BABBB09F_CF25_45B7_ADC7_3741FD210829 */
