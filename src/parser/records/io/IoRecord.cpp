#include <records/io/IoRecord.h>

namespace parser::records::io {

IoRecord::IoRecord(int eventID, int elements)
 : iEventID(eventID)
 , iElements(elements)
 , iByteSizes(std::vector< ByteSize >{ByteSize::BYTE_1, ByteSize::BYTE_2, ByteSize::BYTE_4, ByteSize::BYTE_8, ByteSize::BYTE_X})
{
  // nothing to do
}

IoRecord::IoRecord()
 : IoRecord(0, 0)// RecordsIoMap{})
{
  // nothing to do
}

result_t IoRecord::parse(const reader::ReaderSPtr &reader, int codec)
{
  if (reader == nullptr) {
    return RES_INVARG;
  }
  int ioIdSize = codec == 142 ? 2 : 1;

  iEventID = static_cast<int>(reader->readU(ioIdSize));
  iElements = static_cast<int>(reader->readU(ioIdSize));

  result_t res;
  RecordsIoMap recordsMap;

  for (auto byteSize : iByteSizes) {
    RecordsIoPropertyList propertyList;
    if (byteSize == ByteSize::BYTE_X && (int) ioIdSize == 2) {
      auto recordsCount = reader->readU(ioIdSize);
      for (int i = 0; i < recordsCount; i++) {
        auto io_property = std::make_shared< RecordIoProperty >();
        io_property->parse(reader, ioIdSize);
        auto buf = reader->copy(io_property->iValue);
        switch(io_property->iID) {
          case 10358:
            break;
          default:
            io_property->iValue = buf.size();
            reader->skip(buf.size());
            break;
        }
        propertyList.push_back(io_property);
      }
      continue;
    }
    int recordsCount = reader->readU(ioIdSize);
    for (int i = 0; i < recordsCount; i++) {
      auto io_property = std::make_shared< RecordIoProperty >();
      io_property->parse(reader, ioIdSize, (int) byteSize);
      propertyList.push_back(io_property);
    }
    recordsMap.insert(std::make_pair(byteSize, propertyList));
  }

  return RES_OK;
}
result_t RecordIoProperty::parse(const reader::ReaderSPtr &reader, int id_size)
{
  if (reader == nullptr) {
    return RES_INVARG;
  }

  iID = reader->readU(id_size);
  iValue = reader->readU(2);

  return RES_OK;
}

result_t RecordIoProperty::parse(const reader::ReaderSPtr &reader, int id_size, int val_size)
{
  if (reader == nullptr) {
    return RES_INVARG;
  }

  iID = reader->readU(id_size);
  iValue = reader->read(val_size);
  std::cout << "RecordIoProperty iID " << iID <<  " iValue " << iValue << std::endl;

  return RES_OK;
}

} // namespace parser::records::avl