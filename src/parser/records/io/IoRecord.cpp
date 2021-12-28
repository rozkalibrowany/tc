#include <records/io/IoRecord.h>
#include <fmt/format.h>

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

  iRecordsMap = std::move(recordsMap);
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

  return RES_OK;
}

std::string IoRecord::toString()
{
  auto s = fmt::format("************ Io Record ************\
  \n\tEvent ID: {}\n", iEventID);

  if (iRecordsMap[ByteSize::BYTE_1].size() > 0) {
    s += std::string("\tByte 1\n");
    for (auto r : iRecordsMap[ByteSize::BYTE_1]) {
      s += fmt::format("\n\t\tID: {}, val: {}\n", r->iID, r->iValue);
    }
  }
  if (iRecordsMap[ByteSize::BYTE_2].size() > 0) {
    s += std::string("\tByte 2\n");
    for (auto r : iRecordsMap[ByteSize::BYTE_2]) {
      s += fmt::format("\n\t\tID: {}, val: {}\n", r->iID, r->iValue);
    }
  }
  if (iRecordsMap[ByteSize::BYTE_4].size() > 0) {
    s += std::string("\tByte 4\n");
    for (auto r : iRecordsMap[ByteSize::BYTE_4]) {
      s += fmt::format("\n\t\tID: {}, val: {}\n", r->iID, r->iValue);
    }
  }
  if (iRecordsMap[ByteSize::BYTE_8].size() > 0) {
    s += std::string("\tByte 8\n");
    for (auto r : iRecordsMap[ByteSize::BYTE_8]) {
      s += fmt::format("\n\t\tID: {}, val: {}\n", r->iID, r->iValue);
    }
  }
  if (iRecordsMap[ByteSize::BYTE_X].size() > 0) {
    s += std::string("\tByte X\n");
    for (auto r : iRecordsMap[ByteSize::BYTE_X]) {
      s += fmt::format("\n\t\tID: {}, val: {}\n", r->iID, r->iValue);
    }
  }

  return s;
}

} // namespace parser::records::avl