#include <Packet.h>
#include <algorithm>
#include <records/avl/AVLRecord.h>

namespace parser {


result_t Packet::parse(Buf &buf)
{
  if (contains(buf, '8') || contains(buf, '\x8E')) {
      std::cout << "FOUND 8" << std::endl;
      avl::Records records;
      int offset = -1;
      if (getIdx(buf, '\x8E') == 23 || getIdx(buf, '\x8E') == 8)
      {
          offset = getIdx(buf,'\x8E');
      } else if (getIdx(buf, '8') == 23 || getIdx(buf, '8') == 8)
      {
          offset = getIdx(buf, '8');
      }
      auto reader = reader::msptr(buf, offset);

      int codec = reader->readU(1);
      int recordC = reader->readU(1);

      std::cout << "codec:" << codec << "recordC:" << recordC << "offset:" << offset << std::endl;

			for (int i = 0; i < recordC; i++) {
				auto date = reader->readL(8);
				auto prior = reader->read(1);
				std::cout << "date:" << date << "prior:" << prior << std::endl;
				auto record = std::make_shared< avl::Record >(reader, codec);
				gps::GPSRecord gpsRec;
				auto res = record->get(gpsRec);
				//std::cout << "gpsRec res:" << res << std::endl;
				records.add(record);
			}

      //iData = std::make_unique< avl::AVLPacket >(codec, recordC, records);
      //std::cout << "offset:" << offset << "packet rec size" << iData->iRecords.size() << std::endl;
      return RES_OK;
  }
  std::cout << "NOT FOUND 8" << std::endl;

	return RES_NOENT;
}

result_t Packet::parse(unsigned char* cbuf, size_t size)
{
	Buf buffer(cbuf, size);
  return parse(buffer);
}


bool Packet::contains(const Buf &buf, unsigned char c) const
{
    if (std::find(buf.cbegin(), buf.cend(), c) != buf.cend())
        return true;
    return false;
}

int Packet::getIdx(const Buf &buf, const unsigned char c) {
    for (int i = 0; i < 24; i++) {
        if (c == buf[i]) {
            return i;
        }
    }
    return -1;
}

} // namespace parser